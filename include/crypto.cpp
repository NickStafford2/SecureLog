#include "crypto.h"
#include <cstring>    // For std::memcpy
#include <openssl/evp.h>
#include <openssl/rand.h> 
#include <stdexcept>

// Generates a random AES key of the specified length
std::vector<unsigned char> Crypto::generateKey() {
    std::vector<unsigned char> key(AES_KEY_LENGTH / 8);  // AES_KEY_LENGTH is in bits, so divide by 8 for bytes
    RAND_bytes(key.data(), key.size());  // Fill the key with random bytes
    return key;
}

// Encrypts plaintext using AES encryption (EVP)
std::vector<unsigned char> Crypto::encrypt(const std::string& plainText, const std::vector<unsigned char>& key) {
    std::vector<unsigned char> cipherText(plainText.size() + MY_AES_BLOCK_SIZE);  // Allocate space for ciphertext + IV

    // Create an EVP context for encryption
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Error creating EVP context for encryption");
    }

    // Generate a random IV
    unsigned char iv[MY_AES_BLOCK_SIZE];
    RAND_bytes(iv, MY_AES_BLOCK_SIZE); // Generate random IV

    // Initialize the encryption operation (AES-256-CBC)
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error initializing encryption operation");
    }

    // Perform encryption
    int len;
    if (1 != EVP_EncryptUpdate(ctx, cipherText.data() + MY_AES_BLOCK_SIZE, &len, (unsigned char*)plainText.c_str(), plainText.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error encrypting data");
    }

    // Finalize the encryption
    int cipherTextLen = len;
    if (1 != EVP_EncryptFinal_ex(ctx, cipherText.data() + MY_AES_BLOCK_SIZE + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error finalizing encryption");
    }
    cipherTextLen += len;

    // Prepend the IV to the cipherText for later use in decryption
    std::memcpy(cipherText.data(), iv, MY_AES_BLOCK_SIZE);

    EVP_CIPHER_CTX_free(ctx); // Free the context
    cipherText.resize(cipherTextLen + MY_AES_BLOCK_SIZE);  // Resize to the correct length
    return cipherText;
}

// Decrypts ciphertext using AES decryption (EVP)
std::string Crypto::decrypt(const std::vector<unsigned char>& cipherText, const std::vector<unsigned char>& key) {
    std::vector<unsigned char> plainText(cipherText.size() - MY_AES_BLOCK_SIZE);  // Allocate space for plaintext (without IV)

    // Create an EVP context for decryption
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Error creating EVP context for decryption");
    }

    // Extract the IV from the beginning of the cipherText
    unsigned char iv[MY_AES_BLOCK_SIZE];
    std::memcpy(iv, cipherText.data(), MY_AES_BLOCK_SIZE);

    // Initialize the decryption operation (AES-256-CBC)
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error initializing decryption operation");
    }

    // Perform decryption
    int len;
    if (1 != EVP_DecryptUpdate(ctx, plainText.data(), &len, cipherText.data() + MY_AES_BLOCK_SIZE, cipherText.size() - MY_AES_BLOCK_SIZE)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error decrypting data");
    }

    // Finalize the decryption
    int plainTextLen = len;
    if (1 != EVP_DecryptFinal_ex(ctx, plainText.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error finalizing decryption");
    }
    plainTextLen += len;

    EVP_CIPHER_CTX_free(ctx); // Free the context
    plainText.resize(plainTextLen);  // Resize to the correct length

    return std::string((char*)plainText.data());  // Convert the decrypted byte data back to a string
}

