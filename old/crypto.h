#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>
#include <string>

class Crypto {
public:
    static const int AES_KEY_LENGTH = 256; // AES key length
    static const int AES_BLOCK_SIZE = 16;   // AES block size

    static std::vector<unsigned char> generateKey();
    static std::vector<unsigned char> encrypt(const std::string& plainText, const std::vector<unsigned char>& key);
    static std::string decrypt(const std::vector<unsigned char>& cipherText, const std::vector<unsigned char>& key);
};

#endif // CRYPTO_H
