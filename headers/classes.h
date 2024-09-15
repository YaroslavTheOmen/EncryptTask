
#ifndef NOTES
#define NOTES

#include <chrono>
#include <string>

// BASE CLASS (ABSTRACT)
class note {
private:
  std::chrono::time_point<std::chrono::system_clock> time_;
  std::chrono::time_point<std::chrono::system_clock> time_m_;
  char priority_alphabatical_;
  std::string *note_;
  virtual void show_note() const = 0;

public:
  explicit note(std::string *note, char priority_alphabatical = 'A');
  virtual ~note();
  virtual void change(std::string *note, char priority_alphabatical = 'A');
  virtual void show() const;
};

// HEADED NOTE
class headed_note : public note {
private:
  std::string *header_;
  virtual void show_note() const override;

public:
  explicit headed_note(std::string *note, std::string *header,
                       char priority_alphabatical = 'A');
  virtual ~headed_note();
  virtual void change(std::string *note, std::string *header,
                      char priority_alphabatical = 'A');
  virtual void show() const override;
};

#include "structures.h"

// NOTE WITH DATE
class date_note : public note {
private:
  date *date_;
  virtual void show_note() const override;

public:
  explicit date_note(std::string *note, date *date,
                     char priority_alphabatical = 'A');
  virtual ~date_note();
  virtual void change(std::string *note, date *date,
                      char priority_alphabatical = 'A');
  virtual void show() const override;
};

#endif // !NOTES
