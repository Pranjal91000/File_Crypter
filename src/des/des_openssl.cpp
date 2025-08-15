#include "des_openssl.h"
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <cstring>

namespace DES {

    // DESEncryptor implementation
    DESEncryptor::DESEncryptor() : keySet_(false) {}

    DESEncryptor::DESEncryptor(const std::string& key) : keySet_(false) {
        setKey(key);
    }

    DESEncryptor::~DESEncryptor() {
        // Securely clear the key
        if (!key_.empty()) {
            CryptoUtils::Security::secureClear(&key_[0], key_.size());
        }
    }

    void DESEncryptor::setKey(const std::string& key) {
        if (!validateKey(key)) {
            throw DESException("Invalid DES key format");
        }
        key_ = key;
        keySet_ = true;
    }

    void DESEncryptor::loadKeyFromFile(const std::string& keyFile) {
        try {
            std::string key = CryptoUtils::KeyManager::loadKeyFromFile(keyFile);
            setKey(key);
        } catch (const CryptoUtils::CryptoException& e) {
            throw DESException("Failed to load key: " + std::string(e.what()));
        }
    }

    std::string DESEncryptor::generateKey() {
        std::string key = CryptoUtils::KeyManager::generateDESKey();
        setKey(key);
        return key;
    }

    void DESEncryptor::saveKeyToFile(const std::string& keyFile) const {
        if (!keySet_) {
            throw DESException("No key set to save");
        }
        try {
            CryptoUtils::KeyManager::saveKeyToFile(key_, keyFile);
        } catch (const CryptoUtils::CryptoException& e) {
            throw DESException("Failed to save key: " + std::string(e.what()));
        }
    }

    CryptoUtils::SecureBuffer DESEncryptor::encrypt(const CryptoUtils::SecureBuffer& data) {
        if (!keySet_) {
            throw DESException("No encryption key set");
        }

        std::vector<unsigned char> dataVec = data.toVector();
        return encrypt(dataVec);
    }

    CryptoUtils::SecureBuffer DESEncryptor::encrypt(const std::vector<unsigned char>& data) {
        if (!keySet_) {
            throw DESException("No encryption key set");
        }

        // Convert key to bytes
        std::vector<unsigned char> keyBytes = keyToBytes(key_);

        // Pad data to 8-byte blocks
        std::vector<unsigned char> paddedData = CryptoUtils::Padding::pkcs7Pad(data, 8);

        // Initialize OpenSSL
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw DESException("Failed to create OpenSSL cipher context");
        }

        // Initialize encryption
        if (EVP_EncryptInit_ex(ctx, EVP_des_ecb(), nullptr, keyBytes.data(), nullptr) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw DESException("Failed to initialize DES encryption");
        }

        // Encrypt data
        std::vector<unsigned char> encrypted(paddedData.size() + EVP_MAX_BLOCK_LENGTH);
        int outLen = 0;
        int finalLen = 0;

        if (EVP_EncryptUpdate(ctx, encrypted.data(), &outLen, paddedData.data(), static_cast<int>(paddedData.size())) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw DESException("Failed to encrypt data");
        }

        if (EVP_EncryptFinal_ex(ctx, encrypted.data() + outLen, &finalLen) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw DESException("Failed to finalize encryption");
        }

        EVP_CIPHER_CTX_free(ctx);

        // Resize to actual encrypted size
        encrypted.resize(outLen + finalLen);

