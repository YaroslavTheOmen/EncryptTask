#include "../headers/commands.h"
#include "../headers/utils.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

// <--------------------- ENTER FUNCTIONS ---------------------->

void command_loop(std::vector<MyNote::note *> &notes, settings &config) {
  while (true) {
    try {

      system("clear"); // On Windows, use "cls"
      sorting(notes, config);
      display_notes(notes, config);

      // Get user input
      std::string input;
      std::cout << "> ";
      std::getline(std::cin, input);

      // Parse and execute the command
      if (!execute_command(input, notes, config)) {
        break; // Exit the loop if the command signals to quit
      }

      // Clear the terminal
      system("clear"); // On Windows, use "cls"

      // Display updated notes
      sorting(notes, config);
      display_notes(notes, config);

    } catch (const InvalidCommandException &e) {
      // Clear the terminal before displaying the error message
      system("clear"); // On Windows, use "cls"

      // Display error message
      std::cout << "Error: " << e.what() << std::endl;

      // Wait for the user to enter :quit or :exit
      std::string input;
      while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == ":quit" || input == ":exit") {
          // Clear the terminal again before returning to the main loop
          system("clear"); // On Windows, use "cls"
          break;
        } else {
          std::cout << "Please enter :quit or :exit to return to the main menu."
                    << std::endl;
        }
      }

      continue; // Go back to the start of the loop

    } catch (const std::exception &e) {
      // Clear the terminal before displaying the error message
      system("clear"); // On Windows, use "cls"

      // Display error message
      std::cout << "An unexpected error occurred: " << e.what() << std::endl;

      // Wait for the user to enter :quit or :exit
      std::string input;
      while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == ":quit" || input == ":exit") {
          // Clear the terminal again before returning to the main loop
          system("clear"); // On Windows, use "cls"
          break;
        } else {
          std::cout << "Please enter :quit or :exit to return to the main menu."
                    << std::endl;
        }
      }

      continue; // Go back to the start of the loop
    }
  }
}

bool execute_command(const std::string &input,
                     std::vector<MyNote::note *> &notes, settings &config) {
  if (input.empty()) {
    throw InvalidCommandException("No command entered.");
  }

  std::istringstream iss(input);
  std::string command;
  iss >> command;

  try {
    if (command == ":quit" || command == ":exit") {
      return false; // Exit the loop
    } else if (command == ":wh") {
      handle_wh_command(iss, notes);
    } else if (command == ":wd") {
      handle_wd_command(iss, notes);
    } else if (command == ":d") {
      handle_delete_command(iss, notes);
    } else if (command == ":c") {
      handle_change_command(iss, notes);
    } else if (command == ":s") {
      handle_s_command(iss, config);
    } else if (command == ":help" || command == ":list") {
      print_help();
    } else if (command == ":update") {
      // No action needed; the loop will refresh the display
    } else {
      throw InvalidCommandException("Unknown command.");
    }
  } catch (const InvalidCommandException &e) {
    // Rethrow to be caught in the command loop
    throw;
  } catch (const std::exception &e) {
    // Wrap any other exceptions in UnexpectedErrorException
    throw UnexpectedErrorException(e.what());
  }

  return true; // Continue the loop
}

// <--------------------- BRANCH FUNCTIONS ---------------------->

void handle_wh_command(std::istringstream &iss,
                       std::vector<MyNote::note *> &notes) {
  std::string note_text, header = "Untitled", priority_str = "m";

  // Get note text
  iss >> std::ws;
  if (iss.peek() == '"') {
    iss.get(); // Remove opening quote
    std::getline(iss, note_text, '"');
  } else {
    throw InvalidCommandException("Note text must be enclosed in quotes.");
  }

  // Get header if provided
  iss >> std::ws;
  if (iss.peek() == '"') {
    iss.get(); // Remove opening quote
    std::getline(iss, header, '"');
  }

  // Get priority if provided
  iss >> priority_str;

  priority_gen priority = parse_priority(priority_str);

  // Create and add the new headed note
  notes.push_back(new MyNote::headed_note(new std::string(note_text),
                                          new std::string(header), priority));
}

