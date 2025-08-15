#include <openssl/des.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

void encryptDES(const std::string &keyFile, const std::string &inputFile, const std::string &outputFile) {
    // Read the key
    std::ifstream keyStream(keyFile);
    if (!keyStream.is_open()) {
        std::cerr << "Error: Unable to open key file." << std::endl;
        return;
    }
    std::string keyString;
    std::getline(keyStream, keyString);
    keyStream.close();

    if (keyString.size() < 8) {
        std::cerr << "Error: Key must be at least 8 bytes long." << std::endl;
        return;
    }

    // Prepare DES key
    DES_cblock key;
    DES_key_schedule schedule;
    std::memcpy(key, keyString.data(), 8);
    DES_set_key_unchecked(&key, &schedule);

    // Read plaintext
    std::ifstream inputStream(inputFile, std::ios::binary);
    if (!inputStream.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return;
    }
    std::vector<unsigned char> plaintext((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
    inputStream.close();

    // Pad plaintext to be a multiple of 8 bytes
    while (plaintext.size() % 8 != 0) {
        plaintext.push_back(0);
    }

    // Encrypt plaintext
    std::vector<unsigned char> ciphertext(plaintext.size());
    for (size_t i = 0; i < plaintext.size(); i += 8) {
        DES_ecb_encrypt(reinterpret_cast<const_DES_cblock*>(&plaintext[i]),
                        reinterpret_cast<DES_cblock*>(&ciphertext[i]),
                        &schedule, DES_ENCRYPT);
    }

    // Write ciphertext to output file
    std::ofstream outputStream(outputFile, std::ios::binary);
    if (!outputStream.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }
    outputStream.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());
    outputStream.close();
}

void decryptDES(const std::string &keyFile, const std::string &inputFile, const std::string &outputFile) {
    // Read the key
    std::ifstream keyStream(keyFile);
    if (!keyStream.is_open()) {
        std::cerr << "Error: Unable to open key file." << std::endl;
        return;
    }
    std::string keyString;
    std::getline(keyStream, keyString);
    keyStream.close();

    if (keyString.size() < 8) {
        std::cerr << "Error: Key must be at least 8 bytes long." << std::endl;
        return;
    }

    // Prepare DES key
    DES_cblock key;
    DES_key_schedule schedule;
    std::memcpy(key, keyString.data(), 8);
    DES_set_key_unchecked(&key, &schedule);

    // Read ciphertext
    std::ifstream inputStream(inputFile, std::ios::binary);
    if (!inputStream.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return;
    }
    std::vector<unsigned char> ciphertext((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
    inputStream.close();

    // Decrypt ciphertext
    std::vector<unsigned char> plaintext(ciphertext.size());
    for (size_t i = 0; i < ciphertext.size(); i += 8) {
        DES_ecb_encrypt(reinterpret_cast<const_DES_cblock*>(&ciphertext[i]),
                        reinterpret_cast<DES_cblock*>(&plaintext[i]),
                        &schedule, DES_DECRYPT);
    }

    // Write plaintext to output file
    std::ofstream outputStream(outputFile, std::ios::binary);
    if (!outputStream.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }
    outputStream.write(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
    outputStream.close();
}

int main() {
    std::string keyFile = "key.txt";
    std::string inputFile = "input.txt";
    std::string encryptedFile = "encrypted.bin";
    std::string decryptedFile = "decrypted.txt";

    std::cout << "Encrypting..." << std::endl;
    encryptDES(keyFile, inputFile, encryptedFile);

    std::cout << "Decrypting..." << std::endl;
    decryptDES(keyFile, encryptedFile, decryptedFile);

    std::cout << "Done. Check encrypted.bin and decrypted.txt." << std::endl;
    return 0;
}
