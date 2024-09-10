#ifndef NOTES
#define NOTES

// BASE CLASS (ABSTRACT)
#include <chrono>
#include <string>
class note {
private:
  char *time_[20];
  std::string *note_;
  char *priority_;

public:
  explicit note(std::chrono::time_point<std::chrono::system_clock> *time,
                std::string *note, char *priority);
  ~note();
  virtual void chage() = 0;
  virtual void write() = 0;
};

// HEADED NOTE
class headed_note : public note {
private:
  char *header_[30];

public:
  explicit headed_note(std::chrono::time_point<std::chrono::system_clock> *time,
                       std::string note, char *priority, std::string header);
  virtual void chage();
  virtual void write();
};

// NOTE WITH DATE
class date_note : public note {
private:
  // std::string
};

#endif // !NOTES
