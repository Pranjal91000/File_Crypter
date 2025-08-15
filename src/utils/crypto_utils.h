#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace CryptoUtils {

    /**
     * @brief Exception class for cryptographic operations
     */
    class CryptoException : public std::runtime_error {
    public:
        explicit CryptoException(const std::string& message) : std::runtime_error(message) {}
    };

    /**
     * @brief Secure memory management for sensitive data
     */
    class SecureBuffer {
    private:
        std::vector<unsigned char> data_;
        
    public:
        SecureBuffer() = default;
        explicit SecureBuffer(size_t size);
        SecureBuffer(const std::vector<unsigned char>& data);
        SecureBuffer(const std::string& data);
        
        // Copy constructor and assignment
        SecureBuffer(const SecureBuffer& other);
        SecureBuffer& operator=(const SecureBuffer& other);
        
        // Move constructor and assignment
        SecureBuffer(SecureBuffer&& other) noexcept;
        SecureBuffer& operator=(SecureBuffer&& other) noexcept;
        
        // Destructor - securely clears memory
        ~SecureBuffer();
        
        // Access methods
        unsigned char* data();
        const unsigned char* data() const;
        size_t size() const;
        bool empty() const;
        
        // Secure operations
        void clear();
        void resize(size_t new_size);
        void append(const std::vector<unsigned char>& data);
        
        // Conversion methods
        std::string toString() const;
        std::vector<unsigned char> toVector() const;
    };

    /**
     * @brief Key management utilities
     */
    class KeyManager {
    public:
        /**
         * @brief Generate a random DES key
         * @return 64-bit key as hex string
         */
        static std::string generateDESKey();
        
        /**
         * @brief Validate DES key format
         * @param key Key to validate
         * @return true if valid, false otherwise
         */
        static bool validateDESKey(const std::string& key);
        
        /**
         * @brief Load key from file
         * @param keyFile Path to key file
         * @return Key as string
         * @throws CryptoException if file cannot be read or key is invalid
         */
        static std::string loadKeyFromFile(const std::string& keyFile);
        
        /**
         * @brief Save key to file
         * @param key Key to save
         * @param keyFile Path to key file
         * @throws CryptoException if file cannot be written
         */
        static void saveKeyToFile(const std::string& key, const std::string& keyFile);
    };

    /**
     * @brief Data conversion utilities
     */
    class DataConverter {
    public:
        /**
         * @brief Convert hex string to binary data
         * @param hex Hex string
         * @return Binary data
         */
        static std::vector<unsigned char> hexToBytes(const std::string& hex);
        
        /**
         * @brief Convert binary data to hex string
         * @param data Binary data
         * @return Hex string
         */
        static std::string bytesToHex(const std::vector<unsigned char>& data);
        
        /**
         * @brief Convert string to hex
         * @param str Input string
         * @return Hex string
         */
        static std::string stringToHex(const std::string& str);
        
        /**
         * @brief Convert hex string to regular string
         * @param hex Hex string
         * @return Regular string
         */
        static std::string hexToString(const std::string& hex);
        
        /**
         * @brief Validate hex string
         * @param hex String to validate
         * @return true if valid hex, false otherwise
         */
        static bool isValidHex(const std::string& hex);
    };

    /**
     * @brief Padding utilities for block ciphers
     */
    class Padding {
    public:
        /**
         * @brief PKCS7 padding
         * @param data Data to pad
         * @param blockSize Block size (default 8 for DES)
         * @return Padded data
         */
        static std::vector<unsigned char> pkcs7Pad(const std::vector<unsigned char>& data, size_t blockSize = 8);
        
        /**
         * @brief Remove PKCS7 padding
         * @param data Padded data
         * @return Unpadded data
         * @throws CryptoException if padding is invalid
         */
        static std::vector<unsigned char> pkcs7Unpad(const std::vector<unsigned char>& data);
        
        /**
         * @brief Zero padding
         * @param data Data to pad
         * @param blockSize Block size
         * @return Padded data
         */
        static std::vector<unsigned char> zeroPad(const std::vector<unsigned char>& data, size_t blockSize = 8);
        
        /**
         * @brief Remove zero padding
         * @param data Padded data
         * @return Unpadded data
         */
        static std::vector<unsigned char> zeroUnpad(const std::vector<unsigned char>& data);
    };

    /**
     * @brief Security utilities
     */
    class Security {
    public:
        /**
         * @brief Securely clear memory
         * @param ptr Pointer to memory
         * @param size Size of memory to clear
         */
        static void secureClear(void* ptr, size_t size);
        
        /**
         * @brief Generate random bytes
         * @param size Number of bytes to generate
         * @return Random bytes
         */
        static std::vector<unsigned char> randomBytes(size_t size);
        
        /**
         * @brief Constant time comparison
         * @param a First buffer
         * @param b Second buffer
         * @return true if equal, false otherwise
         */
        static bool constantTimeCompare(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b);
    };

} // namespace CryptoUtils

#endif // CRYPTO_UTILS_H
