#include "crypto.h"
#include <cstring>    // For std::memcpy
#include <openssl/aes.h>
#include <openssl/rand.h> 

// Generates a random AES key of the specified length
std::vector<unsigned char> Crypto::generateKey() {
    std::vector<unsigned char> key(AES_KEY_LENGTH / 8); // AES_KEY_LENGTH is in bits, so divide by 8 for bytes
    RAND_bytes(key.data(), key.size());  // Fill the key with random bytes
    return key;
}

// Encrypts plaintext using AES encryption
std::vector<unsigned char> Crypto::encrypt(const std::string& plainText, const std::vector<unsigned char>& key) {
    std::vector<unsigned char> cipherText(plainText.size() + MY_AES_BLOCK_SIZE);  // Allocate space for ciphertext + IV
    AES_KEY encKey;
    AES_set_encrypt_key(key.data(), AES_KEY_LENGTH, &encKey);  // Set encryption key

    unsigned char iv[MY_AES_BLOCK_SIZE];
    RAND_bytes(iv, MY_AES_BLOCK_SIZE); // Generate random IV

    // Prepend IV to the cipher text for later use
    std::memcpy(cipherText.data(), iv, MY_AES_BLOCK_SIZE);  // Copy IV to the beginning of cipherText
    AES_cbc_encrypt((unsigned char*)plainText.c_str(), cipherText.data() + MY_AES_BLOCK_SIZE,
                    plainText.size(), &encKey, iv, AES_ENCRYPT);  // Perform AES encryption in CBC mode

    return cipherText;  // Return the complete ciphertext (including IV)
}

// Decrypts ciphertext using AES decryption
std::string Crypto::decrypt(const std::vector<unsigned char>& cipherText, const std::vector<unsigned char>& key) {
    std::vector<unsigned char> plainText(cipherText.size() - MY_AES_BLOCK_SIZE);  // Allocate space for plaintext (without IV)
    AES_KEY decKey;
    AES_set_decrypt_key(key.data(), AES_KEY_LENGTH, &decKey);  // Set decryption key

    unsigned char iv[MY_AES_BLOCK_SIZE];
    std::memcpy(iv, cipherText.data(), MY_AES_BLOCK_SIZE);  // Extract IV from the beginning of ciphertext

    // Perform AES decryption in CBC mode (ignore the first block which contains the IV)
    AES_cbc_encrypt(cipherText.data() + MY_AES_BLOCK_SIZE, plainText.data(),
                    cipherText.size() - MY_AES_BLOCK_SIZE, &decKey, iv, AES_DECRYPT);

    return std::string((char*)plainText.data());  // Convert the decrypted byte data back to a string
}

