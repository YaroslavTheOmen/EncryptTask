#ifndef SETTINGS
#define SETTINGS

struct settings {
  enum class sorting { priority, category, creation_time };
  enum class priotiry_category { time, headed };
  enum class date_time { twelve, twenty_four };
};

struct date {};

#endif // !SETTINGS
