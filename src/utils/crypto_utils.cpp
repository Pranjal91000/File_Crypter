#include "crypto_utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <random>
#include <openssl/rand.h>
#include <openssl/evp.h>

namespace CryptoUtils {

    // SecureBuffer implementation
    SecureBuffer::SecureBuffer(size_t size) : data_(size) {}

    SecureBuffer::SecureBuffer(const std::vector<unsigned char>& data) : data_(data) {}

    SecureBuffer::SecureBuffer(const std::string& data) : data_(data.begin(), data.end()) {}

    SecureBuffer::SecureBuffer(const SecureBuffer& other) : data_(other.data_) {}

    SecureBuffer& SecureBuffer::operator=(const SecureBuffer& other) {
        if (this != &other) {
            data_ = other.data_;
        }
        return *this;
    }

    SecureBuffer::SecureBuffer(SecureBuffer&& other) noexcept : data_(std::move(other.data_)) {}

    SecureBuffer& SecureBuffer::operator=(SecureBuffer&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
        }
        return *this;
    }

    SecureBuffer::~SecureBuffer() {
        clear();
    }

    unsigned char* SecureBuffer::data() {
        return data_.data();
    }

    const unsigned char* SecureBuffer::data() const {
        return data_.data();
    }

    size_t SecureBuffer::size() const {
        return data_.size();
    }

    bool SecureBuffer::empty() const {
        return data_.empty();
    }

    void SecureBuffer::clear() {
        if (!data_.empty()) {
            Security::secureClear(data_.data(), data_.size());
            data_.clear();
        }
    }

    void SecureBuffer::resize(size_t new_size) {
        data_.resize(new_size);
    }

    void SecureBuffer::append(const std::vector<unsigned char>& data) {
        data_.insert(data_.end(), data.begin(), data.end());
    }

    std::string SecureBuffer::toString() const {
        return std::string(data_.begin(), data_.end());
    }

    std::vector<unsigned char> SecureBuffer::toVector() const {
        return data_;
    }

    // KeyManager implementation
    std::string KeyManager::generateDESKey() {
        std::vector<unsigned char> keyBytes = Security::randomBytes(8);
        return DataConverter::bytesToHex(keyBytes);
    }

    bool KeyManager::validateDESKey(const std::string& key) {
        if (key.length() != 16) { // 64 bits = 16 hex characters
            return false;
        }
        return DataConverter::isValidHex(key);
    }

    std::string KeyManager::loadKeyFromFile(const std::string& keyFile) {
        std::ifstream file(keyFile);
        if (!file.is_open()) {
            throw CryptoException("Cannot open key file: " + keyFile);
        }

        std::string key;
        std::getline(file, key);
        file.close();

        // Remove whitespace and newlines
        key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

        if (!validateDESKey(key)) {
            throw CryptoException("Invalid DES key format in file: " + keyFile);
        }

        return key;
    }

    void KeyManager::saveKeyToFile(const std::string& key, const std::string& keyFile) {
        if (!validateDESKey(key)) {
            throw CryptoException("Invalid DES key format");
        }

        std::ofstream file(keyFile);
        if (!file.is_open()) {
            throw CryptoException("Cannot create key file: " + keyFile);
        }

        file << key << std::endl;
        file.close();
    }

    // DataConverter implementation
    std::vector<unsigned char> DataConverter::hexToBytes(const std::string& hex) {
        if (hex.length() % 2 != 0) {
            throw CryptoException("Hex string must have even length");
        }

        std::vector<unsigned char> bytes;
        bytes.reserve(hex.length() / 2);

        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }

        return bytes;
    }

    std::string DataConverter::bytesToHex(const std::vector<unsigned char>& data) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        
        for (unsigned char byte : data) {
            ss << std::setw(2) << static_cast<int>(byte);
        }
        
        return ss.str();
    }

    std::string DataConverter::stringToHex(const std::string& str) {
        return bytesToHex(std::vector<unsigned char>(str.begin(), str.end()));
    }

    std::string DataConverter::hexToString(const std::string& hex) {
        std::vector<unsigned char> bytes = hexToBytes(hex);
        return std::string(bytes.begin(), bytes.end());
    }

    bool DataConverter::isValidHex(const std::string& hex) {
        if (hex.empty()) {
            return false;
        }

        for (char c : hex) {
            if (!std::isxdigit(c)) {
                return false;
            }
        }
        return true;
    }

    // Padding implementation
    std::vector<unsigned char> Padding::pkcs7Pad(const std::vector<unsigned char>& data, size_t blockSize) {
        if (blockSize == 0 || blockSize > 255) {
            throw CryptoException("Invalid block size for PKCS7 padding");
        }

        size_t paddingLength = blockSize - (data.size() % blockSize);
        std::vector<unsigned char> padded = data;
        padded.insert(padded.end(), paddingLength, static_cast<unsigned char>(paddingLength));
        return padded;
    }

    std::vector<unsigned char> Padding::pkcs7Unpad(const std::vector<unsigned char>& data) {
        if (data.empty()) {
            throw CryptoException("Cannot unpad empty data");
        }

        unsigned char paddingLength = data.back();
        if (paddingLength == 0 || paddingLength > data.size()) {
            throw CryptoException("Invalid PKCS7 padding");
        }

        // Verify padding
        for (size_t i = data.size() - paddingLength; i < data.size(); ++i) {
            if (data[i] != paddingLength) {
                throw CryptoException("Invalid PKCS7 padding");
            }
        }

        return std::vector<unsigned char>(data.begin(), data.end() - paddingLength);
    }

    std::vector<unsigned char> Padding::zeroPad(const std::vector<unsigned char>& data, size_t blockSize) {
        if (blockSize == 0) {
            throw CryptoException("Invalid block size for zero padding");
        }

        size_t paddingLength = blockSize - (data.size() % blockSize);
        std::vector<unsigned char> padded = data;
        padded.insert(padded.end(), paddingLength, 0);
        return padded;
    }

    std::vector<unsigned char> Padding::zeroUnpad(const std::vector<unsigned char>& data) {
        if (data.empty()) {
            return data;
        }

        size_t end = data.size();
        while (end > 0 && data[end - 1] == 0) {
            --end;
        }

        return std::vector<unsigned char>(data.begin(), data.begin() + end);
    }

    // Security implementation
    void Security::secureClear(void* ptr, size_t size) {
        if (ptr && size > 0) {
            volatile unsigned char* p = static_cast<volatile unsigned char*>(ptr);
            for (size_t i = 0; i < size; ++i) {
                p[i] = 0;
            }
        }
    }

    std::vector<unsigned char> Security::randomBytes(size_t size) {
        std::vector<unsigned char> bytes(size);
        
        // Use std::random for random bytes generation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        for (size_t i = 0; i < size; ++i) {
            bytes[i] = static_cast<unsigned char>(dis(gen));
        }
        
        return bytes;
    }

    bool Security::constantTimeCompare(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b) {
        if (a.size() != b.size()) {
            return false;
        }

        unsigned char result = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            result |= a[i] ^ b[i];
        }

        return result == 0;
    }

} // namespace CryptoUtils
