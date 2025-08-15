#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>
#include "des/des_openssl.h"
#include "des/des_custom.h"
#include "huffman/huffman.h"
#include "utils/crypto_utils.h"
#include "utils/file_utils.h"

// Version information
const std::string VERSION = "1.2.0";
const std::string BUILD_DATE = __DATE__ " " __TIME__;

// Command-line argument parser
class ArgumentParser {
private:
    std::vector<std::string> args_;
    std::unordered_map<std::string, std::string> options_;
    std::vector<std::string> positional_;

public:
    ArgumentParser(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            args_.push_back(argv[i]);
        }
        parse();
    }

    void parse() {
        for (size_t i = 0; i < args_.size(); ++i) {
            std::string arg = args_[i];
            
            if (arg.substr(0, 2) == "--") {
                std::string key = arg.substr(2);
                if (i + 1 < args_.size() && args_[i + 1].substr(0, 2) != "--") {
                    options_[key] = args_[i + 1];
                    ++i; // Skip next argument
                } else {
                    options_[key] = "true";
                }
            } else if (arg.substr(0, 1) == "-") {
                std::string key = arg.substr(1);
                if (i + 1 < args_.size() && args_[i + 1].substr(0, 1) != "-") {
                    options_[key] = args_[i + 1];
                    ++i; // Skip next argument
                } else {
                    options_[key] = "true";
                }
            } else {
                positional_.push_back(arg);
            }
        }
    }

    bool hasOption(const std::string& key) const {
        return options_.find(key) != options_.end();
    }

    std::string getOption(const std::string& key, const std::string& defaultValue = "") const {
        auto it = options_.find(key);
        return (it != options_.end()) ? it->second : defaultValue;
    }

    const std::vector<std::string>& getPositional() const {
        return positional_;
    }
};

// Application class
class FileEncryptoApp {
private:
    bool verbose_;
    bool debug_;

public:
    FileEncryptoApp() : verbose_(false), debug_(false) {}

    void setVerbose(bool verbose) { verbose_ = verbose; }
    void setDebug(bool debug) { debug_ = debug; }

    void printUsage() const {
        std::cout << "File Encryption System v" << VERSION << "\n\n";
        std::cout << "Usage: file_encrypto [OPTIONS] COMMAND\n\n";
        std::cout << "Commands:\n";
        std::cout << "  --encrypt              Encrypt a file\n";
        std::cout << "  --decrypt              Decrypt a file\n";
        std::cout << "  --compress             Compress a file\n";
        std::cout << "  --decompress           Decompress a file\n\n";
        std::cout << "Options:\n";
        std::cout << "  --input <file>         Input file path\n";
        std::cout << "  --output <file>        Output file path\n";
        std::cout << "  --key <file>           Key file path (for encryption/decryption)\n";
        std::cout << "  --algorithm <des>      Encryption algorithm (default: des)\n";
        std::cout << "  --des-type <type>      DES implementation type: openssl (default) or custom\n";
        std::cout << "  --verbose              Enable verbose output\n";
        std::cout << "  --debug                Enable debug output\n";
        std::cout << "  --help                 Show this help message\n";
        std::cout << "  --version              Show version information\n\n";
        std::cout << "Examples:\n";
        std::cout << "  file_encrypto --encrypt --input secret.txt --output secret.enc --key key.txt\n";
        std::cout << "  file_encrypto --decrypt --input secret.enc --output decrypted.txt --key key.txt\n";
        std::cout << "  file_encrypto --compress --input large.txt --output compressed.bin\n";
        std::cout << "  file_encrypto --decompress --input compressed.bin --output decompressed.txt\n";
    }

    void printVersion() const {
        std::cout << "File Encryption System v" << VERSION << "\n";
        std::cout << "Build date: " << BUILD_DATE << "\n";
        std::cout << "Features: DES encryption, Huffman compression\n";
    }

