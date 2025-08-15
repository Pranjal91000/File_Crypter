#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "crypto_utils.h"

namespace FileUtils {

    /**
     * @brief Exception class for file operations
     */
    class FileException : public std::runtime_error {
    public:
        explicit FileException(const std::string& message) : std::runtime_error(message) {}
    };

    /**
     * @brief File operation utilities
     */
    class FileHandler {
    public:
        /**
         * @brief Read file content securely
         * @param filePath Path to the file
         * @return File content as secure buffer
         * @throws FileException if file cannot be read
         */
        static CryptoUtils::SecureBuffer readFile(const std::string& filePath);
        
        /**
         * @brief Read file content as string
         * @param filePath Path to the file
         * @return File content as string
         * @throws FileException if file cannot be read
         */
        static std::string readFileAsString(const std::string& filePath);
        
        /**
         * @brief Write data to file securely
         * @param filePath Path to the file
         * @param data Data to write
         * @throws FileException if file cannot be written
         */
        static void writeFile(const std::string& filePath, const CryptoUtils::SecureBuffer& data);
        
        /**
         * @brief Write string to file
         * @param filePath Path to the file
         * @param data String to write
         * @throws FileException if file cannot be written
         */
        static void writeFile(const std::string& filePath, const std::string& data);
        
        /**
         * @brief Write binary data to file
         * @param filePath Path to the file
         * @param data Binary data to write
         * @throws FileException if file cannot be written
         */
        static void writeFile(const std::string& filePath, const std::vector<unsigned char>& data);
        
        /**
         * @brief Check if file exists
         * @param filePath Path to the file
         * @return true if file exists, false otherwise
         */
        static bool fileExists(const std::string& filePath);
        
        /**
         * @brief Get file size
         * @param filePath Path to the file
         * @return File size in bytes
         * @throws FileException if file cannot be accessed
         */
        static size_t getFileSize(const std::string& filePath);
        
        /**
         * @brief Create directory if it doesn't exist
         * @param dirPath Path to the directory
         * @throws FileException if directory cannot be created
         */
        static void createDirectory(const std::string& dirPath);
        
        /**
         * @brief Get file extension
         * @param filePath Path to the file
         * @return File extension (without dot)
         */
        static std::string getFileExtension(const std::string& filePath);
        
        /**
         * @brief Get file name without extension
         * @param filePath Path to the file
         * @return File name without extension
         */
        static std::string getFileName(const std::string& filePath);
        
        /**
         * @brief Get directory path
         * @param filePath Path to the file
         * @return Directory path
         */
        static std::string getDirectory(const std::string& filePath);
        
        /**
         * @brief Set file permissions (Unix-like systems)
         * @param filePath Path to the file
         * @param permissions Permission bits
         * @throws FileException if permissions cannot be set
         */
        static void setFilePermissions(const std::string& filePath, int permissions);
        
        /**
         * @brief Secure file deletion
         * @param filePath Path to the file
         * @throws FileException if file cannot be deleted
         */
        static void secureDelete(const std::string& filePath);
    };

    /**
     * @brief File validation utilities
     */
    class FileValidator {
    public:
        /**
         * @brief Validate file path
         * @param filePath Path to validate
         * @return true if valid, false otherwise
         */
        static bool isValidPath(const std::string& filePath);
        
        /**
         * @brief Check if file is readable
         * @param filePath Path to the file
         * @return true if readable, false otherwise
         */
        static bool isReadable(const std::string& filePath);
        
        /**
         * @brief Check if file is writable
         * @param filePath Path to the file
         * @return true if writable, false otherwise
         */
        static bool isWritable(const std::string& filePath);
        
        /**
         * @brief Validate file size
         * @param filePath Path to the file
         * @param maxSize Maximum allowed file size in bytes
         * @return true if size is valid, false otherwise
         */
        static bool isValidFileSize(const std::string& filePath, size_t maxSize);
    };

    /**
     * @brief File backup utilities
     */
    class FileBackup {
    public:
        /**
         * @brief Create backup of a file
         * @param filePath Path to the file
         * @param backupPath Path for backup file
         * @throws FileException if backup cannot be created
         */
        static void createBackup(const std::string& filePath, const std::string& backupPath);
        
        /**
         * @brief Restore file from backup
         * @param backupPath Path to backup file
         * @param filePath Path to restore to
         * @throws FileException if restore cannot be performed
         */
        static void restoreFromBackup(const std::string& backupPath, const std::string& filePath);
        
        /**
         * @brief Generate backup filename
         * @param originalPath Original file path
         * @return Backup filename
         */
        static std::string generateBackupName(const std::string& originalPath);
    };

} // namespace FileUtils

#endif // FILE_UTILS_H
