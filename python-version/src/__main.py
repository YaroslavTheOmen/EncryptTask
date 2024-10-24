import os
import getpass
from classes import Note
from commands import command_loop
from note_utils import (
    settings,
    settings_sorting,
    settings_priority_category,
    settings_time_type,
    settings_time_sort,
)
import encryption


def main():
    try:
        # Clear the terminal screen
        os.system("clear")

        # Determine the executable's directory
        exe_dir = os.path.dirname(os.path.abspath(__file__))

        # Construct the path to ../nlib/ relative to exe_dir
        nlib_path = os.path.normpath(os.path.join(exe_dir, "../../nlib"))

        # Ensure the ../nlib/ directory exists
        if not os.path.exists(nlib_path):
            os.makedirs(nlib_path)

        # Construct the full path to notes.dat
        filename = os.path.abspath(os.path.join(nlib_path, "notes.dat"))

        # Initialize variables
        notes = []
        config = settings(
            sorting=settings_sorting.Time,
            priority_category=settings_priority_category.No,
            time_type=settings_time_type.Twenty_four,
            time_sort=settings_time_sort.Newt,
        )
        password = ""

        # Check if file exists
        file_exists = os.path.isfile(filename)

        # Prompt for password
        if not file_exists:
            # File does not exist; ask user to create a new password
            password = getpass.getpass(
                "No existing data found. Please create a new password: "
            )
            if not password:
                print("Password cannot be empty. Exiting.")
                return
            print("New password set.")
        else:
            # File exists; ask user to enter the password
            password = getpass.getpass("Please enter your password: ")
            if not password:
                print("Password cannot be empty. Exiting.")
                return

            # Attempt to load data from the file
            try:
                if not encryption.decrypt_data(notes, config, filename, password):
                    print(
                        "Failed to decrypt data. Incorrect password or corrupted file."
                    )
                    return
                print("Data loaded successfully.")
            except Exception as e:
                print(f"Failed to load data: {e}")
                print("Starting with empty notes and default settings.")

        # Run the command loop
        try:
            command_loop(notes, config)
        except Exception as e:
            print(f"A fatal error occurred: {e}")

        # Save data before exiting
        try:
            if not encryption.encrypt_data(notes, config, filename, password):
                print("Failed to encrypt data.")
            else:
                print("Data saved successfully.")
        except Exception as e:
            print(f"Failed to save data: {e}")

    except Exception as ex:
        print(f"Initialization error: {ex}")
        return


if __name__ == "__main__":
    main()
