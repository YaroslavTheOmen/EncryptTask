#ifndef UTILS_H_
#define UTILS_H_

// <--------------------- SETTINGS AND ENUMS ---------------------->

struct settings {
  enum class sorting { time, time_m, priority_gen };
  enum class prioriry_category { time, headed, no };
  enum class time_type { twelve, twenty_four };
  sorting sorting_{sorting::time};
  prioriry_category prioriry_category_{prioriry_category::no};
  time_type time_type_{time_type::twenty_four};
};

enum class priority_gen { High, Middle, Low };

// <--------------------- DATE STRUCT ---------------------->

struct date {
  int day_;
  int month_;
  int year_;
  void show() const;
  date(int year, int month, int day);
  date(date &date) = default;
};

// <--------------------- TIME FUNCTIONS ---------------------->

#include <string>
void print_string(const std::string &str);
std::string
time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time,
               const settings &config);

#endif // !UTILS_H_