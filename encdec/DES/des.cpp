#include "des.hpp"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// Helper function to read a binary file into a vector of bytes
vector<uint8_t> readFile(const string& filePath) {
    ifstream file(filePath, ios::binary);
    if (!file) {
        throw runtime_error("Could not open file: " + filePath);
    }
    return vector<uint8_t>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

// Helper function to write a vector of bytes to a binary file
void writeFile(const string& filePath, const vector<uint8_t>& data) {
    ofstream file(filePath, ios::binary);
    if (!file) {
        throw runtime_error("Could not open file: " + filePath);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// Placeholder DES encryption function
vector<uint8_t> DES_encrypt(const vector<uint8_t>& data, const vector<uint8_t>& key) {
    // Simple XOR encryption for demonstration purposes
    vector<uint8_t> encrypted(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        encrypted[i] = data[i] ^ key[i % key.size()];
    }
    return encrypted;
}

// Placeholder DES decryption function
vector<uint8_t> DES_decrypt(const vector<uint8_t>& data, const vector<uint8_t>& key) {
    // XOR decryption (symmetric to encryption)
    vector<uint8_t> decrypted(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        decrypted[i] = data[i] ^ key[i % key.size()];
    }
    return decrypted;
}

void encryptDES(const string& keyFile, const string& inputFile, const string& outputFile) {
    try {
        // Read the key and input file
        vector<uint8_t> key = readFile(keyFile);
        vector<uint8_t> input = readFile(inputFile);

        // Perform encryption
        vector<uint8_t> encryptedData = DES_encrypt(input, key);

        // Write the encrypted data to the output file
        writeFile(outputFile, encryptedData);
    } catch (const exception& e) {
        cerr << "Error during encryption: " << e.what() << endl;
    }
}

void decryptDES(const string& keyFile, const string& inputFile, const string& outputFile) {
    try {
        // Read the key and input file
        vector<uint8_t> key = readFile(keyFile);
        vector<uint8_t> encryptedData = readFile(inputFile);

        // Perform decryption
        vector<uint8_t> decryptedData = DES_decrypt(encryptedData, key);

        // Write the decrypted data to the output file
        writeFile(outputFile, decryptedData);
    } catch (const exception& e) {
        cerr << "Error during decryption: " << e.what() << endl;
    }
}
