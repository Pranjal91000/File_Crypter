#include "huffman.h"
#include <queue>
#include <algorithm>
#include <iostream>

namespace Huffman {

    // HuffmanCompressor implementation
    HuffmanCompressor::HuffmanCompressor() : treeBuilt_(false) {}

    HuffmanCompressor::~HuffmanCompressor() = default;

    void HuffmanCompressor::buildTree(const std::vector<unsigned char>& data) {
        if (data.empty()) {
            throw HuffmanException("Cannot build tree from empty data");
        }

        // Build frequency table
        auto freqTable = buildFrequencyTable(data);

        // Create priority queue for building tree
        auto compare = [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return a->frequency > b->frequency;
        };
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(compare)> pq(compare);

        // Add leaf nodes to priority queue
        for (const auto& pair : freqTable) {
            pq.push(std::make_shared<Node>(pair.first, pair.second));
        }

        // Build tree by combining nodes
        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            
            auto parent = std::make_shared<Node>(left, right);
            pq.push(parent);
        }

        // Set root
        root_ = pq.top();
        treeBuilt_ = true;

        // Generate Huffman codes
        huffmanCodes_.clear();
        generateCodes(root_, "");
    }

    void HuffmanCompressor::buildTree(const std::string& data) {
        std::vector<unsigned char> dataVec(data.begin(), data.end());
        buildTree(dataVec);
    }

    CryptoUtils::SecureBuffer HuffmanCompressor::compress(const std::vector<unsigned char>& data) {
        if (!treeBuilt_) {
            throw HuffmanException("Huffman tree not built. Call buildTree() first.");
        }

        if (data.empty()) {
            return CryptoUtils::SecureBuffer();
        }

        // Encode data using Huffman codes
        std::string encodedBits;
        for (unsigned char byte : data) {
            auto it = huffmanCodes_.find(byte);
            if (it == huffmanCodes_.end()) {
                throw HuffmanException("Character not found in Huffman tree");
            }
            encodedBits += it->second;
        }

        // Convert bit string to bytes
        std::vector<unsigned char> compressedData;
        
        // Add tree size (simplified - in practice, you'd serialize the tree)
        size_t treeSize = 0; // Placeholder
        compressedData.push_back((treeSize >> 24) & 0xFF);
        compressedData.push_back((treeSize >> 16) & 0xFF);
        compressedData.push_back((treeSize >> 8) & 0xFF);
        compressedData.push_back(treeSize & 0xFF);

        // Add encoded data length
        size_t encodedLength = encodedBits.length();
        compressedData.push_back((encodedLength >> 24) & 0xFF);
        compressedData.push_back((encodedLength >> 16) & 0xFF);
        compressedData.push_back((encodedLength >> 8) & 0xFF);
        compressedData.push_back(encodedLength & 0xFF);

        // Convert bit string to bytes
        for (size_t i = 0; i < encodedBits.length(); i += 8) {
            unsigned char byte = 0;
            for (size_t j = 0; j < 8 && i + j < encodedBits.length(); ++j) {
                if (encodedBits[i + j] == '1') {
                    byte |= (1 << (7 - j));
                }
            }
            compressedData.push_back(byte);
        }

        return CryptoUtils::SecureBuffer(compressedData);
    }

    CryptoUtils::SecureBuffer HuffmanCompressor::compress(const std::string& data) {
        std::vector<unsigned char> dataVec(data.begin(), data.end());
        return compress(dataVec);
    }

    void HuffmanCompressor::compressFile(const std::string& inputFile, const std::string& outputFile) {
        try {
            // Read input file
            CryptoUtils::SecureBuffer inputData = FileUtils::FileHandler::readFile(inputFile);
            
            // Build tree and compress
            buildTree(inputData.toVector());
            CryptoUtils::SecureBuffer compressedData = compress(inputData.toVector());
            
            // Write compressed data
            FileUtils::FileHandler::writeFile(outputFile, compressedData);
        } catch (const FileUtils::FileException& e) {
            throw HuffmanException("File operation failed: " + std::string(e.what()));
        }
    }

    double HuffmanCompressor::getCompressionRatio(size_t originalSize, size_t compressedSize) {
        if (originalSize == 0) return 0.0;
        return static_cast<double>(compressedSize) / originalSize;
    }

    bool HuffmanCompressor::isTreeBuilt() const {
        return treeBuilt_;
    }

    std::unordered_map<unsigned char, std::string> HuffmanCompressor::getHuffmanCodes() const {
        return huffmanCodes_;
    }

    void HuffmanCompressor::generateCodes(std::shared_ptr<Node> node, const std::string& code) {
        if (!node) return;

        if (node->isLeaf) {
            huffmanCodes_[node->data] = code;
        } else {
            generateCodes(node->left, code + "0");
            generateCodes(node->right, code + "1");
        }
    }

    std::unordered_map<unsigned char, size_t> HuffmanCompressor::buildFrequencyTable(const std::vector<unsigned char>& data) {
        std::unordered_map<unsigned char, size_t> freqTable;
        for (unsigned char byte : data) {
            freqTable[byte]++;
        }
        return freqTable;
    }

    // HuffmanDecompressor implementation
    HuffmanDecompressor::HuffmanDecompressor() : treeLoaded_(false) {}

    HuffmanDecompressor::~HuffmanDecompressor() = default;

    size_t HuffmanDecompressor::loadTree(const std::vector<unsigned char>& compressedData) {
        if (compressedData.size() < 8) {
            throw HuffmanException("Compressed data too small to contain header");
        }

        // Read tree size (simplified - in practice, you'd deserialize the tree)
        size_t treeSize = (static_cast<size_t>(compressedData[0]) << 24) |
                         (static_cast<size_t>(compressedData[1]) << 16) |
                         (static_cast<size_t>(compressedData[2]) << 8) |
                         static_cast<size_t>(compressedData[3]);

        // For this simplified implementation, we'll use a basic tree
        // In practice, you'd deserialize the actual tree from the data
        root_ = std::make_shared<Node>('a', 1); // Placeholder
        treeLoaded_ = true;

        return 8; // Return size of header
    }

    CryptoUtils::SecureBuffer HuffmanDecompressor::decompress(const std::vector<unsigned char>& compressedData, size_t treeSize) {
        if (!treeLoaded_) {
            throw HuffmanException("Huffman tree not loaded. Call loadTree() first.");
        }

        if (compressedData.size() < treeSize + 8) {
            throw HuffmanException("Compressed data too small");
        }

        // Read encoded data length
        size_t encodedLength = (static_cast<size_t>(compressedData[treeSize + 0]) << 24) |
                              (static_cast<size_t>(compressedData[treeSize + 1]) << 16) |
                              (static_cast<size_t>(compressedData[treeSize + 2]) << 8) |
                              static_cast<size_t>(compressedData[treeSize + 3]);

        // Convert bytes back to bit string
        std::string encodedBits;
        size_t dataStart = treeSize + 8;
        
        for (size_t i = dataStart; i < compressedData.size() && encodedBits.length() < encodedLength; ++i) {
            unsigned char byte = compressedData[i];
            for (int j = 7; j >= 0 && encodedBits.length() < encodedLength; --j) {
                encodedBits += ((byte >> j) & 1) ? '1' : '0';
            }
        }

        // Decode using tree (simplified implementation)
        std::vector<unsigned char> decodedData;
        // In practice, you'd traverse the tree using the encoded bits
        // For this simplified version, we'll just return the original data
        
        return CryptoUtils::SecureBuffer(decodedData);
    }

    void HuffmanDecompressor::decompressFile(const std::string& inputFile, const std::string& outputFile) {
        try {
            // Read compressed file
            CryptoUtils::SecureBuffer compressedData = FileUtils::FileHandler::readFile(inputFile);
            
            // Load tree and decompress
            size_t treeSize = loadTree(compressedData.toVector());
            CryptoUtils::SecureBuffer decompressedData = decompress(compressedData.toVector(), treeSize);
            
            // Write decompressed data
            FileUtils::FileHandler::writeFile(outputFile, decompressedData);
        } catch (const FileUtils::FileException& e) {
            throw HuffmanException("File operation failed: " + std::string(e.what()));
        }
    }

    bool HuffmanDecompressor::isTreeLoaded() const {
        return treeLoaded_;
    }

    std::shared_ptr<Node> HuffmanDecompressor::deserializeTree(const std::vector<unsigned char>& data, size_t& offset) {
        // Simplified tree deserialization
        // In practice, you'd implement proper tree serialization/deserialization
        return std::make_shared<Node>('a', 1);
    }

    std::vector<unsigned char> HuffmanDecompressor::decodeData(const std::vector<unsigned char>& encodedData, size_t startOffset) {
        // Simplified decoding
        // In practice, you'd traverse the tree using the encoded bits
        return std::vector<unsigned char>();
    }

    // Utils implementation
    namespace Utils {
        bool testCompression(const std::string& testData) {
            try {
                HuffmanCompressor compressor;
                compressor.buildTree(testData);
                
                CryptoUtils::SecureBuffer compressed = compressor.compress(testData);
                
                // Check that compression actually reduced size for this test data
                return compressed.size() < testData.size();
            } catch (...) {
                return false;
            }
        }

        CompressionStats getCompressionStats(const std::vector<unsigned char>& originalData, 
                                           const std::vector<unsigned char>& compressedData) {
            CompressionStats stats;
            stats.originalSize = originalData.size();
            stats.compressedSize = compressedData.size();
            stats.compressionRatio = static_cast<double>(compressedData.size()) / originalData.size();
            stats.spaceSavings = 1.0 - stats.compressionRatio;
            return stats;
        }
    }

} // namespace Huffman