void handle_wd_command(std::istringstream &iss,
                       std::vector<MyNote::note *> &notes) {
  std::string note_text, date_str = get_current_date(), priority_str = "m";

  // Get note text
  iss >> std::ws;
  if (iss.peek() == '"') {
    iss.get(); // Remove opening quote
    std::getline(iss, note_text, '"');
  } else {
    throw InvalidCommandException("Note text must be enclosed in quotes.");
  }

  // Get date if provided
  iss >> std::ws;
  if (iss.peek() == '"') {
    iss.get(); // Remove opening quote
    std::getline(iss, date_str, '"');
  }

  // Get priority if provided
  iss >> priority_str;

  priority_gen priority = parse_priority(priority_str);

  date *note_date = parse_date(date_str);
  if (!note_date) {
    throw InvalidCommandException("Invalid date format. Use yyyy/mm/dd.");
  }

  // Create and add the new date note
  notes.push_back(
      new MyNote::date_note(new std::string(note_text), note_date, priority));
}

void handle_delete_command(std::istringstream &iss,
                           std::vector<MyNote::note *> &notes) {
  int index;
  if (!(iss >> index)) {
    throw InvalidCommandException("Invalid note number.");
  }

  if (index <= 0 || index > notes.size()) {
    throw InvalidCommandException("Note number out of range.");
  }

  delete notes[index - 1];
  notes.erase(notes.begin() + index - 1);

  std::cout << "Note " << index << " has been deleted." << std::endl;
}

void handle_change_command(std::istringstream &iss,
                           std::vector<MyNote::note *> &notes) {
  int index;
  std::string fields;
  if (!(iss >> index >> fields)) {
    throw InvalidCommandException(
        "Invalid command format. Usage: :c note_number fields");
  }

  if (index <= 0 || index > notes.size()) {
    throw InvalidCommandException("Note number out of range.");
  }

  MyNote::note *base_note = notes[index - 1];
  std::string note_text, header_or_date_str, priority_str;
  std::optional<priority_gen> priority;
  bool change_note_text = false, change_header_or_date = false,
       change_priority = false;

  // Parse the fields and corresponding values
  for (char field : fields) {
    if (field == 'n') {
      // Change note text
      iss >> std::ws;
      if (iss.peek() == '"') {
        iss.get(); // Remove quote
        std::getline(iss, note_text, '"');
        change_note_text = true;
      } else {
        throw InvalidCommandException("Note text must be enclosed in quotes.");
      }
    } else if (field == 'h' || field == 'd') {
      // Change header or date
      iss >> std::ws;
      if (iss.peek() == '"') {
        iss.get(); // Remove quote
        std::getline(iss, header_or_date_str, '"');
        change_header_or_date = true;
      } else {
        throw InvalidCommandException(
            "Header or date must be enclosed in quotes.");
      }
    } else if (field == 'p') {
      // Change priority
      iss >> priority_str;
      priority = parse_priority(priority_str);
      change_priority = true;
    } else {
      throw InvalidCommandException(std::string("Invalid field '") + field +
                                    "'.");
    }
  }

  // Prepare parameters for the change method
  const std::string *new_note_text = change_note_text ? &note_text : nullptr;
  std::optional<priority_gen> new_priority =
      change_priority ? priority : std::nullopt;

  // Detect note type and perform changes accordingly
  if (MyNote::headed_note *note =
          dynamic_cast<MyNote::headed_note *>(base_note)) {
    // It's a headed note
    const std::string *new_header =
        change_header_or_date ? &header_or_date_str : nullptr;
    note->change(new_note_text, new_header, new_priority);
  } else if (MyNote::date_note *note =
                 dynamic_cast<MyNote::date_note *>(base_note)) {
    // It's a date note
    date *new_date = nullptr;
    if (change_header_or_date) {
      new_date = parse_date(header_or_date_str);
      if (!new_date) {
        throw InvalidCommandException("Invalid date format. Use yyyy/mm/dd.");
      }
    }
    note->change(new_note_text, new_date, new_priority);
  } else {
    // Base note
    base_note->change(new_note_text, new_priority);
  }

  std::cout << "Note " << index << " has been updated." << std::endl;
}

