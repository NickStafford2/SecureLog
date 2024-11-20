// crypto.h
#ifndef CRYPTO_H
#define CRYPTO_H
#include <fstream>
#include <iostream>
#include <string>

// bool fileExistsAndIsReadable2(const std::string &filename);
// #endif
// crypto.h
// #ifndef CRYPTO_H
// #define CRYPTO_H

#pragma once

#include <string>
#include <vector>

class CryptoUtils {
public:
  // Encrypts the given plaintext using the provided key
  // Returns empty string on error
  static std::string encrypt(const std::string &plaintext,
                             const std::string &key);

  // Decrypts the given ciphertext using the provided key
  // Returns empty string on error
  static std::string decrypt(const std::string &ciphertext,
                             const std::string &key);

private:
  static const unsigned int KEY_SIZE = 32;   // 256 bits
  static const unsigned int BLOCK_SIZE = 16; // 128 bits

  // Derives a 256-bit key from the provided key string
  static std::vector<unsigned char> deriveKey(const std::string &key);

  // Generates a random IV for CBC mode
  static std::vector<unsigned char> generateIV();

  // Adds PKCS7 padding to the input data
  static std::vector<unsigned char>
  addPadding(const std::vector<unsigned char> &data);

  // Removes PKCS7 padding from the input data
  static std::vector<unsigned char>
  removePadding(const std::vector<unsigned char> &data);
};

#endif
