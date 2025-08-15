#include "file_utils.h"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#endif

namespace FileUtils {

    // FileHandler implementation
    CryptoUtils::SecureBuffer FileHandler::readFile(const std::string& filePath) {
        if (!fileExists(filePath)) {
            throw FileException("File does not exist: " + filePath);
        }

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw FileException("Cannot open file for reading: " + filePath);
        }

        // Get file size
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read file content
        CryptoUtils::SecureBuffer buffer(fileSize);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
        file.close();

        if (file.gcount() != static_cast<std::streamsize>(fileSize)) {
            throw FileException("Error reading file: " + filePath);
        }

        return buffer;
    }

    std::string FileHandler::readFileAsString(const std::string& filePath) {
        CryptoUtils::SecureBuffer buffer = readFile(filePath);
        return buffer.toString();
    }

    void FileHandler::writeFile(const std::string& filePath, const CryptoUtils::SecureBuffer& data) {
        // Create directory if it doesn't exist
        std::string dir = getDirectory(filePath);
        if (!dir.empty()) {
            createDirectory(dir);
        }

        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw FileException("Cannot open file for writing: " + filePath);
        }

        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();

        if (!file.good()) {
            throw FileException("Error writing to file: " + filePath);
        }
    }

    void FileHandler::writeFile(const std::string& filePath, const std::string& data) {
        // Create directory if it doesn't exist
        std::string dir = getDirectory(filePath);
        if (!dir.empty()) {
            createDirectory(dir);
        }

        std::ofstream file(filePath);
        if (!file.is_open()) {
            throw FileException("Cannot open file for writing: " + filePath);
        }

        file << data;
        file.close();

        if (!file.good()) {
            throw FileException("Error writing to file: " + filePath);
        }
    }

    void FileHandler::writeFile(const std::string& filePath, const std::vector<unsigned char>& data) {
        // Create directory if it doesn't exist
        std::string dir = getDirectory(filePath);
        if (!dir.empty()) {
            createDirectory(dir);
        }

        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw FileException("Cannot open file for writing: " + filePath);
        }

        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();

        if (!file.good()) {
            throw FileException("Error writing to file: " + filePath);
        }
    }

    bool FileHandler::fileExists(const std::string& filePath) {
        return std::filesystem::exists(filePath);
    }

    size_t FileHandler::getFileSize(const std::string& filePath) {
        if (!fileExists(filePath)) {
            throw FileException("File does not exist: " + filePath);
        }

        std::error_code ec;
        size_t size = std::filesystem::file_size(filePath, ec);
        if (ec) {
            throw FileException("Cannot get file size: " + filePath);
        }

        return size;
    }

    void FileHandler::createDirectory(const std::string& dirPath) {
        if (dirPath.empty()) {
            return;
        }

        std::error_code ec;
        std::filesystem::create_directories(dirPath, ec);
        if (ec) {
            throw FileException("Cannot create directory: " + dirPath);
        }
    }

    std::string FileHandler::getFileExtension(const std::string& filePath) {
        std::filesystem::path path(filePath);
        return path.extension().string().substr(1); // Remove the dot
    }

    std::string FileHandler::getFileName(const std::string& filePath) {
        std::filesystem::path path(filePath);
        return path.stem().string();
    }

    std::string FileHandler::getDirectory(const std::string& filePath) {
        std::filesystem::path path(filePath);
        return path.parent_path().string();
    }

    void FileHandler::setFilePermissions(const std::string& filePath, int permissions) {
#ifdef _WIN32
        // Windows doesn't have Unix-style permissions
        // This is a placeholder for Windows-specific permission handling
        (void)filePath;
        (void)permissions;
#else
        if (chmod(filePath.c_str(), permissions) != 0) {
            throw FileException("Cannot set file permissions: " + filePath);
        }
#endif
    }

    void FileHandler::secureDelete(const std::string& filePath) {
        if (!fileExists(filePath)) {
            return;
        }

        // Overwrite file with random data before deletion
        try {
            size_t fileSize = getFileSize(filePath);
            if (fileSize > 0) {
                std::vector<unsigned char> randomData = CryptoUtils::Security::randomBytes(fileSize);
                writeFile(filePath, randomData);
            }
        } catch (...) {
            // Continue with deletion even if overwrite fails
        }

        // Delete the file
        std::error_code ec;
        std::filesystem::remove(filePath, ec);
        if (ec) {
            throw FileException("Cannot delete file: " + filePath);
        }
    }

    // FileValidator implementation
    bool FileValidator::isValidPath(const std::string& filePath) {
        if (filePath.empty()) {
            return false;
        }

        // Check for invalid characters
        std::string invalidChars = "<>:\"|?*";
        for (char c : invalidChars) {
            if (filePath.find(c) != std::string::npos) {
                return false;
            }
        }

        return true;
    }

    bool FileValidator::isReadable(const std::string& filePath) {
        if (!FileHandler::fileExists(filePath)) {
            return false;
        }

        std::ifstream file(filePath);
        bool readable = file.good();
        file.close();
        return readable;
    }

    bool FileValidator::isWritable(const std::string& filePath) {
        std::ofstream file(filePath, std::ios::app);
        bool writable = file.good();
        file.close();
        
        // Remove the test file if it was created
        if (FileHandler::fileExists(filePath) && FileHandler::getFileSize(filePath) == 0) {
            std::filesystem::remove(filePath);
        }
        
        return writable;
    }

    bool FileValidator::isValidFileSize(const std::string& filePath, size_t maxSize) {
        try {
            size_t fileSize = FileHandler::getFileSize(filePath);
            return fileSize <= maxSize;
        } catch (...) {
            return false;
        }
    }

    // FileBackup implementation
    void FileBackup::createBackup(const std::string& filePath, const std::string& backupPath) {
        if (!FileHandler::fileExists(filePath)) {
            throw FileException("Source file does not exist: " + filePath);
        }

        CryptoUtils::SecureBuffer data = FileHandler::readFile(filePath);
        FileHandler::writeFile(backupPath, data);
    }

    void FileBackup::restoreFromBackup(const std::string& backupPath, const std::string& filePath) {
        if (!FileHandler::fileExists(backupPath)) {
            throw FileException("Backup file does not exist: " + backupPath);
        }

        CryptoUtils::SecureBuffer data = FileHandler::readFile(backupPath);
        FileHandler::writeFile(filePath, data);
    }

    std::string FileBackup::generateBackupName(const std::string& originalPath) {
        std::filesystem::path path(originalPath);
        std::string fileName = path.stem().string();
        std::string extension = path.extension().string();

        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");

        return fileName + "_backup_" + ss.str() + extension;
    }

} // namespace FileUtils
