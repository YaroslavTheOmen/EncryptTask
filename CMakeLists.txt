cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(ToDoConsol)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Include OpenSSL headers
include_directories(${OPENSSL_INCLUDE_DIR})

# Define the source files
set(SOURCES
    src/main.cpp
    src/src_classes.cpp
    src/src_utils.cpp
)

# Add executable
add_executable(todo_console ${SOURCES})

# Link OpenSSL libraries
target_link_libraries(todo_console ${OPENSSL_LIBRARIES})

# Optional: Set the output directory for binaries
set_target_properties(todo_console PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