        return CryptoUtils::SecureBuffer(encrypted);
    }

    CryptoUtils::SecureBuffer DESEncryptor::encrypt(const std::string& data) {
        std::vector<unsigned char> dataVec(data.begin(), data.end());
        return encrypt(dataVec);
    }

    void DESEncryptor::encryptFile(const std::string& inputFile, const std::string& outputFile) {
        try {
            // Read input file
            CryptoUtils::SecureBuffer inputData = FileUtils::FileHandler::readFile(inputFile);
            
            // Encrypt data
            CryptoUtils::SecureBuffer encryptedData = encrypt(inputData);
            
            // Write encrypted data
            FileUtils::FileHandler::writeFile(outputFile, encryptedData);
        } catch (const FileUtils::FileException& e) {
            throw DESException("File operation failed: " + std::string(e.what()));
        }
    }

    std::string DESEncryptor::getKey() const {
        if (!keySet_) {
            throw DESException("No key set");
        }
        return key_;
    }

    bool DESEncryptor::isKeySet() const {
        return keySet_;
    }

    bool DESEncryptor::validateKey(const std::string& key) const {
        return CryptoUtils::KeyManager::validateDESKey(key);
    }

    std::vector<unsigned char> DESEncryptor::keyToBytes(const std::string& keyHex) const {
        return CryptoUtils::DataConverter::hexToBytes(keyHex);
    }

    // DESDecryptor implementation
    DESDecryptor::DESDecryptor() : keySet_(false) {}

    DESDecryptor::DESDecryptor(const std::string& key) : keySet_(false) {
        setKey(key);
    }

    DESDecryptor::~DESDecryptor() {
        // Securely clear the key
        if (!key_.empty()) {
            CryptoUtils::Security::secureClear(&key_[0], key_.size());
        }
    }

    void DESDecryptor::setKey(const std::string& key) {
        if (!validateKey(key)) {
            throw DESException("Invalid DES key format");
        }
        key_ = key;
        keySet_ = true;
    }

    void DESDecryptor::loadKeyFromFile(const std::string& keyFile) {
        try {
            std::string key = CryptoUtils::KeyManager::loadKeyFromFile(keyFile);
            setKey(key);
        } catch (const CryptoUtils::CryptoException& e) {
            throw DESException("Failed to load key: " + std::string(e.what()));
        }
    }

    CryptoUtils::SecureBuffer DESDecryptor::decrypt(const CryptoUtils::SecureBuffer& data) {
        if (!keySet_) {
            throw DESException("No decryption key set");
        }

        std::vector<unsigned char> dataVec = data.toVector();
        return decrypt(dataVec);
    }

    CryptoUtils::SecureBuffer DESDecryptor::decrypt(const std::vector<unsigned char>& data) {
        if (!keySet_) {
            throw DESException("No decryption key set");
        }

        if (data.size() % 8 != 0) {
            throw DESException("Encrypted data size must be multiple of 8 bytes");
        }

        // Convert key to bytes
        std::vector<unsigned char> keyBytes = keyToBytes(key_);

        // Initialize OpenSSL
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw DESException("Failed to create OpenSSL cipher context");
        }

        // Initialize decryption
        if (EVP_DecryptInit_ex(ctx, EVP_des_ecb(), nullptr, keyBytes.data(), nullptr) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw DESException("Failed to initialize DES decryption");
        }

        // Decrypt data
        std::vector<unsigned char> decrypted(data.size());
        int outLen = 0;
        int finalLen = 0;

        if (EVP_DecryptUpdate(ctx, decrypted.data(), &outLen, data.data(), static_cast<int>(data.size())) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw DESException("Failed to decrypt data");
        }

        if (EVP_DecryptFinal_ex(ctx, decrypted.data() + outLen, &finalLen) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw DESException("Failed to finalize decryption");
        }

        EVP_CIPHER_CTX_free(ctx);

        // Resize to actual decrypted size
        decrypted.resize(outLen + finalLen);

        // Remove padding
        try {
            std::vector<unsigned char> unpaddedData = CryptoUtils::Padding::pkcs7Unpad(decrypted);
            return CryptoUtils::SecureBuffer(unpaddedData);
        } catch (const CryptoUtils::CryptoException& e) {
            throw DESException("Failed to remove padding: " + std::string(e.what()));
        }
    }

    void DESDecryptor::decryptFile(const std::string& inputFile, const std::string& outputFile) {
        try {
            // Read encrypted file
            CryptoUtils::SecureBuffer encryptedData = FileUtils::FileHandler::readFile(inputFile);
            
            // Decrypt data
            CryptoUtils::SecureBuffer decryptedData = decrypt(encryptedData);
            
            // Write decrypted data
            FileUtils::FileHandler::writeFile(outputFile, decryptedData);
        } catch (const FileUtils::FileException& e) {
            throw DESException("File operation failed: " + std::string(e.what()));
        }
    }

    std::string DESDecryptor::getKey() const {
        if (!keySet_) {
            throw DESException("No key set");
        }
        return key_;
    }

    bool DESDecryptor::isKeySet() const {
        return keySet_;
    }

    bool DESDecryptor::validateKey(const std::string& key) const {
        return CryptoUtils::KeyManager::validateDESKey(key);
    }

    std::vector<unsigned char> DESDecryptor::keyToBytes(const std::string& keyHex) const {
        return CryptoUtils::DataConverter::hexToBytes(keyHex);
    }

    // Utils implementation
    namespace Utils {
        std::string generateKey() {
            return CryptoUtils::KeyManager::generateDESKey();
        }

        bool validateKey(const std::string& key) {
            return CryptoUtils::KeyManager::validateDESKey(key);
        }

        bool testEncryption(const std::string& testData) {
            try {
                // Generate a key
                std::string key = generateKey();
                
                // Create encryptor and decryptor
                DESEncryptor encryptor(key);
                DESDecryptor decryptor(key);
                
                // Encrypt test data
                CryptoUtils::SecureBuffer encrypted = encryptor.encrypt(testData);
                
                // Decrypt test data
                CryptoUtils::SecureBuffer decrypted = decryptor.decrypt(encrypted);
                
                // Compare results
                std::string decryptedString = decrypted.toString();
                return decryptedString == testData;
            } catch (...) {
                return false;
            }
        }
    }

} // namespace DES
