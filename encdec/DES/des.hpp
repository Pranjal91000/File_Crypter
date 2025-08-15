#ifndef DES_ENCRYPTION_H
#define DES_ENCRYPTION_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using std::string;    // Adding using directives for convenience
using std::vector;

// Function declarations
vector<uint8_t> readFile(const string& filePath);
void writeFile(const string& filePath, const vector<uint8_t>& data);
vector<uint8_t> DES_encrypt(const vector<uint8_t>& data, const vector<uint8_t>& key);
vector<uint8_t> DES_decrypt(const vector<uint8_t>& data, const vector<uint8_t>& key);

// Encrypt a file using DES
void encryptDES(const string &keyFile, const string &inputFile, const string &outputFile);

// Decrypt a file using DES
void decryptDES(const string &keyFile, const string &inputFile, const string &outputFile);

#endif // DES_ENCRYPTION_H
