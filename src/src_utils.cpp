#include "../headers/utils.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

// <--------------------- SERIALIZATION ---------------------->

void settings::serialize(std::ostream &out) const {
  uint8_t sorting_value = static_cast<uint8_t>(sorting_);
  uint8_t priority_category_value = static_cast<uint8_t>(prioriry_category_);
  uint8_t time_type_value = static_cast<uint8_t>(time_type_);
  uint8_t time_sort_value = static_cast<uint8_t>(time_sort_);

  out.write(reinterpret_cast<const char *>(&sorting_value),
            sizeof(sorting_value));
  out.write(reinterpret_cast<const char *>(&priority_category_value),
            sizeof(priority_category_value));
  out.write(reinterpret_cast<const char *>(&time_type_value),
            sizeof(time_type_value));
  out.write(reinterpret_cast<const char *>(&time_sort_value),
            sizeof(time_sort_value));
}

settings settings::deserialize(std::istream &in) {
  settings config;
  uint8_t sorting_value, priority_category_value, time_type_value,
      time_sort_value;

  in.read(reinterpret_cast<char *>(&sorting_value), sizeof(sorting_value));
  in.read(reinterpret_cast<char *>(&priority_category_value),
          sizeof(priority_category_value));
  in.read(reinterpret_cast<char *>(&time_type_value), sizeof(time_type_value));
  in.read(reinterpret_cast<char *>(&time_sort_value), sizeof(time_sort_value));

  config.sorting_ = static_cast<sorting>(sorting_value);
  config.prioriry_category_ =
      static_cast<prioriry_category>(priority_category_value);
  config.time_type_ = static_cast<time_type>(time_type_value);
  config.time_sort_ = static_cast<time_sort>(time_sort_value);

  return config;
}

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
