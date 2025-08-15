#include "des_custom.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>

namespace DESCustom {

    // Static table definitions
    const int DESEncryptor::PC_1[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };

    const int DESEncryptor::PC_2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    const int DESEncryptor::IP_T[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    const int DESEncryptor::E_T[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
    };

    const int DESEncryptor::S[8][4][16] = {
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        {
            {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        {
            {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        {
            {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        {
            {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        {
            {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        {
            {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };

    const int DESEncryptor::P[32] = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
    };

    const int DESEncryptor::P_1[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    const int DESEncryptor::NUM_LEFTSHIFT[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    // Copy the same tables for decryptor (define them separately to avoid private access issues)
    const int DESDecryptor::PC_1[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };

    const int DESDecryptor::PC_2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    const int DESDecryptor::IP_T[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    const int DESDecryptor::E_T[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
    };

    const int DESDecryptor::S[8][4][16] = {
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        {
            {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        {
            {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        {
            {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        {
            {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        {
            {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        {
            {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };

    const int DESDecryptor::P[32] = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
    };

    const int DESDecryptor::P_1[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    const int DESDecryptor::NUM_LEFTSHIFT[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    // Helper functions
    std::string binToHex(const std::string& binary) {
        std::string hex = "";
        for (size_t i = 0; i < binary.size(); i += 4) {
            std::string k = binary.substr(i, 4);
            if (k == "0000") hex += '0';
            else if (k == "0001") hex += '1';
            else if (k == "0010") hex += '2';
            else if (k == "0011") hex += '3';
            else if (k == "0100") hex += '4';
            else if (k == "0101") hex += '5';
            else if (k == "0110") hex += '6';
            else if (k == "0111") hex += '7';
            else if (k == "1000") hex += '8';
            else if (k == "1001") hex += '9';
            else if (k == "1010") hex += 'A';
            else if (k == "1011") hex += 'B';
            else if (k == "1100") hex += 'C';
            else if (k == "1101") hex += 'D';
            else if (k == "1110") hex += 'E';
            else if (k == "1111") hex += 'F';
        }
        return hex;
    }

    std::string hexToBin(const std::string& hex) {
        std::string binary = "";
        for (char hexChar : hex) {
            int val = (hexChar >= '0' && hexChar <= '9') ? hexChar - '0' : (hexChar - 'A' + 10);
            for (int i = 3; i >= 0; --i) {
                binary += (val >> i) & 1 ? '1' : '0';
            }
        }
        return binary;
    }

    std::string decToBin(int n) {
        std::string bin = "";
        while (n > 0) {
            bin = (char)(n % 2 + '0') + bin;
            n /= 2;
        }
        while (bin.size() < 4) {
            bin = '0' + bin;
        }
        return bin;
    }

    std::string stringToHex(const std::string& input) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (unsigned char c : input) {
            ss << std::setw(2) << static_cast<int>(c);
        }
        return ss.str();
    }

    std::string hexToString(const std::string& hex) {
        std::string result;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            char byte = (char)strtol(byteString.c_str(), nullptr, 16);
            result += byte;
        }
        return result;
    }

    // DESEncryptor implementation
    DESEncryptor::DESEncryptor(const std::string& key) : key(key) {
        if (!validateKey(key)) {
            throw DESException("Invalid DES key format");
        }
        generateSubkeys();
    }

    void DESEncryptor::setKey(const std::string& newKey) {
        if (!validateKey(newKey)) {
            throw DESException("Invalid DES key format");
        }
        key = newKey;
        generateSubkeys();
    }

    void DESEncryptor::loadKeyFromFile(const std::string& filename) {
        try {
            CryptoUtils::SecureBuffer buffer = FileUtils::FileHandler::readFile(filename);
            std::string loadedKey = buffer.toString();
            setKey(loadedKey);
        } catch (const std::exception& e) {
            throw DESException("Failed to load key from file: " + std::string(e.what()));
        }
    }

    void DESEncryptor::generateKey() {
        key = generateRandomKey();
        generateSubkeys();
    }

    std::string DESEncryptor::getKey() const {
        return key;
    }

    std::string DESEncryptor::processKey(const std::string& keyHex) {
        std::string keyBin = hexToBin(keyHex);
        std::string processedKey = "";
        for (int i = 0; i < 56; ++i) {
            processedKey += keyBin[PC_1[i] - 1];
        }
        return processedKey;
    }

    std::string DESEncryptor::shiftBit(const std::string& s, int n) {
        std::string k = "";
        for (int i = n; i < static_cast<int>(s.size()); i++) {
            k += s[i];
        }
        for (int i = 0; i < n; i++) {
            k += s[i];
        }
        return k;
    }

    std::string DESEncryptor::xorAdd(const std::string& s1, const std::string& s2) {
        std::string result = "";
        for (size_t j = 0; j < s1.size(); j++) {
            if (s1[j] != s2[j]) result += '1';
            else result += '0';
        }
        return result;
    }

    std::string DESEncryptor::getElementFromBox(const std::string& s, int k) {
        int dec1 = 0, dec2 = 0, pwr = 0;
        dec1 = (int)(s[0] - '0') * 2 + (int)(s[5] - '0');
        for (int i = static_cast<int>(s.size()) - 2; i >= 1; i--) {
            dec2 += (int)(s[i] - '0') * static_cast<int>(pow(2, pwr++));
        }
        return decToBin(S[k][dec1][dec2]);
    }

    void DESEncryptor::generateSubkeys() {
        std::string processedKey = processKey(key);
        
        std::string keyFirstHalf = processedKey.substr(0, 28);
        std::string keySecondHalf = processedKey.substr(28, 28);

        std::vector<std::string> L_key(16), R_key(16);
        L_key[0] = shiftBit(keyFirstHalf, NUM_LEFTSHIFT[0]);
        R_key[0] = shiftBit(keySecondHalf, NUM_LEFTSHIFT[0]);

        for (int i = 1; i < 16; i++) {
            L_key[i] = shiftBit(L_key[i - 1], NUM_LEFTSHIFT[i]);
            R_key[i] = shiftBit(R_key[i - 1], NUM_LEFTSHIFT[i]);
        }

        subkeys.clear();
        for (int i = 0; i < 16; i++) {
            std::string key56 = L_key[i] + R_key[i];
            std::string key48 = "";
            for (int j = 0; j < 48; j++) {
                key48 += key56[PC_2[j] - 1];
            }
            subkeys.push_back(key48);
        }
    }

    std::string DESEncryptor::encrypt(const std::string& plaintext) {
        // Convert string to hex if it's not already hex
        std::string plaintextHex;
        bool isHex = true;
        for (char c : plaintext) {
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
                isHex = false;
                break;
            }
        }
        if (isHex && plaintext.length() % 2 == 0) {
            plaintextHex = plaintext;
        } else {
            plaintextHex = stringToHex(plaintext);
        }

        // Pad to 16 hex characters (64 bits)
        while (plaintextHex.length() % 16 != 0) {
            plaintextHex += '0';
        }

        std::string result = "";
        for (size_t i = 0; i < plaintextHex.length(); i += 16) {
            std::string block = plaintextHex.substr(i, 16);
            std::string blockBin = hexToBin(block);

            // Initial permutation
            std::string IP = "";
            for (int j = 0; j < 64; j++) {
                IP += blockBin[IP_T[j] - 1];
            }

            std::string L = IP.substr(0, 32);
            std::string R = IP.substr(32, 32);

            // 16 rounds
            for (int round = 0; round < 16; round++) {
                std::string oldL = L;
                L = R;
                
                // Expand R
                std::string R48 = "";
                for (int j = 0; j < 48; j++) {
                    R48 += R[E_T[j] - 1];
                }

                // XOR with subkey
                std::string R_xor_K = xorAdd(R48, subkeys[round]);

                // S-box substitution
                std::string s_output = "";
                for (int j = 0; j < 48; j += 6) {
                    std::string s_input = R_xor_K.substr(j, 6);
                    s_output += getElementFromBox(s_input, j / 6);
                }

                // P permutation
                std::string P_R = "";
                for (int j = 0; j < 32; j++) {
                    P_R += s_output[P[j] - 1];
                }

                // XOR with old L
                R = xorAdd(P_R, oldL);
            }

            // Final permutation
            std::string RL = R + L;
            std::string finalPerm = "";
            for (int j = 0; j < 64; j++) {
                finalPerm += RL[P_1[j] - 1];
            }

            result += binToHex(finalPerm);
        }

        return result;
    }

    std::string DESEncryptor::encryptFile(const std::string& inputFile, const std::string& outputFile) {
        try {
            CryptoUtils::SecureBuffer buffer = FileUtils::FileHandler::readFile(inputFile);
            std::string content = buffer.toString();
            std::string encrypted = encrypt(content);
            FileUtils::FileHandler::writeFile(outputFile, CryptoUtils::SecureBuffer(encrypted));
            return encrypted;
        } catch (const std::exception& e) {
            throw DESException("File encryption failed: " + std::string(e.what()));
        }
    }

    std::vector<uint8_t> DESEncryptor::encryptBytes(const std::vector<uint8_t>& data) {
        std::string dataStr(data.begin(), data.end());
        std::string encrypted = encrypt(dataStr);
        return std::vector<uint8_t>(encrypted.begin(), encrypted.end());
    }

    bool DESEncryptor::validateKey(const std::string& key) {
        if (key.length() != 16) return false;
        for (char c : key) {
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
                return false;
            }
        }
        return true;
    }

    std::string DESEncryptor::generateRandomKey() {
        // Simple random key generation (for demonstration)
        std::string key = "133457799BBCDFF1"; // Default key
        return key;
    }

    // DESDecryptor implementation
    DESDecryptor::DESDecryptor(const std::string& key) : key(key) {
        if (!validateKey(key)) {
            throw DESException("Invalid DES key format");
        }
        generateSubkeys();
    }

    void DESDecryptor::setKey(const std::string& newKey) {
        if (!validateKey(newKey)) {
            throw DESException("Invalid DES key format");
        }
        key = newKey;
        generateSubkeys();
    }

    void DESDecryptor::loadKeyFromFile(const std::string& filename) {
        try {
            CryptoUtils::SecureBuffer buffer = FileUtils::FileHandler::readFile(filename);
            std::string loadedKey = buffer.toString();
            setKey(loadedKey);
        } catch (const std::exception& e) {
            throw DESException("Failed to load key from file: " + std::string(e.what()));
        }
    }

    std::string DESDecryptor::getKey() const {
        return key;
    }

    void DESDecryptor::generateSubkeys() {
        std::string processedKey = processKey(key);
        
        std::string keyFirstHalf = processedKey.substr(0, 28);
        std::string keySecondHalf = processedKey.substr(28, 28);

        std::vector<std::string> L_key(16), R_key(16);
        L_key[0] = shiftBit(keyFirstHalf, NUM_LEFTSHIFT[0]);
        R_key[0] = shiftBit(keySecondHalf, NUM_LEFTSHIFT[0]);

        for (int i = 1; i < 16; i++) {
            L_key[i] = shiftBit(L_key[i - 1], NUM_LEFTSHIFT[i]);
            R_key[i] = shiftBit(R_key[i - 1], NUM_LEFTSHIFT[i]);
        }

        subkeys.clear();
        for (int i = 0; i < 16; i++) {
            std::string key56 = L_key[i] + R_key[i];
            std::string key48 = "";
            for (int j = 0; j < 48; j++) {
                key48 += key56[PC_2[j] - 1];
            }
            subkeys.push_back(key48);
        }
        
        // Reverse subkeys for decryption
        std::reverse(subkeys.begin(), subkeys.end());
    }

    std::string DESDecryptor::processKey(const std::string& keyHex) {
        std::string keyBin = hexToBin(keyHex);
        std::string processedKey = "";
        for (int i = 0; i < 56; ++i) {
            processedKey += keyBin[PC_1[i] - 1];
        }
        return processedKey;
    }

    std::string DESDecryptor::shiftBit(const std::string& s, int n) {
        std::string k = "";
        for (int i = n; i < static_cast<int>(s.size()); i++) {
            k += s[i];
        }
        for (int i = 0; i < n; i++) {
            k += s[i];
        }
        return k;
    }

    std::string DESDecryptor::xorAdd(const std::string& s1, const std::string& s2) {
        std::string result = "";
        for (size_t j = 0; j < s1.size(); j++) {
            if (s1[j] != s2[j]) result += '1';
            else result += '0';
        }
        return result;
    }

    std::string DESDecryptor::getElementFromBox(const std::string& s, int k) {
        int dec1 = 0, dec2 = 0, pwr = 0;
        dec1 = (int)(s[0] - '0') * 2 + (int)(s[5] - '0');
        for (int i = static_cast<int>(s.size()) - 2; i >= 1; i--) {
            dec2 += (int)(s[i] - '0') * static_cast<int>(pow(2, pwr++));
        }
        return decToBin(S[k][dec1][dec2]);
    }

    std::string DESDecryptor::decrypt(const std::string& ciphertext) {
        std::string result = "";
        for (size_t i = 0; i < ciphertext.length(); i += 16) {
            std::string block = ciphertext.substr(i, 16);
            std::string blockBin = hexToBin(block);

            // Initial permutation
            std::string IP = "";
            for (int j = 0; j < 64; j++) {
                IP += blockBin[IP_T[j] - 1];
            }

            std::string L = IP.substr(0, 32);
            std::string R = IP.substr(32, 32);

            // 16 rounds (same as encryption but with reversed subkeys)
            for (int round = 0; round < 16; round++) {
                std::string oldL = L;
                L = R;
                
                // Expand R
                std::string R48 = "";
                for (int j = 0; j < 48; j++) {
                    R48 += R[E_T[j] - 1];
                }

                // XOR with subkey
                std::string R_xor_K = xorAdd(R48, subkeys[round]);

                // S-box substitution
                std::string s_output = "";
                for (int j = 0; j < 48; j += 6) {
                    std::string s_input = R_xor_K.substr(j, 6);
                    s_output += getElementFromBox(s_input, j / 6);
                }

                // P permutation
                std::string P_R = "";
                for (int j = 0; j < 32; j++) {
                    P_R += s_output[P[j] - 1];
                }

                // XOR with old L
                R = xorAdd(P_R, oldL);
            }

            // Final permutation
            std::string RL = R + L;
            std::string finalPerm = "";
            for (int j = 0; j < 64; j++) {
                finalPerm += RL[P_1[j] - 1];
            }

            result += binToHex(finalPerm);
        }

        return result;
    }

    std::string DESDecryptor::decryptFile(const std::string& inputFile, const std::string& outputFile) {
        try {
            CryptoUtils::SecureBuffer buffer = FileUtils::FileHandler::readFile(inputFile);
            std::string content = buffer.toString();
            std::string decrypted = decrypt(content);
            FileUtils::FileHandler::writeFile(outputFile, CryptoUtils::SecureBuffer(decrypted));
            return decrypted;
        } catch (const std::exception& e) {
            throw DESException("File decryption failed: " + std::string(e.what()));
        }
    }

    std::vector<uint8_t> DESDecryptor::decryptBytes(const std::vector<uint8_t>& data) {
        std::string dataStr(data.begin(), data.end());
        std::string decrypted = decrypt(dataStr);
        return std::vector<uint8_t>(decrypted.begin(), decrypted.end());
    }

    bool DESDecryptor::validateKey(const std::string& key) {
        return DESEncryptor::validateKey(key);
    }

    // Utils namespace implementation
    namespace Utils {
        std::string generateKey() {
            return DESEncryptor::generateRandomKey();
        }

        bool testEncryptionDecryption() {
            try {
                std::string testKey = "133457799BBCDFF1";
                std::string testData = "Hello, World!";
                
                DESEncryptor encryptor(testKey);
                DESDecryptor decryptor(testKey);
                
                std::string encrypted = encryptor.encrypt(testData);
                std::string decrypted = decryptor.decrypt(encrypted);
                
                // Convert hex back to string for comparison
                std::string decryptedStr = hexToString(decrypted);
                
                // Remove padding
                while (!decryptedStr.empty() && decryptedStr.back() == '\0') {
                    decryptedStr.pop_back();
                }
                
                return decryptedStr == testData;
            } catch (const std::exception& e) {
                std::cerr << "Test failed: " << e.what() << std::endl;
                return false;
            }
        }

        void benchmarkPerformance() {
            std::string testKey = "133457799BBCDFF1";
            std::string testData = "This is a test message for benchmarking the custom DES implementation.";
            
            DESEncryptor encryptor(testKey);
            DESDecryptor decryptor(testKey);
            
            auto start = std::chrono::high_resolution_clock::now();
            
            for (int i = 0; i < 1000; i++) {
                std::string encrypted = encryptor.encrypt(testData);
                std::string decrypted = decryptor.decrypt(encrypted);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            std::cout << "Custom DES Performance: " << duration.count() << "ms for 1000 encrypt/decrypt cycles" << std::endl;
        }

        std::string getImplementationInfo() {
            return "Custom DES Implementation - Educational/Reference Implementation";
        }
    }

} // namespace DESCustom
