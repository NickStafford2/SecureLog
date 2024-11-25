// crypto.cpp
#include <memory>
#include <openssl/aes.h>  // Include AES cipher from OpenSSL
#include <openssl/evp.h>  // Include EVP (high-level cryptographic functions)
#include <openssl/rand.h> // Include OpenSSL's random number generation
#include <openssl/sha.h>  // Include OpenSSL's SHA (Secure Hash Algorithm)
#include <stdexcept>
#include <string>
#include <vector>

#include "crypto.h" // Include the header for the CryptoUtils class

// Feel free to look this over. This took forever and is quite complicated. I
// tried to use well known libraries and use all the features to make the
// encryption as secure as possible.

// This is a required object for clearing memory.
// Custom deleter for EVP_MD_CTX, which is used in hash operations
// This ensures proper memory cleanup for OpenSSL contexts.
struct EVP_MD_CTX_Deleter {
  void operator()(EVP_MD_CTX *ptr) const { EVP_MD_CTX_free(ptr); }
};

// Encrypt function using AES 256 CBC mode
std::string CryptoUtils::encrypt(const std::string &plaintext,
                                 const std::string &key) {
  try {
    // Step 1: Derive the encryption key and generate a random IV
    auto derived_key = deriveKey(key);
    auto iv = generateIV();

    // Step 2: Initialize encryption context for AES-256-CBC
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    if (!ctx) {
      throw std::runtime_error("Failed to create encryption context");
    }

    // Step 3: Initialize the encryption operation with AES-256-CBC
    if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      throw std::runtime_error("Failed to initialize encryption");
    }

    // Step 4: Pad the plaintext to match AES block size
    auto padded_data = addPadding(
        std::vector<unsigned char>(plaintext.begin(), plaintext.end()));

    // Step 5: Prepare a buffer to hold the ciphertext (with extra space for
    // padding)
    std::vector<unsigned char> ciphertext(padded_data.size() +
                                          EVP_MAX_BLOCK_LENGTH);
    int ciphertext_length = 0, final_length = 0;

    // Step 6: Encrypt the padded data
    if (1 != EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &ciphertext_length,
                               padded_data.data(), padded_data.size())) {
      throw std::runtime_error("Failed to update encryption with data");
    }

    // Step 7: Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx.get(),
                                 ciphertext.data() + ciphertext_length,
                                 &final_length)) {
      throw std::runtime_error("Failed to finalize encryption");
    }

    // Step 8: Combine IV and ciphertext into the final encrypted string
    std::string result;
    result.reserve(iv.size() + ciphertext_length + final_length);
    result.append(reinterpret_cast<char *>(iv.data()), iv.size()); // Append IV
    result.append(reinterpret_cast<char *>(ciphertext.data()),
                  ciphertext_length + final_length); // Append ciphertext

    return result;
  } catch (const std::exception &e) {
    // Consider logging the exception message for better debugging
    return "";
  }
}

// Decrypt function using AES 256 in CBC mode
std::string CryptoUtils::decrypt(const std::string &ciphertext,
                                 const std::string &key) {
  try {
    // Ensure ciphertext is large enough (must be larger than BLOCK_SIZE)
    if (ciphertext.size() <= BLOCK_SIZE) {
      throw std::invalid_argument("Ciphertext size is too small");
    }

    // Step 1: Extract IV from the ciphertext
    std::vector<unsigned char> iv(ciphertext.begin(),
                                  ciphertext.begin() + BLOCK_SIZE);
    // Derive the encryption key from the provided key
    auto derived_key = deriveKey(key);

    // Step 2: Initialize decryption context for AES-256-CBC
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    if (!ctx) {
      throw std::runtime_error("Failed to create decryption context");
    }

    // Step 3: Initialize the decryption operation
    if (1 != EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      throw std::runtime_error("Failed to initialize decryption");
    }

    // Step 4: Prepare a buffer to hold the plaintext
    std::vector<unsigned char> plaintext(ciphertext.size() - BLOCK_SIZE);
    int plaintext_length = 0, final_length = 0;

    // Step 5: Decrypt the data
    if (1 != EVP_DecryptUpdate(ctx.get(), plaintext.data(), &plaintext_length,
                               reinterpret_cast<const unsigned char *>(
                                   ciphertext.data() + BLOCK_SIZE),
                               ciphertext.size() - BLOCK_SIZE)) {
      throw std::runtime_error("Failed to update decryption with ciphertext");
    }

    // Step 6: Finalize the decryption
    if (1 != EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + plaintext_length,
                                 &final_length)) {
      throw std::runtime_error("Failed to finalize decryption");
    }

    // Step 7: Remove padding and convert to string
    auto unpadded_data = removePadding(std::vector<unsigned char>(
        plaintext.begin(),
        plaintext.begin() + plaintext_length + final_length));

    return std::string(unpadded_data.begin(), unpadded_data.end());
  } catch (const std::exception &e) {
    // Consider logging the exception message for better debugging
    throw std::runtime_error("integrity violation");
  }
}

