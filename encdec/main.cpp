#include "DES/des.hpp"
#include "Huffman/huffman.hpp"
#include <iostream>
#include <string>

int main() {
    // Input and output file paths
    std::string keyFile = "key.txt";
    std::string inputFile = "input.txt";
    std::string encryptedFile = "encrypted_output.bin";
    std::string decryptedFile = "decrypted_output.txt";

    // Encrypt the input file
    std::cout << "Starting encryption process..." << std::endl;
    encryptDES(keyFile, inputFile, encryptedFile);
    std::cout << "Encryption completed. Encrypted data written to: " << encryptedFile << std::endl;

    // Compress the file
    std::string compressedFile = "compressed.bin";
    std::cout << "Compressing file..." << std::endl;
    compressFile(inputFile, compressedFile);

    // Decompress the file
    std::string decompressedFile = "decompressed.txt";
    std::cout << "Decompressing file..." << std::endl;
    decompressFile(compressedFile, decompressedFile);

    // Decrypt the encrypted file
    std::cout << "Starting decryption process..." << std::endl;
    decryptDES(keyFile, encryptedFile, decryptedFile);
    std::cout << "Decryption completed. Decrypted data written to: " << decryptedFile << std::endl;

    return 0;
}

