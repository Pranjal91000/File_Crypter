#ifndef HUFFMAN_FILE_HPP
#define HUFFMAN_FILE_HPP

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <chrono>

void compressFile(const std::string& inputFile, const std::string& outputFile);
void decompressFile(const std::string& inputFile, const std::string& outputFile);

#endif // HUFFMAN_FILE_HPP
