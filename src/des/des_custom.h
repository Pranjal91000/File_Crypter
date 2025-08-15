#ifndef DES_CUSTOM_H
#define DES_CUSTOM_H

#include <string>
#include <vector>
#include <memory>
#include "../utils/crypto_utils.h"
#include "../utils/file_utils.h"

namespace DESCustom {

    class DESException : public std::runtime_error {
    public:
        explicit DESException(const std::string& message) : std::runtime_error(message) {}
    };

    class DESEncryptor {
    private:
        std::string key;
        std::vector<std::string> subkeys;
        
        // DES permutation tables and S-boxes
        static const int PC_1[56];
        static const int PC_2[48];
        static const int IP_T[64];
        static const int E_T[48];
        static const int S[8][4][16];
        static const int P[32];
        static const int P_1[64];
        static const int NUM_LEFTSHIFT[16];

        void generateSubkeys();
        std::string processKey(const std::string& keyHex);
        std::string shiftBit(const std::string& s, int n);
        std::string xorAdd(const std::string& s1, const std::string& s2);
        std::string getElementFromBox(const std::string& s, int k);
        std::string expandR(const std::string& r32);

    public:
        explicit DESEncryptor(const std::string& key);
        
        // Key management
        void setKey(const std::string& newKey);
        void loadKeyFromFile(const std::string& filename);
        void generateKey();
        std::string getKey() const;
        
        // Encryption methods
        std::string encrypt(const std::string& plaintext);
        std::string encryptFile(const std::string& inputFile, const std::string& outputFile);
        std::vector<uint8_t> encryptBytes(const std::vector<uint8_t>& data);
        
        // Utility methods
        static bool validateKey(const std::string& key);
        static std::string generateRandomKey();
    };

    class DESDecryptor {
    private:
        std::string key;
        std::vector<std::string> subkeys;
        
        // Same tables as encryptor
        static const int PC_1[56];
        static const int PC_2[48];
        static const int IP_T[64];
        static const int E_T[48];
        static const int S[8][4][16];
        static const int P[32];
        static const int P_1[64];
        static const int NUM_LEFTSHIFT[16];

        void generateSubkeys();
        std::string processKey(const std::string& keyHex);
        std::string shiftBit(const std::string& s, int n);
        std::string xorAdd(const std::string& s1, const std::string& s2);
        std::string getElementFromBox(const std::string& s, int k);
        std::string expandR(const std::string& r32);

    public:
        explicit DESDecryptor(const std::string& key);
        
        // Key management
        void setKey(const std::string& newKey);
        void loadKeyFromFile(const std::string& filename);
        std::string getKey() const;
        
        // Decryption methods
        std::string decrypt(const std::string& ciphertext);
        std::string decryptFile(const std::string& inputFile, const std::string& outputFile);
        std::vector<uint8_t> decryptBytes(const std::vector<uint8_t>& data);
        
        // Utility methods
        static bool validateKey(const std::string& key);
    };

    namespace Utils {
        // General DES utilities
        std::string generateKey();
        bool testEncryptionDecryption();
        void benchmarkPerformance();
        std::string getImplementationInfo();
    }

} // namespace DESCustom

#endif // DES_CUSTOM_H
