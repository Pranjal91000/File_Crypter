# File Encryption System in C++

A comprehensive file encryption and compression system implementing DES (Data Encryption Standard) encryption and Huffman compression algorithms.

## Features

- **DES Encryption/Decryption**: Two implementations available:
  - **OpenSSL-based DES**: Production-ready, highly optimized implementation using OpenSSL
  - **Custom DES**: Educational/reference implementation with full algorithm visibility
- **Huffman Compression**: Lossless data compression for efficient storage
- **Cross-platform**: Works on Windows, Linux, and macOS
- **Production-ready**: Proper error handling, input validation, and security measures

## Project Structure

```
File_Crypter/
├── src/
│   ├── des/
│   │   ├── des_openssl.cpp      # OpenSSL-based DES implementation
│   │   ├── des_openssl.h
│   │   ├── des_custom.cpp       # Custom DES implementation
│   │   └── des_custom.h
│   ├── huffman/
│   │   ├── huffman.cpp          # Huffman compression implementation
│   │   └── huffman.h
│   ├── utils/
│   │   ├── file_utils.cpp       # File handling utilities
│   │   ├── file_utils.h
│   │   ├── crypto_utils.cpp     # Cryptographic utilities
│   │   └── crypto_utils.h
│   └── main.cpp                 # Main application entry point
├── include/                     # Header files
├── build/                       # Build artifacts
├── tests/                       # Test files
├── examples/                    # Example files
├── docs/                        # Documentation
│   └── DES_INTEGRATION_SUMMARY.md
├── Makefile                     # Build configuration
├── CMakeLists.txt              # CMake configuration
├── .gitignore                  # Git ignore rules
├── key.txt                     # Sample key file
├── test_input.txt              # Sample input file
└── README.md                   # This file
```

## Prerequisites

### Required Dependencies

1. **C++ Compiler**: GCC 7+ or Clang 6+ or MSVC 2017+
2. **OpenSSL**: For cryptographic operations
3. **Make**: For building the project
4. **CMake**: For alternative build system

### Installing Dependencies

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential libssl-dev cmake
```

#### CentOS/RHEL:
```bash
sudo yum groupinstall "Development Tools"
sudo yum install openssl-devel cmake
```

#### macOS:
```bash
brew install openssl cmake
```

#### Windows:
- Install Visual Studio 2017 or later
- Install OpenSSL for Windows
- Install CMake

## Installation

### Method 1: Using Make

```bash
# Clone the repository
git clone <repository-url>
cd "File Encrypto in C++"

# Build the project
make

# Install (optional)
sudo make install
```

### Method 2: Using CMake

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Install (optional)
sudo make install
```

## Usage

### Basic Usage

```bash
# Encrypt a file
./file_encrypto --encrypt --input input.txt --output encrypted.bin --key key.txt

# Decrypt a file
./file_encrypto --decrypt --input encrypted.bin --output decrypted.txt --key key.txt

# Compress a file
./file_encrypto --compress --input input.txt --output compressed.bin

# Decompress a file
./file_encrypto --decompress --input compressed.bin --output decompressed.txt

# Encrypt and compress
./file_encrypto --encrypt --compress --input input.txt --output encrypted_compressed.bin --key key.txt
```

### Command Line Options

| Option | Description | Required |
|--------|-------------|----------|
| `--encrypt` | Encrypt the input file | No |
| `--decrypt` | Decrypt the input file | No |
| `--compress` | Compress the input file | No |
| `--decompress` | Decompress the input file | No |
| `--input <file>` | Input file path | Yes |
| `--output <file>` | Output file path | Yes |
| `--key <file>` | Key file path (for encryption/decryption) | When encrypting/decrypting |
| `--algorithm <des>` | Encryption algorithm (default: des) | No |
| `--des-type <type>` | DES implementation type: openssl (default) or custom | No |
| `--help` | Show help message | No |
| `--version` | Show version information | No |

### Key File Format

The key file should contain a 64-bit (8-byte) key in hexadecimal format:

```
133457799BBCDFF1
```

### Choosing DES Implementation

The system provides two DES implementations, each with different characteristics:

#### OpenSSL-based DES (Default)
- **Use case**: Production environments, high-performance requirements
- **Advantages**: 
  - Highly optimized and tested
  - Industry-standard implementation
  - Better performance
  - Constant-time operations for security
  - Regular security updates
- **Disadvantages**: 
  - Less educational value
  - Requires OpenSSL dependency

#### Custom DES Implementation
- **Use case**: Educational purposes, learning cryptography, research
- **Advantages**: 
  - Full algorithm visibility
  - Educational value
  - No external cryptographic dependencies
  - Complete control over implementation
- **Disadvantages**: 
  - Slower performance
  - Not optimized for production use
  - May have implementation bugs
  - No security audits

#### Usage Examples

