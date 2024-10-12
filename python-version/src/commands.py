import os
import shlex
from datetime import datetime
from typing import List, Optional
from classes import Note, Headed_note, Date_note, sorting
from note_utils import (
    settings,
    settings_sorting,
    settings_time_sort,
    settings_priority_category,
    settings_time_type,
    priority_gen,
    date,
)


# Custom Exceptions
class InvalidCommandException(Exception):
    pass


class UnexpectedErrorException(Exception):
    pass


# Command Loop
def command_loop(notes: List[Note], config: settings):
    while True:
        try:
            os.system("clear")  # or 'cls' on Windows
            sorting(notes, config)
            display_notes(notes, config)

            # Get user input
            input_str = input("> ")

            # Parse and execute the command
            if not execute_command(input_str, notes, config):
                os.system("clear")
                break  # Exit the loop if the command signals to quit

            # Clear the terminal
            os.system("clear")

            # Display updated notes
            sorting(notes, config)
            display_notes(notes, config)

        except InvalidCommandException as e:
            # Clear the terminal before displaying the error message
            os.system("clear")

            # Display error message
            print(f"Error: {e}")

            # Wait for the user to enter :quit or :exit
            while True:
                input_str = input("> ")

                if input_str in (":quit", ":exit", ""):
                    os.system("clear")
                    break
                else:
                    print("Please enter :quit or :exit to return to the main menu.")

            continue  # Go back to the start of the loop

        except Exception as e:
            # Clear the terminal before displaying the error message
            os.system("clear")

            # Display error message
            print(f"An unexpected error occurred: {e}")

            # Wait for the user to enter :quit or :exit
            while True:
                input_str = input("> ")

                if input_str in (":quit", ":exit", ""):
                    os.system("clear")
                    break
                else:
                    print("Please enter :quit or :exit to return to the main menu.")

            continue  # Go back to the start of the loop


# Execute Command
def execute_command(input_str: str, notes: List[Note], config: settings) -> bool:
    if not input_str.strip():
        raise InvalidCommandException("No command entered.")

    tokens = shlex.split(input_str)
    if not tokens:
        raise InvalidCommandException("No command entered.")
    command = tokens[0]
    args = tokens[1:]

    try:
        if command in (":quit", ":exit"):
            return False  # Exit the loop
        elif command == ":wh":
            handle_wh_command(args, notes)
        elif command == ":wd":
            handle_wd_command(args, notes)
        elif command == ":d":
            handle_delete_command(args, notes)
        elif command == ":c":
            handle_change_command(args, notes)
        elif command == ":s":
            handle_s_command(args, config)
        elif command in (":help", ":list"):
            print_help()
        elif command == ":update":
            # No action needed; the loop will refresh the display
            pass
        else:
            raise InvalidCommandException("Unknown command.")

    except InvalidCommandException as e:
        # Rethrow to be caught in the command loop
        raise
    except Exception as e:
        # Wrap any other exceptions in UnexpectedErrorException
        raise UnexpectedErrorException(str(e))

    return True  # Continue the loop


# Handler Functions
def handle_wh_command(args: List[str], notes: List[Note]):
    import shlex

    args_str = " ".join(args)
    tokens = shlex.split(args_str)

    if not tokens:
        raise InvalidCommandException("Note text must be enclosed in quotes.")

    note_text = tokens[0]
    header = "Untitled"
    priority_str = "m"

    idx = 1

    if idx < len(tokens):
        # Next token could be header or priority
        if tokens[idx].lower() not in ("h", "m", "l"):
            header = tokens[idx]
            idx += 1

    if idx < len(tokens):
        priority_str = tokens[idx]

    priority = parse_priority(priority_str)

    # Create and add the new headed note
    notes.append(Headed_note(note_text, header, priority))


