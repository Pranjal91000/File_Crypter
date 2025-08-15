#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "../utils/crypto_utils.h"
#include "../utils/file_utils.h"

namespace Huffman {

    /**
     * @brief Exception class for Huffman operations
     */
    class HuffmanException : public std::runtime_error {
    public:
        explicit HuffmanException(const std::string& message) : std::runtime_error(message) {}
    };

    /**
     * @brief Huffman tree node
     */
    struct Node {
        unsigned char data;
        size_t frequency;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        bool isLeaf;

        Node(unsigned char d, size_t freq) 
            : data(d), frequency(freq), left(nullptr), right(nullptr), isLeaf(true) {}
        
        Node(std::shared_ptr<Node> l, std::shared_ptr<Node> r)
            : data(0), frequency(l->frequency + r->frequency), left(l), right(r), isLeaf(false) {}
    };

    /**
     * @brief Huffman compressor class
     */
    class HuffmanCompressor {
    private:
        std::unordered_map<unsigned char, std::string> huffmanCodes_;
        std::shared_ptr<Node> root_;
        bool treeBuilt_;

    public:
        HuffmanCompressor();
        ~HuffmanCompressor();

        /**
         * @brief Build Huffman tree from data
         * @param data Input data
         */
        void buildTree(const std::vector<unsigned char>& data);

        /**
         * @brief Build Huffman tree from string
         * @param data Input string
         */
        void buildTree(const std::string& data);

        /**
         * @brief Compress data
         * @param data Data to compress
         * @return Compressed data
         * @throws HuffmanException if tree is not built
         */
        CryptoUtils::SecureBuffer compress(const std::vector<unsigned char>& data);

        /**
         * @brief Compress string
         * @param data String to compress
         * @return Compressed data
         * @throws HuffmanException if tree is not built
         */
        CryptoUtils::SecureBuffer compress(const std::string& data);

        /**
         * @brief Compress file
         * @param inputFile Input file path
         * @param outputFile Output file path
         * @throws HuffmanException if compression fails
         */
        void compressFile(const std::string& inputFile, const std::string& outputFile);

        /**
         * @brief Get compression ratio
         * @param originalSize Original data size
         * @param compressedSize Compressed data size
         * @return Compression ratio (0.0 to 1.0)
         */
        static double getCompressionRatio(size_t originalSize, size_t compressedSize);

        /**
         * @brief Check if tree is built
         * @return true if tree is built, false otherwise
         */
        bool isTreeBuilt() const;

        /**
         * @brief Get Huffman codes
         * @return Map of character to code
         */
        std::unordered_map<unsigned char, std::string> getHuffmanCodes() const;

    private:
        /**
         * @brief Generate Huffman codes from tree
         * @param node Current node
         * @param code Current code
         */
        void generateCodes(std::shared_ptr<Node> node, const std::string& code);

        /**
         * @brief Build frequency table
         * @param data Input data
         * @return Frequency table
         */
        std::unordered_map<unsigned char, size_t> buildFrequencyTable(const std::vector<unsigned char>& data);
    };

    /**
     * @brief Huffman decompressor class
     */
    class HuffmanDecompressor {
    private:
        std::shared_ptr<Node> root_;
        bool treeLoaded_;

    public:
        HuffmanDecompressor();
        ~HuffmanDecompressor();

        /**
         * @brief Load Huffman tree from compressed data
         * @param compressedData Compressed data containing tree
         * @return Size of tree data
         * @throws HuffmanException if tree cannot be loaded
         */
        size_t loadTree(const std::vector<unsigned char>& compressedData);

        /**
         * @brief Decompress data
         * @param compressedData Compressed data
         * @param treeSize Size of tree data in compressed data
         * @return Decompressed data
         * @throws HuffmanException if decompression fails
         */
        CryptoUtils::SecureBuffer decompress(const std::vector<unsigned char>& compressedData, size_t treeSize);

        /**
         * @brief Decompress file
         * @param inputFile Input file path
         * @param outputFile Output file path
         * @throws HuffmanException if decompression fails
         */
        void decompressFile(const std::string& inputFile, const std::string& outputFile);

        /**
         * @brief Check if tree is loaded
         * @return true if tree is loaded, false otherwise
         */
        bool isTreeLoaded() const;

    private:
        /**
         * @brief Deserialize Huffman tree
         * @param data Tree data
         * @param offset Current offset in data
         * @return Root node of tree
         */
        std::shared_ptr<Node> deserializeTree(const std::vector<unsigned char>& data, size_t& offset);

        /**
         * @brief Decode data using tree
         * @param encodedData Encoded data
         * @param startOffset Starting offset in encoded data
         * @return Decoded data
         */
        std::vector<unsigned char> decodeData(const std::vector<unsigned char>& encodedData, size_t startOffset);
    };

    /**
     * @brief Utility functions for Huffman operations
     */
    namespace Utils {
        /**
         * @brief Test Huffman compression/decompression
         * @param testData Test data
         * @return true if test passes, false otherwise
         */
        bool testCompression(const std::string& testData = "Hello, Huffman compression test!");

        /**
         * @brief Get compression statistics
         * @param originalData Original data
         * @param compressedData Compressed data
         * @return Compression statistics
         */
        struct CompressionStats {
            size_t originalSize;
            size_t compressedSize;
            double compressionRatio;
            double spaceSavings;
        };

        CompressionStats getCompressionStats(const std::vector<unsigned char>& originalData, 
                                           const std::vector<unsigned char>& compressedData);
    };

} // namespace Huffman

#endif // HUFFMAN_H
