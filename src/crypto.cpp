// crypto.cpp
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#include "crypto.h"

// Custom deleter for OpenSSL contexts
struct EVP_MD_CTX_Deleter {
  void operator()(EVP_MD_CTX *ptr) const { EVP_MD_CTX_free(ptr); }
};

std::string CryptoUtils::encrypt(const std::string &plaintext,
                                 const std::string &key) {
  try {
    // Derive key and generate IV
    auto derived_key = deriveKey(key);
    auto iv = generateIV();

    // Initialize encryption context
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    if (!ctx) {
      return "";
    }

    // Initialize encryption operation
    if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      return "";
    }

    // Prepare input data with padding
    auto padded_data = addPadding(
        std::vector<unsigned char>(plaintext.begin(), plaintext.end()));

    // Prepare output buffer
    std::vector<unsigned char> ciphertext(padded_data.size() +
                                          EVP_MAX_BLOCK_LENGTH);
    int out_len1 = 0, out_len2 = 0;

    // Perform encryption
    if (1 != EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &out_len1,
                               padded_data.data(), padded_data.size())) {
      return "";
    }

    if (1 != EVP_EncryptFinal_ex(ctx.get(), ciphertext.data() + out_len1,
                                 &out_len2)) {
      return "";
    }

    // Combine IV and ciphertext
    std::string result;
    result.reserve(iv.size() + out_len1 + out_len2);
    result.append(reinterpret_cast<char *>(iv.data()), iv.size());
    result.append(reinterpret_cast<char *>(ciphertext.data()),
                  out_len1 + out_len2);

    return result;
  } catch (...) {
    return "";
  }
}

std::string CryptoUtils::decrypt(const std::string &ciphertext,
                                 const std::string &key) {
  try {
    if (ciphertext.size() <= BLOCK_SIZE) {
      return "";
    }

    // Extract IV and derive key
    std::vector<unsigned char> iv(ciphertext.begin(),
                                  ciphertext.begin() + BLOCK_SIZE);
    auto derived_key = deriveKey(key);

    // Initialize decryption context
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

    if (!ctx) {
      return "";
    }

    // Initialize decryption operation
    if (1 != EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      return "";
    }

    // Prepare output buffer
    std::vector<unsigned char> plaintext(ciphertext.size() - BLOCK_SIZE);
    int out_len1 = 0, out_len2 = 0;

    // Perform decryption
    if (1 != EVP_DecryptUpdate(ctx.get(), plaintext.data(), &out_len1,
                               reinterpret_cast<const unsigned char *>(
                                   ciphertext.data() + BLOCK_SIZE),
                               ciphertext.size() - BLOCK_SIZE)) {
      return "";
    }

    if (1 != EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + out_len1,
                                 &out_len2)) {
      return "";
    }

    // Remove padding and convert to string
    auto unpadded_data = removePadding(std::vector<unsigned char>(
        plaintext.begin(), plaintext.begin() + out_len1 + out_len2));

    return std::string(unpadded_data.begin(), unpadded_data.end());
  } catch (...) {
    return "";
  }
}

std::vector<unsigned char> CryptoUtils::deriveKey(const std::string &key) {
  // Use EVP_Digest for key derivation
  std::vector<unsigned char> derived_key(KEY_SIZE);

  // Create message digest context
  std::unique_ptr<EVP_MD_CTX, EVP_MD_CTX_Deleter> ctx(EVP_MD_CTX_new());

  if (!ctx) {
    throw std::runtime_error("Failed to create digest context");
  }

  if (1 != EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr)) {
    throw std::runtime_error("Failed to initialize digest");
  }

  // Update with key data
  if (1 != EVP_DigestUpdate(ctx.get(), key.c_str(), key.length())) {
    throw std::runtime_error("Failed to update digest");
  }

  unsigned int digest_len;
  if (1 != EVP_DigestFinal_ex(ctx.get(), derived_key.data(), &digest_len)) {
    throw std::runtime_error("Failed to finalize digest");
  }

  return derived_key;
}

std::vector<unsigned char> CryptoUtils::generateIV() {
  std::vector<unsigned char> iv(BLOCK_SIZE);

  // Use RAND_bytes from OpenSSL for cryptographically secure random generation
  if (1 != RAND_bytes(iv.data(), BLOCK_SIZE)) {
    throw std::runtime_error("Failed to generate IV");
  }

  return iv;
}

std::vector<unsigned char>
CryptoUtils::addPadding(const std::vector<unsigned char> &data) {
  size_t padding_len = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
  std::vector<unsigned char> padded = data;
  padded.resize(data.size() + padding_len, padding_len);
  return padded;
}

std::vector<unsigned char>
CryptoUtils::removePadding(const std::vector<unsigned char> &data) {
  if (data.empty()) {
    return data;
  }

  unsigned char padding_len = data.back();
  if (padding_len > BLOCK_SIZE || padding_len > data.size()) {
    throw std::runtime_error("Invalid padding");
  }

  for (size_t i = data.size() - padding_len; i < data.size(); ++i) {
    if (data[i] != padding_len) {
      throw std::runtime_error("Invalid padding");
    }
  }

  return std::vector<unsigned char>(data.begin(), data.end() - padding_len);
}
