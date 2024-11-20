// // inputValidation.cpp
// #include <fstream>
// #include <iostream>
// #include <string>
//
// #include "crypto.h"
// // Function definition for isNumeric
// bool isNumeric(const std::string &str) {
//   for (char c : str) {
//     if (!std::isdigit(c))
//       return false;
//   }
//   return true;
// }
//
// bool fileExistsAndIsReadable2(const std::string &filename) {
//   std::ifstream file(filename); // Try to open the file for reading
//
//   // Check if the file was opened successfully
//   if (file.is_open()) {
//     file.close(); // Close the file as we only needed to check for existence
//     and
//                   // readability
//     return true;
//   }
//   return false;
// }
// #include <fstream>
// #include <iostream>
// #include <string>
//
// #include "crypto.h"
// // Function definition for isNumeric
//
// bool fileExistsAndIsReadable2(const std::string &filename) {
//   std::ifstream file(filename); // Try to open the file for reading
//
//   // Check if the file was opened successfully
//   if (file.is_open()) {
//     file.close(); // Close the file as we only needed to check for existence
//     and
//                   // readability
//     return true;
//   }
//   return false;
// }
//
// crypto.cpp

#include <memory>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "crypto.h"

void CryptoUtils::print() { int x = 0; }

std::string CryptoUtils::encrypt(const std::string &plaintext,
                                 const std::string &key) {
  try {
    // Derive key and generate IV
    auto derived_key = deriveKey(key);
    auto iv = generateIV();

    // Initialize encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
      return "";
    }

    // Initialize encryption operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      EVP_CIPHER_CTX_free(ctx);
      return "";
    }

    // Prepare input data with padding
    auto padded_data = addPadding(
        std::vector<unsigned char>(plaintext.begin(), plaintext.end()));

    // Prepare output buffer
    std::vector<unsigned char> ciphertext(padded_data.size() +
                                          EVP_MAX_BLOCK_LENGTH);
    int out_len1 = 0;

    // Perform encryption
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len1,
                               padded_data.data(), padded_data.size())) {
      EVP_CIPHER_CTX_free(ctx);
      return "";
    }

    int out_len2 = 0;
    if (1 !=
        EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len1, &out_len2)) {
      EVP_CIPHER_CTX_free(ctx);
      return "";
    }

    EVP_CIPHER_CTX_free(ctx);

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
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
      return "";
    }

    // Initialize decryption operation
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                derived_key.data(), iv.data())) {
      EVP_CIPHER_CTX_free(ctx);
      return "";
    }

    // Prepare output buffer
    std::vector<unsigned char> plaintext(ciphertext.size() - BLOCK_SIZE);
    int out_len1 = 0;

    // Perform decryption
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &out_len1,
                               reinterpret_cast<const unsigned char *>(
                                   ciphertext.data() + BLOCK_SIZE),
                               ciphertext.size() - BLOCK_SIZE)) {
      EVP_CIPHER_CTX_free(ctx);
      return "";
    }

    int out_len2 = 0;
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + out_len1, &out_len2)) {
      EVP_CIPHER_CTX_free(ctx);
      return "";
    }

    EVP_CIPHER_CTX_free(ctx);

    // Remove padding and convert to string
    auto unpadded_data = removePadding(std::vector<unsigned char>(
        plaintext.begin(), plaintext.begin() + out_len1 + out_len2));

    return std::string(unpadded_data.begin(), unpadded_data.end());
  } catch (...) {
    return "";
  }
}

std::vector<unsigned char> CryptoUtils::deriveKey(const std::string &key) {
  std::vector<unsigned char> derived_key(KEY_SIZE);
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, key.c_str(), key.length());
  SHA256_Final(derived_key.data(), &sha256);
  return derived_key;
}

std::vector<unsigned char> CryptoUtils::generateIV() {
  std::vector<unsigned char> iv(BLOCK_SIZE);
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
