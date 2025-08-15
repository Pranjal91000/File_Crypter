#include <iostream>
#include <chrono>
#include <string>
#include "../src/des/des_openssl.h"
#include "../src/des/des_custom.h"
#include "../src/utils/crypto_utils.h"
#include "../src/utils/file_utils.h"

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

void benchmarkDES(const std::string& testData, const std::string& key, int iterations = 1000) {
    printHeader("DES Implementation Comparison");
    
    std::cout << "Test data size: " << testData.length() << " bytes" << std::endl;
    std::cout << "Key: " << key << std::endl;
    std::cout << "Iterations: " << iterations << std::endl;
    
    // Test OpenSSL DES
    std::cout << "\n--- OpenSSL-based DES ---" << std::endl;
    try {
        DES::DESEncryptor opensslEncryptor;
        DES::DESDecryptor opensslDecryptor;
        opensslEncryptor.setKey(key);
        opensslDecryptor.setKey(key);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            std::string encrypted = opensslEncryptor.encrypt(testData);
            std::string decrypted = opensslDecryptor.decrypt(encrypted);
            
            if (i == 0) {
                std::cout << "First iteration - Original: " << testData.substr(0, 20) << "..." << std::endl;
                std::cout << "First iteration - Decrypted: " << decrypted.substr(0, 20) << "..." << std::endl;
                std::cout << "Encryption/Decryption successful: " << (decrypted == testData ? "YES" : "NO") << std::endl;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "OpenSSL DES Performance: " << duration.count() << "ms for " << iterations << " cycles" << std::endl;
        std::cout << "Average time per cycle: " << (duration.count() / (double)iterations) << "ms" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "OpenSSL DES Error: " << e.what() << std::endl;
    }
    
    // Test Custom DES
    std::cout << "\n--- Custom DES ---" << std::endl;
    try {
        DESCustom::DESEncryptor customEncryptor;
        DESCustom::DESDecryptor customDecryptor;
        customEncryptor.setKey(key);
        customDecryptor.setKey(key);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            std::string encrypted = customEncryptor.encrypt(testData);
            std::string decrypted = customDecryptor.decrypt(encrypted);
            
            if (i == 0) {
                std::cout << "First iteration - Original: " << testData.substr(0, 20) << "..." << std::endl;
                std::cout << "First iteration - Decrypted: " << decrypted.substr(0, 20) << "..." << std::endl;
                std::cout << "Encryption/Decryption successful: " << (decrypted == testData ? "YES" : "NO") << std::endl;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "Custom DES Performance: " << duration.count() << "ms for " << iterations << " cycles" << std::endl;
        std::cout << "Average time per cycle: " << (duration.count() / (double)iterations) << "ms" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Custom DES Error: " << e.what() << std::endl;
    }
}

void testFileOperations() {
    printHeader("File Operation Tests");
    
    std::string testKey = "133457799BBCDFF1";
    std::string testData = "This is a test file for DES encryption comparison.\n";
    testData += "It contains multiple lines to test the encryption process.\n";
    testData += "The goal is to verify that both implementations work correctly.\n";
    
    std::string inputFile = "test_input.txt";
    std::string opensslOutput = "test_openssl_encrypted.bin";
    std::string customOutput = "test_custom_encrypted.bin";
    std::string opensslDecrypted = "test_openssl_decrypted.txt";
    std::string customDecrypted = "test_custom_decrypted.txt";
    
    // Create test file
    try {
        FileUtils::FileHandler::writeFile(inputFile, testData);
        std::cout << "Created test file: " << inputFile << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating test file: " << e.what() << std::endl;
        return;
    }
    
    // Test OpenSSL DES file operations
    std::cout << "\n--- OpenSSL DES File Operations ---" << std::endl;
    try {
        DES::DESEncryptor opensslEncryptor;
        DES::DESDecryptor opensslDecryptor;
        opensslEncryptor.setKey(testKey);
        opensslDecryptor.setKey(testKey);
        
        opensslEncryptor.encryptFile(inputFile, opensslOutput);
        std::cout << "OpenSSL encryption completed: " << opensslOutput << std::endl;
        
        opensslDecryptor.decryptFile(opensslOutput, opensslDecrypted);
        std::cout << "OpenSSL decryption completed: " << opensslDecrypted << std::endl;
        
        std::string decryptedContent = FileUtils::FileHandler::readFile(opensslDecrypted);
        std::cout << "OpenSSL file test successful: " << (decryptedContent == testData ? "YES" : "NO") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "OpenSSL file operation error: " << e.what() << std::endl;
    }
    
    // Test Custom DES file operations
    std::cout << "\n--- Custom DES File Operations ---" << std::endl;
    try {
        DESCustom::DESEncryptor customEncryptor;
        DESCustom::DESDecryptor customDecryptor;
        customEncryptor.setKey(testKey);
        customDecryptor.setKey(testKey);
        
        customEncryptor.encryptFile(inputFile, customOutput);
        std::cout << "Custom encryption completed: " << customOutput << std::endl;
        
        customDecryptor.decryptFile(customOutput, customDecrypted);
        std::cout << "Custom decryption completed: " << customDecrypted << std::endl;
        
        std::string decryptedContent = FileUtils::FileHandler::readFile(customDecrypted);
        std::cout << "Custom file test successful: " << (decryptedContent == testData ? "YES" : "NO") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Custom file operation error: " << e.what() << std::endl;
    }
    
    // Clean up test files
    try {
        FileUtils::FileHandler::deleteFile(inputFile);
        FileUtils::FileHandler::deleteFile(opensslOutput);
        FileUtils::FileHandler::deleteFile(customOutput);
        FileUtils::FileHandler::deleteFile(opensslDecrypted);
        FileUtils::FileHandler::deleteFile(customDecrypted);
        std::cout << "\nTest files cleaned up." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error cleaning up test files: " << e.what() << std::endl;
    }
}

void testKeyGeneration() {
    printHeader("Key Generation Tests");
    
    std::cout << "--- OpenSSL Key Generation ---" << std::endl;
    try {
        std::string opensslKey = DES::Utils::generateKey();
        std::cout << "Generated OpenSSL key: " << opensslKey << std::endl;
        std::cout << "Key length: " << opensslKey.length() << " characters" << std::endl;
        std::cout << "Valid key format: " << (DES::DESEncryptor::validateKey(opensslKey) ? "YES" : "NO") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "OpenSSL key generation error: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Custom Key Generation ---" << std::endl;
    try {
        std::string customKey = DESCustom::Utils::generateKey();
        std::cout << "Generated custom key: " << customKey << std::endl;
        std::cout << "Key length: " << customKey.length() << " characters" << std::endl;
        std::cout << "Valid key format: " << (DESCustom::DESEncryptor::validateKey(customKey) ? "YES" : "NO") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Custom key generation error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "DES Implementation Comparison Tool" << std::endl;
    std::cout << "==================================" << std::endl;
    
    std::string testKey = "133457799BBCDFF1";
    std::string testData = "Hello, this is a test message for DES comparison!";
    
    // Run tests
    benchmarkDES(testData, testKey, 100); // Reduced iterations for faster testing
    testFileOperations();
    testKeyGeneration();
    
    printHeader("Summary");
    std::cout << "Comparison completed successfully!" << std::endl;
    std::cout << "Use OpenSSL-based DES for production environments." << std::endl;
    std::cout << "Use Custom DES for educational and research purposes." << std::endl;
    
    return 0;
}
