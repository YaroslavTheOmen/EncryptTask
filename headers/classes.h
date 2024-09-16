#ifndef NOTES
#define NOTES

#include "functions.h"
#include <chrono>
#include <string>

// BASE CLASS (ABSTRACT)
class note {
private:
  std::chrono::time_point<std::chrono::system_clock> time_;
  std::chrono::time_point<std::chrono::system_clock> time_m_;
  priority_gen priority_gen_;
  std::string *note_;

protected:
  virtual void show_note() const = 0;

public:
  explicit note(std::string *note,
                priority_gen priority_gen = priority_gen::High);
  virtual ~note();
  virtual void change(std::string *note,
                      priority_gen priority_gen = priority_gen::High);
  virtual void show() const;
};

// HEADED NOTE
class headed_note : public note {
private:
  std::string *header_;

protected:
  virtual void show_note() const override;

public:
  explicit headed_note(std::string *note, std::string *header,
                       priority_gen priority_gen = priority_gen::High);
  virtual ~headed_note();
  virtual void change(std::string *note, std::string *header,
                      priority_gen priority_gen = priority_gen::High);
  virtual void show() const override;
};

// NOTE WITH DATE
class date_note : public note {
private:
  date *date_;

protected:
  virtual void show_note() const override;

public:
  explicit date_note(std::string *note, date *date,
                     priority_gen priority_gen = priority_gen::High);
  virtual ~date_note();
  virtual void change(std::string *note, date *date,
                      priority_gen priority_gen = priority_gen::High);
  virtual void show() const override;
};

#endif // !NOTES
