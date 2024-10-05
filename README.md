# EncryptTask

A secure, console-based note-taking application written in C++, featuring AES-256 encryption using OpenSSL. The application allows users to create, view, edit, and delete notes securely. It prompts users for a password on startup and encrypts all stored data to ensure privacy.

By default, notes are stored in `project_dir/nlib/notes.dat`

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
  - [macOS](#macos)
  - [Linux (Debian/Ubuntu)](#linux-debianubuntu)
- [Installation (macOS / Linux)](#installation-macos--linux)
- [Building the Application](#building-the-application)
- [Running the Application (macOS / Linux)](#running-the-application-macos--linux)
- [Usage](#usage)
  - [Commands](#commands)
- [Uninstallation](#uninstallation)
  - [macOS and Linux](#macos-and-linux)
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

## Installation (macOS / Linux)

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/YaroslavTheOmen/EncryptTask.git
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

## Running the Application (macOS / Linux)

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

The application supports a variety of commands to manage your notes efficiently. Below is a list of available commands along with their descriptions:

#### Exit Commands

- `:quit`, `:exit`
  - **Description**: Exit the program.

#### Adding Notes

- `:wh "note text" "header (default = Untitled)" h/m/l`

  - **Description**: Create a new **headed note** with the specified text, header, and priority.
  - **Parameters**:
    - `"note text"`: The content of the note.
    - `"header"`: The header/title of the note. Defaults to "Untitled" if not provided.
    - `h/m/l`: Priority level (high, medium, low).

- `:wd "note text" "date (default = today)" h/m/l`
  - **Description**: Create a new **dated note** with the specified text, date, and priority.
  - **Parameters**:
    - `"note text"`: The content of the note.
    - `"date"`: The date for the note (yyyy/mm/dd). Defaults to today's date if not provided.
    - `h/m/l`: Priority level (high, medium, low).

#### Managing Notes

- `:d note_number`

  - **Description**: Delete a note by its number.
  - **Parameters**:
    - `note_number`: The identifier of the note you wish to delete.

- `:c note_number fields text_for_each_field`
  - **Description**: Change or update the fields of an existing note.
  - **Parameters**:
    - `note_number`: The identifier of the note you wish to modify.
    - `fields`: The specific fields to change. Available options:
      - `h` - Header (for headed notes)
      - `d` - Date (for dated notes) (yyyy/mm/dd)
      - `n` - Note text
      - `p` - Priority (h/m/l)
    - `text_for_each_field`: Provide the new content for each field, separated by spaces.

#### Settings

- `:s t/tm/p/d t/h/n/d t/tw/d n/o/d`
  - **Description**: Modify application settings.
  - **Parameters**:
    1. **Sorting**:
       - `t` - Sort by creation time
       - `tm` - Sort by modification time
       - `p` - Sort by priority
       - `d` - Make no changes
    2. **Priority Category**:
       - `t` - Date notes
       - `h` - Headed notes
       - `n` - No priority
       - `d` - Make no changes
    3. **Time Format**:
       - `tw` - 24-hour time format
       - `t` - 12-hour time format
       - `d` - Make no changes
    4. **Sorting for Time**:
       - `o` - Old first
       - `n` - New first
       - `d` - Make no changes

#### Help and Display

- `:help`, `:list`

  - **Description**: Display the help message with all available commands.

- `:update`
  - **Description**: Refresh the notes display to show the latest changes.

---

For more detailed information on each command, use the `:help` command within the application.

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
