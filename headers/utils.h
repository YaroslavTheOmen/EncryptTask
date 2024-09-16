#ifndef SETTINGS
#define SETTINGS

struct settings {
  enum class sorting { time, time_m, priority_gen };
  enum class prioriry_category { time, headed, no };
  enum class time_type { twelve, twenty_four };
  sorting sorting_{sorting::time};
  prioriry_category prioriry_category_{prioriry_category::no};
  time_type time_type_{time_type::twenty_four};
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

#ifndef NOTE_FUNCTIONS
#define NOTE_FUNCTIONS

#include <string>
void print_string(const std::string &str);
std::string
time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time,
               const settings &config);

#endif // !NOTE_FUNCTIONS
