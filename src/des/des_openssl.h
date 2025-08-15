#ifndef DES_OPENSSL_H
#define DES_OPENSSL_H

#include <string>
#include <vector>
#include <memory>
#include "../utils/crypto_utils.h"
#include "../utils/file_utils.h"

namespace DES {

    /**
     * @brief Exception class for DES operations
     */
    class DESException : public std::runtime_error {
    public:
        explicit DESException(const std::string& message) : std::runtime_error(message) {}
    };

    /**
     * @brief OpenSSL-based DES encryption class
     */
    class DESEncryptor {
    private:
        std::string key_;
        bool keySet_;

    public:
        DESEncryptor();
        explicit DESEncryptor(const std::string& key);
        ~DESEncryptor();

        /**
         * @brief Set the encryption key
         * @param key 64-bit key in hex format
         * @throws DESException if key is invalid
         */
        void setKey(const std::string& key);

        /**
         * @brief Load key from file
         * @param keyFile Path to key file
         * @throws DESException if key cannot be loaded
         */
        void loadKeyFromFile(const std::string& keyFile);

        /**
         * @brief Generate a random key
         * @return Generated key in hex format
         */
        std::string generateKey();

        /**
         * @brief Save current key to file
         * @param keyFile Path to key file
         * @throws DESException if key cannot be saved
         */
        void saveKeyToFile(const std::string& keyFile) const;

        /**
         * @brief Encrypt data
         * @param data Data to encrypt
         * @return Encrypted data
         * @throws DESException if encryption fails
         */
        CryptoUtils::SecureBuffer encrypt(const CryptoUtils::SecureBuffer& data);

        /**
         * @brief Encrypt data from vector
         * @param data Data to encrypt
         * @return Encrypted data
         * @throws DESException if encryption fails
         */
        CryptoUtils::SecureBuffer encrypt(const std::vector<unsigned char>& data);

        /**
         * @brief Encrypt string
         * @param data String to encrypt
         * @return Encrypted data
         * @throws DESException if encryption fails
         */
        CryptoUtils::SecureBuffer encrypt(const std::string& data);

        /**
         * @brief Encrypt file
         * @param inputFile Input file path
         * @param outputFile Output file path
         * @throws DESException if encryption fails
         */
        void encryptFile(const std::string& inputFile, const std::string& outputFile);

        /**
         * @brief Get current key
         * @return Current key in hex format
         * @throws DESException if no key is set
         */
        std::string getKey() const;

        /**
         * @brief Check if key is set
         * @return true if key is set, false otherwise
         */
        bool isKeySet() const;

    private:
        /**
         * @brief Validate key format
         * @param key Key to validate
         * @return true if valid, false otherwise
         */
        bool validateKey(const std::string& key) const;

        /**
         * @brief Convert hex key to bytes
         * @param keyHex Key in hex format
         * @return Key as bytes
         */
        std::vector<unsigned char> keyToBytes(const std::string& keyHex) const;
    };

    /**
     * @brief OpenSSL-based DES decryption class
     */
    class DESDecryptor {
    private:
        std::string key_;
        bool keySet_;

    public:
        DESDecryptor();
        explicit DESDecryptor(const std::string& key);
        ~DESDecryptor();

        /**
         * @brief Set the decryption key
         * @param key 64-bit key in hex format
         * @throws DESException if key is invalid
         */
        void setKey(const std::string& key);

        /**
         * @brief Load key from file
         * @param keyFile Path to key file
         * @throws DESException if key cannot be loaded
         */
        void loadKeyFromFile(const std::string& keyFile);

        /**
         * @brief Decrypt data
         * @param data Data to decrypt
         * @return Decrypted data
         * @throws DESException if decryption fails
         */
        CryptoUtils::SecureBuffer decrypt(const CryptoUtils::SecureBuffer& data);

        /**
         * @brief Decrypt data from vector
         * @param data Data to decrypt
         * @return Decrypted data
         * @throws DESException if decryption fails
         */
        CryptoUtils::SecureBuffer decrypt(const std::vector<unsigned char>& data);

        /**
         * @brief Decrypt file
         * @param inputFile Input file path
         * @param outputFile Output file path
         * @throws DESException if decryption fails
         */
        void decryptFile(const std::string& inputFile, const std::string& outputFile);

        /**
         * @brief Get current key
         * @return Current key in hex format
         * @throws DESException if no key is set
         */
        std::string getKey() const;

        /**
         * @brief Check if key is set
         * @return true if key is set, false otherwise
         */
        bool isKeySet() const;

    private:
        /**
         * @brief Validate key format
         * @param key Key to validate
         * @return true if valid, false otherwise
         */
        bool validateKey(const std::string& key) const;

        /**
         * @brief Convert hex key to bytes
         * @param keyHex Key in hex format
         * @return Key as bytes
         */
        std::vector<unsigned char> keyToBytes(const std::string& keyHex) const;
    };

    /**
     * @brief Utility functions for DES operations
     */
    namespace Utils {
        /**
         * @brief Generate a random DES key
         * @return 64-bit key in hex format
         */
        std::string generateKey();

        /**
         * @brief Validate DES key format
         * @param key Key to validate
         * @return true if valid, false otherwise
         */
        bool validateKey(const std::string& key);

        /**
         * @brief Test DES encryption/decryption
         * @param testData Test data
         * @return true if test passes, false otherwise
         */
        bool testEncryption(const std::string& testData = "Hello, DES encryption test!");
    };

} // namespace DES

#endif // DES_OPENSSL_H
