cmake_minimum_required(VERSION 3.10)
project(EncryptTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add your source files
set(SOURCES
    src/main.cpp
    src/src_classes.cpp
    src/src_commands.cpp
    src/src_serialization.cpp
    src/src_utils.cpp
    src/src_encryption.cpp
)


# Include directories
include_directories(headers)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

add_executable(EncryptTask ${SOURCES})

# Link OpenSSL libraries
target_link_libraries(EncryptTask OpenSSL::SSL OpenSSL::Crypto)
