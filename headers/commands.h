#include "classes.h"
#include "utils.h"

#ifndef COMMANDS_NOTES
#define COMMANDS_NOTES

// <--------------------- ENTER FUNCTIONS ---------------------->

void command_loop(std::vector<MyNote::note *> &notes, settings &config);
bool execute_command(const std::string &input,
                     std::vector<MyNote::note *> &notes, settings &config);

// <--------------------- BRANCH FUNCTIONS ---------------------->

void handle_wh_command(std::istringstream &iss,
                       std::vector<MyNote::note *> &notes);
void handle_wd_command(std::istringstream &iss,
                       std::vector<MyNote::note *> &notes);
void handle_delete_command(std::istringstream &iss,
                           std::vector<MyNote::note *> &notes);
void handle_change_command(std::istringstream &iss,
                           std::vector<MyNote::note *> &notes);
void handle_s_command(std::istringstream &iss, settings &config);

// <--------------------- DATA PARSING ---------------------->

priority_gen parse_priority(const std::string &str);
date *parse_date(const std::string &date_str);
std::string get_current_date();

// <--------------------- EXTRA FUNCTIONS ---------------------->

void display_notes(const std::vector<MyNote::note *> &notes,
                   const settings &config);
void print_help();
void pause_for_user();
std::string get_current_date();

// <--------------------- EXEPTIONS ---------------------->

// Custom exception for invalid commands or arguments
class InvalidCommandException : public std::exception {
public:
  InvalidCommandException(const std::string &message) : msg_(message) {}
  virtual const char *what() const noexcept override { return msg_.c_str(); }

private:
  std::string msg_;
};

// Custom exception for unexpected errors
class UnexpectedErrorException : public std::exception {
public:
  UnexpectedErrorException(const std::string &message)
      : msg_("An unexpected error occurred: " + message) {}
  virtual const char *what() const noexcept override { return msg_.c_str(); }

private:
  std::string msg_;
};

#endif // !COMMANDS_NOTES
