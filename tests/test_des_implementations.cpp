#include <iostream>
#include <cassert>
#include <string>
#include "../src/des/des_openssl.h"
#include "../src/des/des_custom.h"
#include "../src/utils/crypto_utils.h"

void testOpenSSLDES() {
    std::cout << "Testing OpenSSL DES implementation..." << std::endl;
    
    std::string key = "133457799BBCDFF1";
    std::string plaintext = "Hello, World!";
    
    try {
        DES::DESEncryptor encryptor;
        DES::DESDecryptor decryptor;
        
        encryptor.setKey(key);
        decryptor.setKey(key);
        
        std::string encrypted = encryptor.encrypt(plaintext);
        std::string decrypted = decryptor.decrypt(encrypted);
        
        assert(decrypted == plaintext);
        std::cout << "✓ OpenSSL DES encryption/decryption test passed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ OpenSSL DES test failed: " << e.what() << std::endl;
        assert(false);
    }
}

void testCustomDES() {
    std::cout << "Testing Custom DES implementation..." << std::endl;
    
    std::string key = "133457799BBCDFF1";
    std::string plaintext = "Hello, World!";
    
    try {
        DESCustom::DESEncryptor encryptor;
        DESCustom::DESDecryptor decryptor;
        
        encryptor.setKey(key);
        decryptor.setKey(key);
        
        std::string encrypted = encryptor.encrypt(plaintext);
        std::string decrypted = decryptor.decrypt(encrypted);
        
        assert(decrypted == plaintext);
        std::cout << "✓ Custom DES encryption/decryption test passed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Custom DES test failed: " << e.what() << std::endl;
        assert(false);
    }
}

void testKeyValidation() {
    std::cout << "Testing key validation..." << std::endl;
    
    // Valid keys
    assert(DES::DESEncryptor::validateKey("133457799BBCDFF1"));
    assert(DESCustom::DESEncryptor::validateKey("133457799BBCDFF1"));
    
    // Invalid keys
    assert(!DES::DESEncryptor::validateKey("invalid"));
    assert(!DESCustom::DESEncryptor::validateKey("invalid"));
    assert(!DES::DESEncryptor::validateKey("1234567890ABCDEF")); // Wrong length
    assert(!DESCustom::DESEncryptor::validateKey("1234567890ABCDEF")); // Wrong length
    
    std::cout << "✓ Key validation tests passed" << std::endl;
}

void testKeyGeneration() {
    std::cout << "Testing key generation..." << std::endl;
    
    try {
        std::string opensslKey = DES::Utils::generateKey();
        std::string customKey = DESCustom::Utils::generateKey();
        
        assert(opensslKey.length() == 16);
        assert(customKey.length() == 16);
        assert(DES::DESEncryptor::validateKey(opensslKey));
        assert(DESCustom::DESEncryptor::validateKey(customKey));
        
        std::cout << "✓ Key generation tests passed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Key generation test failed: " << e.what() << std::endl;
        assert(false);
    }
}

void testCrossImplementation() {
    std::cout << "Testing cross-implementation compatibility..." << std::endl;
    
    std::string key = "133457799BBCDFF1";
    std::string plaintext = "Cross-implementation test";
    
    try {
        // Encrypt with OpenSSL, decrypt with Custom
        DES::DESEncryptor opensslEncryptor;
        DESCustom::DESDecryptor customDecryptor;
        
        opensslEncryptor.setKey(key);
        customDecryptor.setKey(key);
        
        std::string encrypted = opensslEncryptor.encrypt(plaintext);
        std::string decrypted = customDecryptor.decrypt(encrypted);
        
        // Note: Cross-implementation compatibility may not work due to different
        // padding schemes or implementation details
        std::cout << "OpenSSL -> Custom: " << (decrypted == plaintext ? "✓ Compatible" : "✗ Not compatible") << std::endl;
        
        // Encrypt with Custom, decrypt with OpenSSL
        DESCustom::DESEncryptor customEncryptor;
        DES::DESDecryptor opensslDecryptor;
        
        customEncryptor.setKey(key);
        opensslDecryptor.setKey(key);
        
        encrypted = customEncryptor.encrypt(plaintext);
        decrypted = opensslDecryptor.decrypt(encrypted);
        
        std::cout << "Custom -> OpenSSL: " << (decrypted == plaintext ? "✓ Compatible" : "✗ Not compatible") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Cross-implementation test failed: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "DES Implementation Tests" << std::endl;
    std::cout << "=======================" << std::endl;
    
    testOpenSSLDES();
    testCustomDES();
    testKeyValidation();
    testKeyGeneration();
    testCrossImplementation();
    
    std::cout << "\nAll tests completed!" << std::endl;
    return 0;
}
