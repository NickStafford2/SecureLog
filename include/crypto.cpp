#include "crypto.h"

std::vector<unsigned char> Crypto::generateKey() {
    std::vector<unsigned char> key(AES_KEY_LENGTH / 8);
    RAND_bytes(key.data(), key.size());
    return key;
}

std::vector<unsigned char> Crypto::encrypt(const std::string& plainText, const std::vector<unsigned char>& key) {
    std::vector<unsigned char> cipherText(plainText.size() + AES_BLOCK_SIZE);
    AES_KEY encKey;
    AES_set_encrypt_key(key.data(), AES_KEY_LENGTH, &encKey);

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE); // Generate random IV

    // Prepend IV to the cipher text for later use
    std::memcpy(cipherText.data(), iv, AES_BLOCK_SIZE);
    AES_cbc_encrypt((unsigned char*)plainText.c_str(), cipherText.data() + AES_BLOCK_SIZE,
                    plainText.size(), &encKey, iv, AES_ENCRYPT);

    return cipherText;
}

std::string Crypto::decrypt(const std::vector<unsigned char>& cipherText, const std::vector<unsigned char>& key) {
    std::vector<unsigned char> plainText(cipherText.size());
    AES_KEY decKey;
    AES_set_decrypt_key(key.data(), AES_KEY_LENGTH, &decKey);

    unsigned char iv[AES_BLOCK_SIZE];
    std::memcpy(iv, cipherText.data(), AES_BLOCK_SIZE); // Extract IV from cipher text

    AES_cbc_encrypt(cipherText.data() + AES_BLOCK_SIZE, plainText.data(),
                    cipherText.size() - AES_BLOCK_SIZE, &decKey, iv, AES_DECRYPT);
    
    return std::string((char*)plainText.data());
}