```bash
# Use OpenSSL-based DES (default)
./file_encrypto --encrypt --input secret.txt --output encrypted.bin --key key.txt

# Use custom DES implementation
./file_encrypto --encrypt --input secret.txt --output encrypted.bin --key key.txt --des-type custom

# Use custom DES for decryption
./file_encrypto --decrypt --input encrypted.bin --output decrypted.txt --key key.txt --des-type custom
```

**Recommendation**: Use OpenSSL-based DES for production use and custom DES for educational purposes.

## Examples

See the `examples/` directory for sample code and usage patterns:

- `basic_usage.cpp`: Basic encryption, decryption, and compression examples
- `des_comparison.cpp`: Comparison between OpenSSL and custom DES implementations

### Running Examples

```bash
# Build examples
make examples

# Run basic usage example
./build/basic_usage

# Run DES comparison example
./build/des_comparison
```
  
### Example Workflow

1. **Create a key file**:
   ```bash
   echo "133457799BBCDFF1" > key.txt
   ```

2. **Create an input file**:
   ```bash
   echo "Hello, this is a secret message!" > secret.txt
   ```

3. **Encrypt the file**:
   ```bash
   ./file_encrypto --encrypt --input secret.txt --output secret.enc --key key.txt
   ```

4. **Decrypt the file**:
   ```bash
   ./file_encrypto --decrypt --input secret.enc --output secret_decrypted.txt --key key.txt
   ```

5. **Verify the result**:
   ```bash
   diff secret.txt secret_decrypted.txt
   ```

## API Usage

### C++ API

#### OpenSSL-based DES (Recommended for Production)

```cpp
#include "des/des_openssl.h"
#include "huffman/huffman.h"
#include "utils/file_utils.h"

// Encrypt a file
DES::DESEncryptor encryptor;
encryptor.loadKeyFromFile("key.txt");
encryptor.encryptFile("input.txt", "output.bin");

// Decrypt a file
DES::DESDecryptor decryptor;
decryptor.loadKeyFromFile("key.txt");
decryptor.decryptFile("output.bin", "decrypted.txt");

// Compress a file
Huffman::HuffmanCompressor compressor;
compressor.buildTree(inputData);
compressor.compressFile("input.txt", "compressed.bin");
```

#### Custom DES (Educational/Reference)

```cpp
#include "des/des_custom.h"
#include "huffman/huffman.h"
#include "utils/file_utils.h"

// Encrypt a file
DESCustom::DESEncryptor encryptor;
encryptor.loadKeyFromFile("key.txt");
encryptor.encryptFile("input.txt", "output.bin");

// Decrypt a file
DESCustom::DESDecryptor decryptor;
decryptor.loadKeyFromFile("key.txt");
decryptor.decryptFile("output.bin", "decrypted.txt");

// Generate a random key
std::string key = DESCustom::Utils::generateKey();

// Test the implementation
bool testResult = DESCustom::Utils::testEncryptionDecryption();
```

#### Huffman Compression API

```cpp
#include "huffman/huffman.h"

// Compress a file
Huffman::HuffmanCompressor compressor;
compressor.buildTree(inputData);
compressor.compressFile("input.txt", "compressed.bin");

// Decompress a file
Huffman::HuffmanDecompressor decompressor;
decompressor.decompressFile("compressed.bin", "decompressed.txt");
```

## Security Considerations

1. **Key Management**: Store keys securely and never commit them to version control
2. **Key Strength**: Use strong, randomly generated keys
3. **File Permissions**: Ensure encrypted files have appropriate permissions
4. **Memory Security**: Sensitive data is cleared from memory after use
5. **Input Validation**: All inputs are validated to prevent buffer overflows

## Testing

Run the test suite:

```bash
# Run all tests
make test

# Run specific test categories
make test-encryption
make test-compression
make test-integration
```

## Performance

- **Encryption Speed**: ~10MB/s on modern hardware
- **Compression Ratio**: Typically 20-40% for text files
- **Memory Usage**: Minimal memory footprint

## Troubleshooting

### Common Issues

1. **OpenSSL not found**:
   ```bash
   # Ubuntu/Debian
   sudo apt install libssl-dev
   
   # macOS
   brew install openssl
   ```

2. **Permission denied**:
   ```bash
   chmod +x file_encrypto
   ```

3. **Key file not found**:
   Ensure the key file exists and has correct permissions

### Debug Mode

Enable debug output:

```bash
./file_encrypto --debug --encrypt --input file.txt --output file.enc --key key.txt
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Version History

- **v1.0.0**: Initial release with DES encryption and Huffman compression
- **v1.1.0**: Added OpenSSL implementation and improved error handling
- **v1.2.0**: Production-ready release with comprehensive testing

## Support

For support and questions:
- Create an issue on GitHub
- Check the documentation in the `docs/` directory
- Review the troubleshooting section above
