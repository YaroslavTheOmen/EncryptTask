#ifndef NOTE_FUNCTIONS
#define NOTE_FUNCTIONS

#include <string>
void print_string(const std::string &str);
std::string
time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time);

#endif // !NOTE_FUNCTIONS

#ifndef SETTINGS
#define SETTINGS

struct settings {
  enum class sorting { priority, category, creation_time };
  enum class prioriry_category { time, headed };
  enum class date_time { twelve, twenty_four };
};

enum class priority_gen { High, Middle, Low };

#endif // !SETTINGS

#ifndef TIME
#define TIME
struct date {
  int day_;
  int month_;
  int year_;
  void show() const;
  date(int year, int month, int day);
  date(date &date) = default;
};

#endif // !TIME