    int run(const ArgumentParser& parser) {
        try {
            // Handle help and version
            if (parser.hasOption("help")) {
                printUsage();
                return 0;
            }

            if (parser.hasOption("version")) {
                printVersion();
                return 0;
            }

            // Set verbosity
            verbose_ = parser.hasOption("verbose");
            debug_ = parser.hasOption("debug");

            if (debug_) {
                verbose_ = true;
            }

            // Validate required arguments
            std::string inputFile = parser.getOption("input");
            std::string outputFile = parser.getOption("output");

            if (inputFile.empty() || outputFile.empty()) {
                std::cerr << "Error: --input and --output are required\n\n";
                printUsage();
                return 1;
            }

            // Validate file paths
            if (!FileUtils::FileValidator::isValidPath(inputFile)) {
                std::cerr << "Error: Invalid input file path: " << inputFile << std::endl;
                return 1;
            }

            if (!FileUtils::FileValidator::isValidPath(outputFile)) {
                std::cerr << "Error: Invalid output file path: " << outputFile << std::endl;
                return 1;
            }

            // Check if input file exists
            if (!FileUtils::FileHandler::fileExists(inputFile)) {
                std::cerr << "Error: Input file does not exist: " << inputFile << std::endl;
                return 1;
            }

            // Handle encryption
            if (parser.hasOption("encrypt")) {
                return handleEncryption(parser);
            }

            // Handle decryption
            if (parser.hasOption("decrypt")) {
                return handleDecryption(parser);
            }

            // Handle compression
            if (parser.hasOption("compress")) {
                return handleCompression(parser);
            }

            // Handle decompression
            if (parser.hasOption("decompress")) {
                return handleDecompression(parser);
            }

            std::cerr << "Error: No operation specified. Use --help for usage information.\n";
            return 1;

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            if (debug_) {
                std::cerr << "Stack trace: " << std::endl;
                // Add stack trace if available
            }
            return 1;
        }
    }

private:
    int handleEncryption(const ArgumentParser& parser) {
        std::string inputFile = parser.getOption("input");
        std::string outputFile = parser.getOption("output");
        std::string keyFile = parser.getOption("key");

        if (keyFile.empty()) {
            std::cerr << "Error: --key is required for encryption\n";
            return 1;
        }

        if (verbose_) {
            std::cout << "Starting encryption...\n";
            std::cout << "Input file: " << inputFile << std::endl;
            std::cout << "Output file: " << outputFile << std::endl;
            std::cout << "Key file: " << keyFile << std::endl;
        }

        std::string desType = parser.getOption("des-type", "openssl");
        
        if (verbose_) {
            std::cout << "Using DES implementation: " << desType << std::endl;
        }

        try {
            if (desType == "custom") {
                CryptoUtils::SecureBuffer buffer = FileUtils::FileHandler::readFile(keyFile);
                std::string key = buffer.toString();
                DESCustom::DESEncryptor encryptor(key);
                encryptor.encryptFile(inputFile, outputFile);
            } else {
                DES::DESEncryptor encryptor;
                encryptor.loadKeyFromFile(keyFile);
                encryptor.encryptFile(inputFile, outputFile);
            }

            if (verbose_) {
                size_t inputSize = FileUtils::FileHandler::getFileSize(inputFile);
                size_t outputSize = FileUtils::FileHandler::getFileSize(outputFile);
                std::cout << "Encryption completed successfully!\n";
                std::cout << "Input size: " << inputSize << " bytes\n";
                std::cout << "Output size: " << outputSize << " bytes\n";
            }

            return 0;
        } catch (const DES::DESException& e) {
            std::cerr << "Encryption error: " << e.what() << std::endl;
            return 1;
        } catch (const DESCustom::DESException& e) {
            std::cerr << "Encryption error: " << e.what() << std::endl;
            return 1;
        }
    }

