#include "../headers/classes.h"
#include "../headers/commands.h"
#include "../headers/encryption.h"
#include "../headers/utils.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string>
#include <vector>

int main() {

  system("clear");

  // No need to initialize OpenSSL in OpenSSL 1.1.0 and later
  // OpenSSL_add_all_algorithms();
  // ERR_load_crypto_strings();
  std::vector<MyNote::note *> notes;
  settings config;

  const std::string filename = "../nlib/notes.dat";
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
      if (!decrypt_data(notes, config, filename, password)) {
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
    if (!encrypt_data(notes, config, filename, password)) {
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

  // No need to clean up OpenSSL in OpenSSL 1.1.0 and later
  // EVP_cleanup();
  // ERR_free_strings();

  return 0;
}
