# EncryptTask

A secure, console-based note-taking application written in C++ and Python, featuring AES-256 encryption using OpenSSL (C++) and the `cryptography` library (Python). The application allows users to create, view, edit, and delete notes securely. It prompts users for a password on startup and encrypts all stored data to ensure privacy.

By default, notes are stored in `project_dir/nlib/notes.dat`

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
  - [macOS](#macos)
  - [Linux (Debian/Ubuntu)](#linux-debianubuntu)
  - [Python](#python)
- [Installation (macOS / Linux)](#installation-macos--linux)
  - [C++ Application](#cpp-application)
  - [Python Application](#python-application)
- [Building the Application](#building-the-application)
  - [C++ Application](#building-the-cpp-application)
- [Running the Application (macOS / Linux)](#running-the-application-macos--linux)
  - [C++ Application](#running-the-cpp-application)
  - [Python Application](#running-the-python-application)
- [Usage](#usage)
  - [Commands](#commands)
- [Uninstallation](#uninstallation)
  - [macOS and Linux](#macos-and-linux)
  - [Python Application Uninstallation](#python-application-uninstallation)
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
- **Python Version Available**: Provides similar functionality with easy setup and extensibility.

## Prerequisites

- **C++17** or later.
- **CMake** version 3.10 or higher.
- **OpenSSL** development libraries.
- **Python 3.7** or later.
- **pip** package manager.

### macOS

- **Xcode Command Line Tools**:

  ```bash
  xcode-select --install

  •	Homebrew (if not already installed):
  ```

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

    •	OpenSSL:

brew install openssl

Linux (Debian/Ubuntu)

    •	Update package lists:

sudo apt-get update

    •	Install required packages:

sudo apt-get install build-essential cmake libssl-dev python3 python3-pip

Python

    •	Python 3.7 or later is required.
    •	pip should be installed alongside Python.
    •	Virtual Environment (Optional but Recommended):

It’s advisable to create a virtual environment to manage dependencies.

python3 -m venv venv
source venv/bin/activate # On Windows: venv\Scripts\activate

    •	Install Python Dependencies:

Navigate to the Python application directory and install required packages.

pip install -r requirements.txt

Note: Ensure that a requirements.txt file is present with the necessary dependencies, primarily the cryptography library.
Example requirements.txt:

cryptography

Installation (macOS / Linux)

C++ Application

    1.	Clone the Repository:

git clone https://github.com/YaroslavTheOmen/EncryptTask.git
cd EncryptTask

    2.	Create Build Directory:

mkdir build
cd build

    3.	Configure the Project:

cmake ..

    4.	Build the Project:

make

    •	The executable will be placed in the bin directory inside the project root (EncryptTask/bin/EncryptTask).

Python Application

    1.	Clone the Repository (if not already done):

git clone https://github.com/YaroslavTheOmen/EncryptTask.git
cd EncryptTask

    2.	Navigate to Python Application Directory:

Assuming the Python version is located in python-version/src/.

cd python-version/src

    3.	Create and Activate a Virtual Environment (Optional but Recommended):

python3 -m venv venv
source venv/bin/activate # On Windows: venv\Scripts\activate

    4.	Install Python Dependencies:

pip install -r requirements.txt

Note: Ensure that a requirements.txt file is present with the necessary dependencies, primarily the cryptography library.
Example requirements.txt:

cryptography

Building the Application

C++ Application

If you need to rebuild the C++ application after making changes:

    1.	Clean the Build Directory:

make clean

    2.	Rebuild:

make

Running the Application (macOS / Linux)

C++ Application

    1.	Open Terminal.
    2.	Navigate to the Executable:

cd path_to_EncryptTask/bin

    •	Replace path_to_EncryptTask with the actual path to the EncryptTask project directory.

    3.	Run the Application:

./EncryptTask

Python Application

    1.	Open Terminal.
    2.	Navigate to the Python Application Directory:

cd path_to_EncryptTask/python-version/src

    •	Replace path_to_EncryptTask with the actual path to the EncryptTask project directory.

    3.	Activate the Virtual Environment (if created):

source venv/bin/activate # On Windows: venv\Scripts\activate

    4.	Run the Application:

python main.py

    •	Ensure that main.py is the entry point of the Python application.

Usage

Upon running the application for the first time:

    •	Create a New Password: You’ll be prompted to create a new password. This password will be used to encrypt and decrypt your notes.
    •	Subsequent Runs: You’ll be prompted to enter your password to access your encrypted notes.

Commands

The application supports a variety of commands to manage your notes efficiently. Below is a list of available commands along with their descriptions:

Exit Commands

    •	:quit, :exit
    •	Description: Exit the program.

Adding Notes

    •	:wh "note text" "header (default = Untitled)" h/m/l
    •	Description: Create a new headed note with the specified text, header, and priority.
    •	Parameters:
    •	"note text": The content of the note.
    •	"header": The header/title of the note. Defaults to “Untitled” if not provided.
    •	h/m/l: Priority level (high, medium, low).
    •	:wd "note text" "date (default = today)" h/m/l
    •	Description: Create a new dated note with the specified text, date, and priority.
    •	Parameters:
    •	"note text": The content of the note.
    •	"date": The date for the note (yyyy/mm/dd). Defaults to today’s date if not provided.
    •	h/m/l: Priority level (high, medium, low).

Managing Notes

    •	:d note_number
    •	Description: Delete a note by its number.
    •	Parameters:
    •	note_number: The identifier of the note you wish to delete.
    •	:c note_number fields text_for_each_field
    •	Description: Change or update the fields of an existing note.
    •	Parameters:
    •	note_number: The identifier of the note you wish to modify.
    •	fields: The specific fields to change. Available options:
    •	h - Header (for headed notes)
    •	d - Date (for dated notes) (yyyy/mm/dd)
    •	n - Note text
    •	p - Priority (h/m/l)
    •	text_for_each_field: Provide the new content for each field, separated by spaces.

Settings

    •	:s t/tm/p/d t/h/n/d t/tw/d n/o/d
    •	Description: Modify application settings.
    •	Parameters:
    1.	Sorting:
    •	t - Sort by creation time
    •	tm - Sort by modification time
    •	p - Sort by priority
    •	d - Make no changes
    2.	Priority Category:
    •	t - Date notes
    •	h - Headed notes
    •	n - No priority
    •	d - Make no changes
    3.	Time Format:
    •	tw - 24-hour time format
    •	t - 12-hour time format
    •	d - Make no changes
    4.	Sorting for Time:
    •	o - Old first
    •	n - New first
    •	d - Make no changes

Help and Display

    •	:help, :list
    •	Description: Display the help message with all available commands.
    •	:update
    •	Description: Refresh the notes display to show the latest changes.

For more detailed information on each command, use the :help command within the application.

Uninstallation

macOS and Linux

C++ Application

    1.	Delete the Build and Binary Files:

rm -rf path_to_EncryptTask/build
rm -rf path_to_EncryptTask/bin

    •	Replace path_to_EncryptTask with the actual path to the EncryptTask project directory.

    2.	Remove Configuration and Data Files:
    •	Delete notes.dat or any other data files stored by the application in the project directory or your home directory.

Python Application Uninstallation

    1.	Deactivate the Virtual Environment (if activated):

deactivate

    2.	Delete the Virtual Environment:

rm -rf path_to_EncryptTask/python-version/src/venv

    •	Replace path_to_EncryptTask with the actual path to the EncryptTask project directory.

    3.	Remove Python Application Files:

rm -rf path_to_EncryptTask/python-version/src

    •	Ensure you back up any necessary data before deletion.

    4.	Remove Configuration and Data Files:
    •	Delete notes.dat or any other data files stored by the application in the project directory or your home directory.

Python Application Uninstallation

If you installed the Python application using additional package managers or installed system-wide packages, ensure to remove them accordingly. However, if you used a virtual environment as recommended, deleting the venv directory suffices.

License

This project is licensed under the MIT License - see the LICENSE file for details.

Acknowledgements

    •	OpenSSL: Used for encryption and decryption (C++).
    •	cryptography: Used for encryption and decryption (Python).
    •	CMake: Build system generator.
    •	Contributors: Thanks to everyone who has contributed to this project.

Note: This application is a console-based program and does not have a graphical user interface. It must be run from a terminal or command prompt. Ensure that you have the necessary permissions and that all dependencies are installed.

If you encounter any issues or have questions, please feel free to open an issue on the project’s GitHub repository.

Additional Information for Python Users:

    •	Data Compatibility: Both the C++ and Python applications share the same notes.dat file. Ensure that both applications use the same password to access and modify the notes.
    •	Concurrency: Avoid running both applications simultaneously to prevent data corruption.
    •	Backup: Regularly back up your notes.dat file to prevent data loss.
