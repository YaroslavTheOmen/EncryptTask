#ifndef SETTINGS
#define SETTINGS

struct settings {
  enum class sorting { priority, category, creation_time };
  enum class prioriry_category { time, headed };
  enum class date_time { twelve, twenty_four };
};

#endif // !SETTINGS

#ifndef TIME
#define TIME
struct date {
  int day;
  int month;
  int year;
};

#endif // !TIME
