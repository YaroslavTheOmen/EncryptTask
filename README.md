# EncryptTask

A secure, console-based note-taking application written in C++, featuring AES-256 encryption using OpenSSL. The application allows users to create, view, edit, and delete notes securely. It prompts users for a password on startup and encrypts all stored data to ensure privacy.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [macOS](#macos)
  - [Linux](#linux)
- [Building the Application](#building-the-application)
- [Running the Application](#running-the-application)
- [Usage](#usage)
  - [Commands](#commands)
- [Uninstallation](#uninstallation)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Features

- Secure storage of notes with AES-256 encryption.
- Password protection on application startup.
- Support for different types of notes:
  - **Headed Note**: Includes a header and content.
  - **Date Note**: Associated with a specific date.
- Priority levels for notes: High, Middle, Low.
- Command-line interface for managing notes.
- Cross-platform support: macOS and Linux.

## Prerequisites

- **C++17** or later.
- **CMake** version 3.10 or higher.
- **OpenSSL** development libraries.

### macOS

- **Xcode Command Line Tools**:

  ```bash
  xcode-select --install
  ```

- **Homebrew** (if not already installed):

  ```bash
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  ```

- **OpenSSL**:

  ```bash
  brew install openssl
  ```

### Linux (Debian/Ubuntu)

- **Update package lists**:

  ```bash
  sudo apt-get update
  ```

- **Install required packages**:

  ```bash
  sudo apt-get install build-essential cmake libssl-dev
  ```

## Installation

### macOS

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/yourusername/EncryptTask.git
   cd EncryptTask
   ```

2. **Create Build Directory**:

   ```bash
   mkdir build
   cd build
   ```

3. **Configure the Project**:

   ```bash
   cmake ..
   ```

4. **Build the Project**:

   ```bash
   make
   ```

   - The executable will be placed in the `bin` directory inside the project root (`EncryptTask/bin/EncryptTask`).

### Linux

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/yourusername/EncryptTask.git
   cd EncryptTask
   ```

2. **Create Build Directory**:

   ```bash
   mkdir build
   cd build
   ```

3. **Configure the Project**:

   ```bash
   cmake ..
   ```

4. **Build the Project**:

   ```bash
   make
   ```

   - The executable will be placed in the `bin` directory inside the project root (`EncryptTask/bin/EncryptTask`).

## Building the Application

If you need to rebuild the application after making changes:

1. **Clean the Build Directory**:

   ```bash
   make clean
   ```

2. **Rebuild**:

   ```bash
   make
   ```

## Running the Application

### macOS and Linux

1. **Open Terminal**.

2. **Navigate to the Executable**:

   ```bash
   cd path_to_EncryptTask/bin
   ```

   - Replace `path_to_EncryptTask` with the actual path to the `EncryptTask` project directory.

3. **Run the Application**:

   ```bash
   ./EncryptTask
   ```

## Usage

Upon running the application for the first time:

- **Create a New Password**: You'll be prompted to create a new password. This password will be used to encrypt and decrypt your notes.
- **Subsequent Runs**: You'll be prompted to enter your password to access your encrypted notes.

### Commands

Use the following commands within the application to manage your notes:

- **Add a Note**:

  - **Syntax**:

    ```
    :a [type] "note content" ["header/date"] [priority]
    ```

  - **Types**:

    - `h`: Headed note.
    - `d`: Date note.

  - **Example**:

    - Add a headed note with high priority:

      ```
      :a h "This is a test note" "Test Header" h
      ```

- **View Notes**:

  ```
  :v
  ```

- **Change a Note**:

  - **Syntax**:

    ```
    :c note_number fields [values...]
    ```

  - **Fields**:

    - `n`: Change note text.
    - `h`: Change header (for headed notes).
    - `d`: Change date (for date notes).
    - `p`: Change priority.

  - **Example**:

    - Change note 1's content and priority:

      ```
      :c 1 np "Updated content" m
      ```

- **Delete a Note**:

  ```
  :d note_number
  ```

- **Change Settings**:

  ```
  :s [setting_name] [values...]
  ```

- **Help**:

  ```
  :h
  ```

- **Exit the Application**:

  ```
  :q
  ```

## Uninstallation

### macOS and Linux

1. **Delete the Build and Binary Files**:

   ```bash
   rm -rf path_to_EncryptTask/build
   rm -rf path_to_EncryptTask/bin
   ```

   - Replace `path_to_EncryptTask` with the actual path to the `EncryptTask` project directory.

2. **Remove Configuration and Data Files**:

   - Delete `notes.dat` or any other data files stored by the application in the project directory or your home directory.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- **OpenSSL**: Used for encryption and decryption.
- **CMake**: Build system generator.
- **Contributors**: Thanks to everyone who has contributed to this project.

---

**Note**: This application is a console-based program and does not have a graphical user interface. It must be run from a terminal or command prompt. Ensure that you have the necessary permissions and that all dependencies are installed.

If you encounter any issues or have questions, please feel free to open an issue on the project's GitHub repository.
