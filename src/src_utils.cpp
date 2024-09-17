#include "../headers/utils.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

// <--------------------- DATE STRUCT ---------------------->

void date::show() const {
  std::cout << year_ << "." << month_ << "." << day_ << std::endl;
}

date::date(int year, int month, int day)
    : day_(day), year_(year), month_(month) {}

// <--------------------- TIME FUNCTIONS ---------------------->

void print_string(const std::string &str) {
  std::istringstream stream(str);
  std::string word;
  std::string current_line;

  while (stream >> word) {
    if (current_line.length() + word.length() + 1 > 45) {
      std::cout << current_line << std::endl;
      current_line = word;
    } else {
      if (!current_line.empty()) {
        current_line += " ";
      }
      current_line += word;
    }
  }

  if (!current_line.empty()) {
    std::cout << current_line << std::endl;
  }
}

std::string
time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time,
               const settings &config) {
  std::time_t time_c = std::chrono::system_clock::to_time_t(time);
  std::tm *time_info = std::localtime(&time_c);
  char buffer[80];

  // Choose format based on the time_type in the settings struct
  if (config.time_type_ == settings::time_type::twenty_four) {
    // 24-hour format
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
  } else if (config.time_type_ == settings::time_type::twelve) {
    // 12-hour format
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M:%S %p", time_info);
  }

  return std::string(buffer);
}
