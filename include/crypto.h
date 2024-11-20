#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>
#include <string>

class Crypto {
public:
    static const int AES_KEY_LENGTH = 256; // AES key length (in bits)
    static const int MY_AES_BLOCK_SIZE = 16; // AES block size (in bytes)

    // Generates a random AES key of the specified length
    static std::vector<unsigned char> generateKey();

    // Encrypts plaintext using AES encryption
    static std::vector<unsigned char> encrypt(const std::string& plainText, const std::vector<unsigned char>& key);

    // Decrypts ciphertext using AES decryption
    static std::string decrypt(const std::vector<unsigned char>& cipherText, const std::vector<unsigned char>& key);
};

#endif // CRYPTO_H

