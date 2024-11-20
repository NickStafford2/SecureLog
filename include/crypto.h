#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>  // Include EVP functions
#include <vector>
#include <string>

class Crypto {
public:
    static const int AES_KEY_LENGTH = 256; // AES key length (in bits)
    static const int MY_AES_BLOCK_SIZE = 16; // AES block size (in bytes)

    static std::vector<unsigned char> generateKey();
    static std::vector<unsigned char> encrypt(const std::string& plainText, const std::vector<unsigned char>& key);
    static std::string decrypt(const std::vector<unsigned char>& cipherText, const std::vector<unsigned char>& key);
};

#endif // CRYPTO_H