def handle_wd_command(args: List[str], notes: List[Note]):
    import shlex

    args_str = " ".join(args)
    tokens = shlex.split(args_str)

    if not tokens:
        raise InvalidCommandException("Note text must be enclosed in quotes.")

    note_text = tokens[0]
    date_str = get_current_date()
    priority_str = "m"

    idx = 1

    if idx < len(tokens):
        # Next token could be date or priority
        if tokens[idx].lower() not in ("h", "m", "l"):
            date_str = tokens[idx]
            idx += 1

    if idx < len(tokens):
        priority_str = tokens[idx]

    priority = parse_priority(priority_str)

    note_date = parse_date(date_str)
    if not note_date:
        raise InvalidCommandException("Invalid date format. Use yyyy/mm/dd.")

    # Create and add the new date note
    notes.append(Date_note(note_text, note_date, priority))


def handle_delete_command(args: List[str], notes: List[Note]):
    if not args:
        raise InvalidCommandException("Invalid note number.")

    try:
        index = int(args[0])
    except ValueError:
        raise InvalidCommandException("Invalid note number.")

    if index <= 0 or index > len(notes):
        raise InvalidCommandException("Note number out of range.")

    del notes[index - 1]
    print(f"Note {index} has been deleted.")


def handle_change_command(args: List[str], notes: List[Note]):
    if len(args) < 2:
        raise InvalidCommandException(
            "Invalid command format. Usage: :c note_number fields"
        )

    try:
        index = int(args[0])
    except ValueError:
        raise InvalidCommandException("Invalid note number.")

    fields = args[1]
    field_args = args[2:]

    if index <= 0 or index > len(notes):
        raise InvalidCommandException("Note number out of range.")

    base_note = notes[index - 1]
    note_text: Optional[str] = None
    header_or_date_str: Optional[str] = None
    priority: Optional[priority_gen] = None
    change_note_text = False
    change_header_or_date = False
    change_priority = False

    import shlex

    # Reconstruct field_args into a string for shlex to parse
    field_args_str = " ".join(field_args)
    tokens = shlex.split(field_args_str)
    token_idx = 0

    for field in fields:
        if field == "n":
            # Change note text
            if token_idx >= len(tokens):
                raise InvalidCommandException("Missing note text.")
            note_text = tokens[token_idx]
            token_idx += 1
            change_note_text = True
        elif field in ("h", "d"):
            # Change header or date
            if token_idx >= len(tokens):
                raise InvalidCommandException("Missing header or date.")
            header_or_date_str = tokens[token_idx]
            token_idx += 1
            change_header_or_date = True
        elif field == "p":
            # Change priority
            if token_idx >= len(tokens):
                raise InvalidCommandException("Missing priority.")
            priority_str = tokens[token_idx]
            token_idx += 1
            priority = parse_priority(priority_str)
            change_priority = True
        else:
            raise InvalidCommandException(f"Invalid field '{field}'.")

    # Ensure header_or_date_str is not None before using
    if change_header_or_date:
        if header_or_date_str is None:
            raise InvalidCommandException("Header or date is missing.")
        # Add an assertion to help the type checker understand
        assert header_or_date_str is not None

    # Detect note type and perform changes accordingly
    if isinstance(base_note, Headed_note):
        # It's a headed note
        new_header = header_or_date_str if change_header_or_date else None
        base_note.change(note=note_text, header=new_header, priority=priority)
    elif isinstance(base_note, Date_note):
        # It's a date note
        new_date = None
        if change_header_or_date:
            # header_or_date_str is assured to be a str here
            new_date = parse_date(header_or_date_str)
            if not new_date:
                raise InvalidCommandException("Invalid date format. Use yyyy/mm/dd.")
        base_note.change(note=note_text, date=new_date, priority=priority)
    else:
        # Base note
        base_note.change(note=note_text, priority=priority)

    print(f"Note {index} has been updated.")


