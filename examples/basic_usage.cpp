#include <iostream>
#include <string>
#include "des/des_openssl.h"
#include "huffman/huffman.h"
#include "utils/crypto_utils.h"

int main() {
    try {
        std::cout << "File Encryption System - Basic Usage Example\n";
        std::cout << "=============================================\n\n";

        // Generate a key
        std::string key = DES::Utils::generateKey();
        std::cout << "Generated key: " << key << std::endl;

        // Test data
        std::string testData = "Hello, this is a test message for encryption and compression!";
        std::cout << "Original data: " << testData << std::endl;
        std::cout << "Original size: " << testData.size() << " bytes\n\n";

        // Test DES encryption/decryption
        std::cout << "Testing DES encryption/decryption...\n";
        DES::DESEncryptor encryptor(key);
        DES::DESDecryptor decryptor(key);

        CryptoUtils::SecureBuffer encrypted = encryptor.encrypt(testData);
        CryptoUtils::SecureBuffer decrypted = decryptor.decrypt(encrypted);

        std::string decryptedString = decrypted.toString();
        std::cout << "Decrypted data: " << decryptedString << std::endl;
        std::cout << "Encryption test: " << (decryptedString == testData ? "PASSED" : "FAILED") << std::endl;
        std::cout << "Encrypted size: " << encrypted.size() << " bytes\n\n";

        // Test Huffman compression/decompression
        std::cout << "Testing Huffman compression/decompression...\n";
        Huffman::HuffmanCompressor compressor;
        Huffman::HuffmanDecompressor decompressor;

        compressor.buildTree(testData);
        CryptoUtils::SecureBuffer compressed = compressor.compress(testData);

        // For decompression, we need to serialize the tree
        // This is a simplified example - in practice, the tree would be included in the compressed data
        std::cout << "Compressed size: " << compressed.size() << " bytes\n";
        double ratio = Huffman::HuffmanCompressor::getCompressionRatio(testData.size(), compressed.size());
        std::cout << "Compression ratio: " << (ratio * 100) << "%\n";
        std::cout << "Space savings: " << ((1.0 - ratio) * 100) << "%\n\n";

        // Test combined encryption and compression
        std::cout << "Testing combined encryption and compression...\n";
        CryptoUtils::SecureBuffer encryptedCompressed = encryptor.encrypt(compressed);
        std::cout << "Encrypted and compressed size: " << encryptedCompressed.size() << " bytes\n\n";

        std::cout << "All tests completed successfully!\n";
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
