# File Encrypto in C++

This project provides file encryption and compression utilities using DES encryption and Huffman coding in C++. It is organized into several modules for encryption, decryption, and compression.

## Project Structure

```
.vscode/                # VSCode configuration
app/                    # Example files and outputs
DES/                    # DES encryption/decryption (classic and OpenSSL)
encdec/                 # Combined encryption, decryption, and compression
    DES/                # DES implementation for encdec
    Huffman/            # Huffman coding implementation
```

## Features

- **DES Encryption/Decryption**: Encrypt and decrypt files using DES.
- **Huffman Compression/Decompression**: Compress and decompress files using Huffman coding.
- **Combined Workflow**: Encrypt, compress, decompress, and decrypt files in sequence.

## How to Build

Make sure you have a C++ compiler (e.g., `g++`) and optionally OpenSSL for the classic DES implementation.

### Build All Modules

```sh
# Example for encdec/main.cpp
g++ -std=c++17 -o encdec/main.exe encdec/main.cpp encdec/DES/des.cpp encdec/Huffman/huffman.cpp
# Example for DES/main.cpp
g++ -std=c++17 -o DES/main.exe DES/main.cpp
# Example for DES/decrypt.cpp (requires OpenSSL)
g++ -std=c++17 -o DES/decrypt.exe DES/decrypt.cpp -lssl -lcrypto
```

## How to Use

### 1. DES Encryption/Decryption

- Place your plaintext in `DES/input.txt`.
- Place your key (16 hex characters) in `DES/key.txt`.
- Run the DES encryption:

```sh
./DES/main.exe
```

- Encrypted output will be in `DES/encrypted.txt`.

- To decrypt using OpenSSL DES (optional):

```sh
./DES/decrypt.exe
```

- Decrypted output will be in `DES/decrypted.txt`.

### 2. Combined Encryption, Compression, and Decryption

- Place your plaintext in `encdec/input.txt`.
- Place your key in `encdec/key.txt`.
- Run the combined workflow:

```sh
./encdec/main.exe
```

- Outputs:
  - Encrypted file: `encdec/encrypted_output.bin`
  - Compressed file: `encdec/compressed.bin`
  - Decompressed file: `encdec/decompressed.txt`
  - Decrypted file: `encdec/decrypted_output.txt`

### 3. Huffman Compression/Decompression (Standalone)

- Use [`encdec/Huffman/huffman.cpp`](encdec/Huffman/huffman.cpp) directly for compression/decompression.
- Example usage in code:
  - `compressFile("input.txt", "compressed.bin");`
  - `decompressFile("compressed.bin", "decompressed.txt");`

## Configuration

- VSCode settings are provided in [.vscode/settings.json](.vscode/settings.json) and [.vscode/c_cpp_properties.json](.vscode/c_cpp_properties.json).
- Launch configurations for debugging are in [.vscode/launch.json](.vscode/launch.json).

## Example Files

- `app/z.txt`, `encdec/input.txt`: Example plaintext files.
- `encdec/key.txt`: Example DES key.

## License

This project is for educational purposes.

---