// Key derivation function using SHA-256 (to generate a secure key from the
// password)
std::vector<unsigned char> CryptoUtils::deriveKey(const std::string &key) {
  // Step 1: Create a vector to store the derived key. The length is defined by
  // KEY_SIZE. This vector will hold the result of the SHA-256 hash, which will
  // be the derived key.
  std::vector<unsigned char> derived_key(KEY_SIZE);

  // Step 2: Create a unique pointer to an EVP_MD_CTX (OpenSSL's message digest
  // context) This context will be used to compute the SHA-256 hash.
  std::unique_ptr<EVP_MD_CTX, EVP_MD_CTX_Deleter> ctx(EVP_MD_CTX_new());

  // If the context could not be created, throw an error.
  if (!ctx) {
    throw std::runtime_error("Failed to create digest context");
  }

  // Step 3: Initialize the SHA-256 hash function within the EVP_MD_CTX.
  // EVP_sha256() selects the SHA-256 algorithm. The third argument is for
  // optional initialization data, which is NULL in this case.
  if (1 != EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr)) {
    throw std::runtime_error("Failed to initialize SHA-256 digest");
  }

  // Step 4: Update the digest context with the input data (the password).
  // This will feed the password string (key) into the SHA-256 hash function.
  if (1 != EVP_DigestUpdate(ctx.get(), key.c_str(), key.length())) {
    throw std::runtime_error("Failed to update SHA-256 digest");
  }

  // Step 5: Finalize the hash computation and store the result in derived_key.
  // This step actually computes the hash and stores it in the provided vector.
  unsigned int digest_len;
  if (1 != EVP_DigestFinal_ex(ctx.get(), derived_key.data(), &digest_len)) {
    throw std::runtime_error("Failed to finalize SHA-256 digest");
  }

  // Step 6: Return the derived key as a vector of unsigned characters.
  // This vector now contains the 32-byte hash derived from the original
  // password.
  return derived_key;
}

// Generate a cryptographically secure random IV using OpenSSL's RAND_bytes
//
// I found this online and I think we need this.
// An IV, or Initialization Vector, is a random or pseudo-random value used in
// cryptography to add unpredictability and ensure that the same plaintext, when
// encrypted multiple times with the same key, results in different ciphertexts.
// This prevents patterns from emerging in encrypted data, thereby enhancing
// security.
std::vector<unsigned char> CryptoUtils::generateIV() {
  std::vector<unsigned char> iv(BLOCK_SIZE);

  if (1 != RAND_bytes(iv.data(), BLOCK_SIZE)) {
    throw std::runtime_error("Failed to generate IV");
  }
  return iv;
}

// Add padding to ensure the data size is a multiple of the AES block size
std::vector<unsigned char>
CryptoUtils::addPadding(const std::vector<unsigned char> &data) {
  size_t padding_len = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
  std::vector<unsigned char> padded = data;
  padded.resize(data.size() + padding_len, padding_len); // Append padding
  return padded;
}

// Remove padding after decryption
std::vector<unsigned char>
CryptoUtils::removePadding(const std::vector<unsigned char> &data) {
  if (data.empty()) {
    return data;
  }

  unsigned char padding_len = data.back();
  if (padding_len > BLOCK_SIZE || padding_len > data.size()) {
    throw std::runtime_error("Invalid padding length");
  }

  for (size_t i = data.size() - padding_len; i < data.size(); ++i) {
    if (data[i] != padding_len) {
      throw std::runtime_error("Invalid padding bytes");
    }
  }

  return std::vector<unsigned char>(data.begin(), data.end() - padding_len);
}
