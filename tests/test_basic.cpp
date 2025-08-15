#include <iostream>
#include <string>
#include <cassert>
#include "des/des_openssl.h"
#include "huffman/huffman.h"
#include "utils/crypto_utils.h"

void testDESEncryption() {
    std::cout << "Testing DES encryption...\n";
    
    // Test key generation
    std::string key1 = DES::Utils::generateKey();
    std::string key2 = DES::Utils::generateKey();
    assert(key1 != key2); // Keys should be different
    assert(DES::Utils::validateKey(key1));
    assert(DES::Utils::validateKey(key2));
    
    // Test encryption/decryption
    std::string testData = "Hello, DES encryption test!";
    DES::DESEncryptor encryptor(key1);
    DES::DESDecryptor decryptor(key1);
    
    CryptoUtils::SecureBuffer encrypted = encryptor.encrypt(testData);
    CryptoUtils::SecureBuffer decrypted = decryptor.decrypt(encrypted);
    
    assert(decrypted.toString() == testData);
    std::cout << "DES encryption test: PASSED\n";
}

void testHuffmanCompression() {
    std::cout << "Testing Huffman compression...\n";
    
    std::string testData = "This is a test message for Huffman compression. It contains repeated characters like 'e', 's', 't' which should compress well.";
    
    Huffman::HuffmanCompressor compressor;
    compressor.buildTree(testData);
    
    CryptoUtils::SecureBuffer compressed = compressor.compress(testData);
    
    // Check that compression actually reduced size (for this test data)
    assert(compressed.size() < testData.size());
    
    double ratio = Huffman::HuffmanCompressor::getCompressionRatio(testData.size(), compressed.size());
    assert(ratio < 1.0); // Should be compressed
    
    std::cout << "Huffman compression test: PASSED\n";
}

void testCryptoUtils() {
    std::cout << "Testing Crypto utilities...\n";
    
    // Test hex conversion
    std::string hex = "48656C6C6F"; // "Hello" in hex
    std::vector<unsigned char> bytes = CryptoUtils::DataConverter::hexToBytes(hex);
    std::string backToHex = CryptoUtils::DataConverter::bytesToHex(bytes);
    assert(backToHex == hex);
    
    // Test padding
    std::vector<unsigned char> data = {0x01, 0x02, 0x03};
    std::vector<unsigned char> padded = CryptoUtils::Padding::pkcs7Pad(data, 8);
    assert(padded.size() == 8);
    
    std::vector<unsigned char> unpadded = CryptoUtils::Padding::pkcs7Unpad(padded);
    assert(unpadded == data);
    
    std::cout << "Crypto utilities test: PASSED\n";
}

void testFileUtils() {
    std::cout << "Testing File utilities...\n";
    
    // Test path validation
    assert(FileUtils::FileValidator::isValidPath("test.txt"));
    assert(!FileUtils::FileValidator::isValidPath("test<>.txt"));
    
    std::cout << "File utilities test: PASSED\n";
}

int main() {
    try {
        std::cout << "Running basic tests...\n\n";
        
        testDESEncryption();
        testHuffmanCompression();
        testCryptoUtils();
        testFileUtils();
        
        std::cout << "\nAll tests passed!\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
