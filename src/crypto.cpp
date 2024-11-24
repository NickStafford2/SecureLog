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

// feel free to look this over. This took forever and is quite complicated. I
// tried to use well known libraries and use all the features to make the
// encryption as secure as possible.

// This is a required object for clearing memory. Custom deleter for OpenSSL
// EVP_MD_CTX (message digest context) to free it correctly.
struct EVP_MD_CTX_Deleter {
  void operator()(EVP_MD_CTX *ptr) const { EVP_MD_CTX_free(ptr); }
};

// Encrypt function using AES 256 CBC mode
std::string CryptoUtils::encrypt(const std::string &plaintext,
                                 const std::string &key) {
  try {
    // Derive a new key from the user-provided key and generate an
    // initialization vector (IV)
    auto derived_key = deriveKey(key);
    auto iv = generateIV();

    // Initialize encryption context using OpenSSL's EVP_CIPHER_CTX
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    if (!ctx) {
      return "";
    }

    // Initialize the encryption operation with AES-256-CBC mode, the derived
    // key, and the IV
    if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      return "";
    }

    // Prepare input data by adding padding to it (for AES block size
    // compliance)
    auto padded_data = addPadding(
        std::vector<unsigned char>(plaintext.begin(), plaintext.end()));

    // Prepare an output buffer to hold the ciphertext
    // Adding extra space for padding
    std::vector<unsigned char> ciphertext(padded_data.size() +
                                          EVP_MAX_BLOCK_LENGTH);

    int out_len1 = 0, out_len2 = 0;

    // Perform encryption (update the ciphertext with the encrypted data)
    if (1 != EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &out_len1,
                               padded_data.data(), padded_data.size())) {
      return "";
    }

    // Finalize the encryption (write the final encrypted block)
    if (1 != EVP_EncryptFinal_ex(ctx.get(), ciphertext.data() + out_len1,
                                 &out_len2)) {
      return "";
    }

    // Combine IV and ciphertext into the final result
    std::string result;
    // Reserve space for IV + ciphertext
    result.reserve(iv.size() + out_len1 + out_len2);
    // Add IV to the result
    result.append(reinterpret_cast<char *>(iv.data()), iv.size());
    result.append(reinterpret_cast<char *>(ciphertext.data()),
                  out_len1 + out_len2); // Add ciphertext to the result

    return result;
  } catch (...) {
    return "";
  }
}

// Decrypt function using AES 256 in normal CBC mode
std::string CryptoUtils::decrypt(const std::string &ciphertext,
                                 const std::string &key) {
  try {
    // Return empty string if ciphertext is too short (invalid)
    if (ciphertext.size() <= BLOCK_SIZE) {
      return "";
    }

    // Extract the IV from the beginning of the ciphertext
    std::vector<unsigned char> iv(ciphertext.begin(),
                                  ciphertext.begin() + BLOCK_SIZE);
    // Derive the key from the provided key
    auto derived_key = deriveKey(key);

    // Initialize decryption context using OpenSSL's EVP_CIPHER_CTX
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    if (!ctx) {
      return "";
    }

    // Initialize the decryption operation with AES-256-CBC mode, the derived
    // key, and the IV
    if (1 != EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      return "";
    }

    // Prepare an output buffer to hold the plaintext
    std::vector<unsigned char> plaintext(ciphertext.size() - BLOCK_SIZE);
    int out_len1 = 0, out_len2 = 0;

    // Perform decryption (update the plaintext with decrypted data)
    if (1 != EVP_DecryptUpdate(ctx.get(), plaintext.data(), &out_len1,
                               reinterpret_cast<const unsigned char *>(
                                   ciphertext.data() + BLOCK_SIZE),
                               ciphertext.size() - BLOCK_SIZE)) {
      return "";
    }

    // Finalize the decryption (write the final decrypted block)
    if (1 != EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + out_len1,
                                 &out_len2)) {
      return "";
    }

    // Remove padding from decrypted data and convert it back to string
    auto unpadded_data = removePadding(std::vector<unsigned char>(
        plaintext.begin(), plaintext.begin() + out_len1 + out_len2));
    // Return the decrypted string
    return std::string(unpadded_data.begin(), unpadded_data.end());
  } catch (...) {
    return "";
  }
}

// Key derivation using SHA-256
std::vector<unsigned char> CryptoUtils::deriveKey(const std::string &key) {
  // Create a vector to hold the derived key
  std::vector<unsigned char> derived_key(KEY_SIZE);

  // Create a message digest context using OpenSSL's EVP_MD_CTX
  std::unique_ptr<EVP_MD_CTX, EVP_MD_CTX_Deleter> ctx(EVP_MD_CTX_new());

  if (!ctx) {
    throw std::runtime_error("Failed to create digest context");
  }

  // Initialize the SHA-256 hash function
  if (1 != EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr)) {
    throw std::runtime_error("Failed to initialize digest");
  }

  // Update the digest with the key data
  if (1 != EVP_DigestUpdate(ctx.get(), key.c_str(), key.length())) {
    throw std::runtime_error("Failed to update digest");
  }

  unsigned int digest_len;
  // Finalize the digest and store the result in the derived key
  if (1 != EVP_DigestFinal_ex(ctx.get(), derived_key.data(), &digest_len)) {
    throw std::runtime_error("Failed to finalize digest");
  }

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

  // Generate random bytes for the IV
  if (1 != RAND_bytes(iv.data(), BLOCK_SIZE)) {
    throw std::runtime_error("Failed to generate IV");
  }
  return iv;
}

// Add padding to data to make it a multiple of the block size (AES block size)
std::vector<unsigned char>
CryptoUtils::addPadding(const std::vector<unsigned char> &data) {
  // Calculate how much padding is needed
  size_t padding_len = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
  std::vector<unsigned char> padded = data;
  padded.resize(data.size() + padding_len, padding_len);
  return padded;
}

// Remove padding from data after decryption
std::vector<unsigned char>
CryptoUtils::removePadding(const std::vector<unsigned char> &data) {
  if (data.empty()) {
    return data;
  }
  // The padding length is stored in the last byte
  unsigned char padding_len = data.back();
  if (padding_len > BLOCK_SIZE || padding_len > data.size()) {
    throw std::runtime_error("Invalid padding");
  }

  // Check that the padding bytes are valid
  for (size_t i = data.size() - padding_len; i < data.size(); ++i) {
    if (data[i] != padding_len) {
      throw std::runtime_error("Invalid padding");
    }
  }

  return std::vector<unsigned char>(data.begin(), data.end() - padding_len);
}