void handle_s_command(std::istringstream &iss, settings &config) {
  std::string sort_by, priority_category, time_type;
  iss >> sort_by >> priority_category >> time_type;

  // Validate and set sorting
  if (sort_by == "t")
    config.sorting_ = settings::sorting::time;
  else if (sort_by == "tm")
    config.sorting_ = settings::sorting::time_m;
  else if (sort_by == "p")
    config.sorting_ = settings::sorting::priority_gen;
  else if (sort_by != "d")
    throw InvalidCommandException("Invalid sorting option.");

  // Validate and set priority category
  if (priority_category == "t")
    config.prioriry_category_ = settings::prioriry_category::time;
  else if (priority_category == "h")
    config.prioriry_category_ = settings::prioriry_category::headed;
  else if (priority_category == "n")
    config.prioriry_category_ = settings::prioriry_category::no;
  else if (priority_category != "d")
    throw InvalidCommandException("Invalid priority category option.");

  // Validate and set time type
  if (time_type == "t")
    config.time_type_ = settings::time_type::twelve;
  else if (time_type == "tw")
    config.time_type_ = settings::time_type::twenty_four;
  else if (time_type != "d")
    throw InvalidCommandException("Invalid time type option.");

  std::cout << "Settings have been updated." << std::endl;
}

// <--------------------- DATA PARSING ---------------------->

priority_gen parse_priority(const std::string &str) {
  if (str == "h" || str == "H")
    return priority_gen::High;
  if (str == "m" || str == "M")
    return priority_gen::Middle;
  if (str == "l" || str == "L")
    return priority_gen::Low;
  throw InvalidCommandException("Invalid priority value. Use h/m/l.");
}

date *parse_date(const std::string &date_str) {
  int year, month, day;
  char sep1, sep2;
  std::istringstream iss(date_str);
  if (iss >> year >> sep1 >> month >> sep2 >> day && sep1 == '/' &&
      sep2 == '/') {
    return new date(year, month, day);
  }
  return nullptr; // Invalid date format
}

// <--------------------- EXTRA FUNCTIONS ---------------------->

void display_notes(const std::vector<MyNote::note *> &notes,
                   const settings &config) {
  int index = 1;
  for (const auto &note : notes) {
    std::cout << "Note " << index++ << ":" << std::endl;
    note->show(config);
    std::cout << std::endl;
  }
}

void pause_for_user() {
  std::cout << "Press Enter to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void print_help() {
  // Clear the terminal
  system("clear"); // On Windows, use "cls"

  // Display help message
  std::cout << "Available commands:" << std::endl;
  std::cout << ":quit, :exit - Exit the program" << std::endl;
  std::cout << ":wh \"note text\" \"header (default = Untitled)\" h/m/l"
            << std::endl;
  std::cout << ":wd \"note text\" \"date (default = today)\" h/m/l"
            << std::endl;
  std::cout << ":d note_number - Delete a note" << std::endl;
  std::cout << ":c note_number fields - Change a note (fields: h/d, n, p)"
            << std::endl;
  std::cout << "   h - header (for headed notes)" << std::endl;
  std::cout << "   d - date (for date notes)" << std::endl;
  std::cout << "   n - note text" << std::endl;
  std::cout << "   p - priority (h/m/l)" << std::endl;
  std::cout << ":s t/tm/p/d t/h/n/d t/tw/d - Change settings" << std::endl;
  std::cout << ":help, :list - Show this help message" << std::endl;
  std::cout << ":update - Refresh the notes display" << std::endl;

  // Wait for the user to enter :quit or :exit
  std::string input;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, input);

    if (input == ":quit" || input == ":exit") {
      // Clear the terminal again before returning to the main loop
      system("clear"); // On Windows, use "cls"
      return;
    } else {
      std::cout << "Please enter :quit or :exit to return to the main menu."
                << std::endl;
    }
  }
}

std::string get_current_date() {
  auto now = std::chrono::system_clock::now();
  auto time_c = std::chrono::system_clock::to_time_t(now);
  std::tm *time_info = std::localtime(&time_c);
  char buffer[11];
  strftime(buffer, sizeof(buffer), "%Y/%m/%d", time_info);
  return std::string(buffer);
}