    int handleDecryption(const ArgumentParser& parser) {
        std::string inputFile = parser.getOption("input");
        std::string outputFile = parser.getOption("output");
        std::string keyFile = parser.getOption("key");

        if (keyFile.empty()) {
            std::cerr << "Error: --key is required for decryption\n";
            return 1;
        }

        if (verbose_) {
            std::cout << "Starting decryption...\n";
            std::cout << "Input file: " << inputFile << std::endl;
            std::cout << "Output file: " << outputFile << std::endl;
            std::cout << "Key file: " << keyFile << std::endl;
        }

        std::string desType = parser.getOption("des-type", "openssl");
        
        if (verbose_) {
            std::cout << "Using DES implementation: " << desType << std::endl;
        }

        try {
            if (desType == "custom") {
                CryptoUtils::SecureBuffer buffer = FileUtils::FileHandler::readFile(keyFile);
                std::string key = buffer.toString();
                DESCustom::DESDecryptor decryptor(key);
                decryptor.decryptFile(inputFile, outputFile);
            } else {
                DES::DESDecryptor decryptor;
                decryptor.loadKeyFromFile(keyFile);
                decryptor.decryptFile(inputFile, outputFile);
            }

            if (verbose_) {
                size_t inputSize = FileUtils::FileHandler::getFileSize(inputFile);
                size_t outputSize = FileUtils::FileHandler::getFileSize(outputFile);
                std::cout << "Decryption completed successfully!\n";
                std::cout << "Input size: " << inputSize << " bytes\n";
                std::cout << "Output size: " << outputSize << " bytes\n";
            }

            return 0;
        } catch (const DES::DESException& e) {
            std::cerr << "Decryption error: " << e.what() << std::endl;
            return 1;
        } catch (const DESCustom::DESException& e) {
            std::cerr << "Decryption error: " << e.what() << std::endl;
            return 1;
        }
    }

    int handleCompression(const ArgumentParser& parser) {
        std::string inputFile = parser.getOption("input");
        std::string outputFile = parser.getOption("output");

        if (verbose_) {
            std::cout << "Starting compression...\n";
            std::cout << "Input file: " << inputFile << std::endl;
            std::cout << "Output file: " << outputFile << std::endl;
        }

        try {
            Huffman::HuffmanCompressor compressor;
            
            // Read input file
            CryptoUtils::SecureBuffer inputData = FileUtils::FileHandler::readFile(inputFile);
            
            // Build tree and compress
            compressor.buildTree(inputData.toVector());
            CryptoUtils::SecureBuffer compressedData = compressor.compress(inputData.toVector());
            
            // Write compressed data
            FileUtils::FileHandler::writeFile(outputFile, compressedData);

            if (verbose_) {
                size_t inputSize = inputData.size();
                size_t outputSize = compressedData.size();
                double ratio = Huffman::HuffmanCompressor::getCompressionRatio(inputSize, outputSize);
                std::cout << "Compression completed successfully!\n";
                std::cout << "Input size: " << inputSize << " bytes\n";
                std::cout << "Output size: " << outputSize << " bytes\n";
                std::cout << "Compression ratio: " << (ratio * 100) << "%\n";
                std::cout << "Space savings: " << ((1.0 - ratio) * 100) << "%\n";
            }

            return 0;
        } catch (const Huffman::HuffmanException& e) {
            std::cerr << "Compression error: " << e.what() << std::endl;
            return 1;
        }
    }

    int handleDecompression(const ArgumentParser& parser) {
        std::string inputFile = parser.getOption("input");
        std::string outputFile = parser.getOption("output");

        if (verbose_) {
            std::cout << "Starting decompression...\n";
            std::cout << "Input file: " << inputFile << std::endl;
            std::cout << "Output file: " << outputFile << std::endl;
        }

        try {
            Huffman::HuffmanDecompressor decompressor;
            
            // Read compressed file
            CryptoUtils::SecureBuffer compressedData = FileUtils::FileHandler::readFile(inputFile);
            
            // Load tree and decompress
            size_t treeSize = decompressor.loadTree(compressedData.toVector());
            CryptoUtils::SecureBuffer decompressedData = decompressor.decompress(compressedData.toVector(), treeSize);
            
            // Write decompressed data
            FileUtils::FileHandler::writeFile(outputFile, decompressedData);

            if (verbose_) {
                size_t inputSize = compressedData.size();
                size_t outputSize = decompressedData.size();
                std::cout << "Decompression completed successfully!\n";
                std::cout << "Input size: " << inputSize << " bytes\n";
                std::cout << "Output size: " << outputSize << " bytes\n";
            }

            return 0;
        } catch (const Huffman::HuffmanException& e) {
            std::cerr << "Decompression error: " << e.what() << std::endl;
            return 1;
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        ArgumentParser parser(argc, argv);
        FileEncryptoApp app;
        return app.run(parser);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