def handle_s_command(args: List[str], config: settings):
    if len(args) < 4:
        raise InvalidCommandException("Not enough arguments for :s command.")

    sort_by, priority_category, time_type, time_order = args[:4]

    # Validate and set sorting
    if sort_by == "t":
        config.sorting = settings_sorting.Time
    elif sort_by == "tm":
        config.sorting = settings_sorting.Time_m
    elif sort_by == "p":
        config.sorting = settings_sorting.Priority_gen
    elif sort_by == "d":
        # Keep current setting
        pass
    else:
        raise InvalidCommandException("Invalid sorting option.")

    # Validate and set priority category
    if priority_category == "t":
        config.priority_category = settings_priority_category.Time
    elif priority_category == "h":
        config.priority_category = settings_priority_category.Headed
    elif priority_category == "n":
        config.priority_category = settings_priority_category.No
    elif priority_category == "d":
        # Keep current setting
        pass
    else:
        raise InvalidCommandException("Invalid priority category option.")

    # Validate and set time type
    if time_type == "t":
        config.time_type = settings_time_type.Twelve
    elif time_type == "tw":
        config.time_type = settings_time_type.Twenty_four
    elif time_type == "d":
        # Keep current setting
        pass
    else:
        raise InvalidCommandException("Invalid time type option.")

    # Validate and set time sorting order
    if time_order == "n":
        config.time_sort = settings_time_sort.Newt
    elif time_order == "o":
        config.time_sort = settings_time_sort.Oldt
    elif time_order == "d":
        # Keep current setting
        pass
    elif time_order:
        raise InvalidCommandException("Invalid time order option.")

    print("Settings have been updated.")


# Data Parsing Functions
def parse_priority(priority_str: str) -> priority_gen:
    if priority_str.lower() == "h":
        return priority_gen.High
    elif priority_str.lower() == "m":
        return priority_gen.Middle
    elif priority_str.lower() == "l":
        return priority_gen.Low
    else:
        raise InvalidCommandException("Invalid priority value. Use h/m/l.")


# def parse_date(date_str: str) -> Optional[date]:
#    try:
#        parts = date_str.split("/")
#        if len(parts) != 3:
#            return None
#        year, month, day = map(int, parts)
#        return date(_day=day, _month=month, _year=year)
#    except ValueError:
#        return None  # Invalid date format


def parse_date(date_str: Optional[str]) -> Optional[date]:
    if date_str is None:
        return None
    try:
        parts = date_str.split("/")
        if len(parts) != 3:
            return None
        year, month, day = map(int, parts)
        return date(_day=day, _month=month, _year=year)
    except ValueError:
        return None  # Invalid date format


# Extra Functions
def display_notes(notes: List[Note], config: settings):
    index = 1
    for note in notes:
        print(f"Note {index}:")
        note.show(config)
        print()
        index += 1


def print_help():
    os.system("clear")
    print("Available commands:")
    print(":quit, :exit - Exit the program")
    print(':wh "note text" "header (default = Untitled)" h/m/l')
    print(':wd "note text" "date (default = today)" h/m/l')
    print(":d note_number - Delete a note")
    print(":c note_number fields - Change a note (fields: h/d, n, p)")
    print("   h - header (for headed notes)")
    print("   d - date (for date notes) (yyyy/mm/dd)")
    print("   n - note text")
    print("   p - priority (h/m/l)")
    print(":s t/tm/p/d t/h/n/d t/tw/d n/o/d - Change settings")
    print(":help, :list - Show this help message")
    print(":update - Refresh the notes display")
    # Wait for the user to enter :quit or :exit
    while True:
        input_str = input("> ")
        if input_str in (":quit", ":exit", ""):
            os.system("clear")
            return
        else:
            print("Please enter :quit or :exit to return to the main menu.")


def get_current_date() -> str:
    return datetime.now().strftime("%Y/%m/%d")


# Entry Point
# if __name__ == "__main__":
#    notes = []
#    config = settings(
#        sorting=settings_sorting.Time,
#        priority_category=settings_priority_category.No,
#        time_type=settings_time_type.Twenty_four,
#        time_sort=settings_time_sort.Newt,
#    )
#    command_loop(notes, config)
