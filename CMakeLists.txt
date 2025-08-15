cmake_minimum_required(VERSION 3.16)
project(FileEncrypto VERSION 1.2.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/des/des_openssl.cpp
    src/des/des_custom.cpp
    src/huffman/huffman.cpp
    src/utils/crypto_utils.cpp
    src/utils/file_utils.cpp
)

# Create executable
add_executable(file_encrypto ${SOURCES})

# Link OpenSSL
target_link_libraries(file_encrypto OpenSSL::SSL OpenSSL::Crypto)

# Set properties
set_target_properties(file_encrypto PROPERTIES
    OUTPUT_NAME "file_encrypto"
    VERSION ${PROJECT_VERSION}
)

# Installation
install(TARGETS file_encrypto
    RUNTIME DESTINATION bin
)

install(FILES README.md
    DESTINATION share/file-encrypto
)

# Testing
enable_testing()
if(EXISTS "${CMAKE_SOURCE_DIR}/tests")
    file(GLOB TEST_SOURCES "tests/*.cpp")
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCES})
        target_link_libraries(${TEST_NAME} OpenSSL::SSL OpenSSL::Crypto)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()

# Examples
if(EXISTS "${CMAKE_SOURCE_DIR}/examples")
    file(GLOB EXAMPLE_SOURCES "examples/*.cpp")
    foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
        add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE} ${SOURCES})
        target_link_libraries(${EXAMPLE_NAME} OpenSSL::SSL OpenSSL::Crypto)
    endforeach()
endif()

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FileEncryptoConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# CPack configuration
set(CPACK_PACKAGE_NAME "file-encrypto")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "File Encryption System with DES and Huffman Compression")
set(CPACK_PACKAGE_VENDOR "File Encryption System")
set(CPACK_PACKAGE_CONTACT "support@file-encrypto.com")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
endif()

include(CPack)
