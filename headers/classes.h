#ifndef CLASSES_H_
#define CLASSES_H_

#include "utils.h"
#include <chrono>
#include <string>
#include <vector>

// <--------------------- BASE CLASS NOTE (ABSTRACT) ---------------------->

namespace MyNote {
class note {
private:
  std::chrono::time_point<std::chrono::system_clock> time_;
  std::chrono::time_point<std::chrono::system_clock> time_m_;
  priority_gen priority_gen_;
  std::string *note_;
  friend void sorting(std::vector<MyNote::note *> &vect, settings config);
  friend void sort_notes(std::vector<MyNote::note *>::iterator begin,
                         std::vector<MyNote::note *>::iterator end,
                         settings::sorting sort_by);

protected:
  virtual void show_note() const = 0;

public:
  explicit note(std::string *note,
                priority_gen priority_gen = priority_gen::High);
  virtual ~note();
  virtual void change(std::string *note,
                      priority_gen priority_gen = priority_gen::High);
  virtual void show(const settings config) const;
};

// <--------------------- HEADED NOTE ---------------------->

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
  virtual void show(const settings config) const override;
};

// <--------------------- DATE NOTE ---------------------->

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
  virtual void show(const settings config) const override;
};

// <--------------------- SORING FUNCTIONS ---------------------->
// <--------------------- THAT WORK WITH SETTINGS ---------------------->

#include "utils.h"
#include <vector>

void sorting(std::vector<MyNote::note *> &vect, settings config);
void sort_notes(std::vector<MyNote::note *>::iterator begin,
                std::vector<MyNote::note *>::iterator end,
                settings::sorting sort_by);

} // namespace MyNote
#endif // !CLASSES_H_
