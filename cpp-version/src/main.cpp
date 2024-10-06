#include "../headers/classes.h"
#include "../headers/commands.h"
#include "../headers/encryption.h"
#include "../headers/utils.h"
#include <cstdlib>
#include <filesystem> // C++17 filesystem library
#include <fstream>
#include <iostream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string>
#include <vector>

#ifdef __linux__
#include <libgen.h> // For dirname
#include <limits.h> // For PATH_MAX
#include <unistd.h> // For readlink
#elif defined(__APPLE__)
#include <limits.h>      // For PATH_MAX
#include <mach-o/dyld.h> // For _NSGetExecutablePath
#endif

namespace fs = std::filesystem;

// Function to get the executable's directory
std::string get_executable_dir() {
  char path[PATH_MAX];
  std::string exePath;

#ifdef __linux__
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
  if (count == -1 || count >= PATH_MAX) {
    throw std::runtime_error("Failed to determine executable path.");
  }
  exePath = std::string(path, count);
#elif defined(__APPLE__)
  uint32_t size = PATH_MAX;
  if (_NSGetExecutablePath(path, &size) != 0) {
    throw std::runtime_error("Executable path is too long.");
  }
  // Resolve any symbolic links, relative paths, etc.
  char resolvedPath[PATH_MAX];
  if (realpath(path, resolvedPath) == nullptr) {
    throw std::runtime_error("Failed to resolve executable path.");
  }
  exePath = std::string(resolvedPath);
#else
#error "Unsupported platform"
#endif

  // Extract the directory part
  fs::path p = fs::path(exePath).parent_path();
  return p.string();
}

int main() {
  try {
    // Clear the terminal screen
    system("clear");

    // Determine the executable's directory
    std::string exeDir = get_executable_dir();

    // Construct the path to ../nlib/relative to exeDir
    fs::path nlibPath = fs::path(exeDir) / "../../nlib";

    // Normalize the path to eliminate any ".." or "." segments
    nlibPath = fs::weakly_canonical(nlibPath);

    // Ensure the ../nlib/ directory exists
    if (!fs::exists(nlibPath)) {
      // Attempt to create the directory
      if (!fs::create_directories(nlibPath)) {
        std::cerr << "Failed to create directory: " << nlibPath << std::endl;
        return 1;
      }
    }

    // Construct the full path to notes.dat
    fs::path filename = nlibPath / "notes.dat";

    // Optionally, convert to absolute path
    filename = fs::absolute(filename);

    // Initialize variables
    std::vector<MyNote::note *> notes;
    settings config;
    std::string password;

    // Check if file exists
    std::ifstream infile(filename, std::ios::binary);
    bool file_exists = infile.good();
    infile.close();

    // Prompt for password
    if (!file_exists) {
      // File does not exist; ask user to create a new password
      std::cout << "No existing data found. Please create a new password: ";
      std::getline(std::cin, password);
      if (password.empty()) {
        std::cerr << "Password cannot be empty. Exiting." << std::endl;
        return 1;
      }
      std::cout << "New password set." << std::endl;
    } else {
      // File exists; ask user to enter the password
      std::cout << "Please enter your password: ";
      std::getline(std::cin, password);
      if (password.empty()) {
        std::cerr << "Password cannot be empty. Exiting." << std::endl;
        return 1;
      }

      // Attempt to load data from the file
      try {
        if (!decrypt_data(notes, config, filename.string(), password)) {
          std::cerr
              << "Failed to decrypt data. Incorrect password or corrupted file."
              << std::endl;
          return 1;
        }
        std::cout << "Data loaded successfully." << std::endl;
      } catch (const std::exception &e) {
        std::cout << "Failed to load data: " << e.what() << std::endl;
        std::cout << "Starting with empty notes and default settings."
                  << std::endl;
      }
    }

    // Run the command loop
    try {
      command_loop(notes, config);
    } catch (const std::exception &e) {
      std::cout << "A fatal error occurred: " << e.what() << std::endl;
    }

    // Save data before exiting
    try {
      if (!encrypt_data(notes, config, filename.string(), password)) {
        std::cerr << "Failed to encrypt data." << std::endl;
      } else {
        std::cout << "Data saved successfully." << std::endl;
      }
    } catch (const std::exception &e) {
      std::cout << "Failed to save data: " << e.what() << std::endl;
    }

    // Clean up memory
    for (auto note : notes) {
      delete note;
    }

    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "Initialization error: " << ex.what() << std::endl;
    return 1;
  }
}
