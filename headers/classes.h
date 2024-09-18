#ifndef CLASSES_H_
#define CLASSES_H_

#include "utils.h"
#include <chrono>
#include <optional>
#include <string>
#include <vector>

// <--------------------- FOR SERIALIZATION ---------------------->

enum class NoteType : uint8_t { HeadedNote = 1, DateNote = 2 };

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
                         settings::sorting sort_by,
                         settings::time_sort time_order);

protected:
  virtual void show_note() const = 0;
  void serialize_base(std::ostream &out) const;
  void deserialize_base(std::istream &in);

public:
  explicit note(std::string *note,
                priority_gen priority_gen = priority_gen::High);
  virtual ~note();
  virtual void
  change(const std::string *note = nullptr,
         const std::optional<priority_gen> &priority = std::nullopt);
  virtual void show(const settings config) const;
  virtual void serialize(std::ostream &out) const = 0;
  static note *deserialize(std::istream &in);
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
  virtual void
  change(const std::string *note = nullptr, const std::string *header = nullptr,
         const std::optional<priority_gen> &priority = std::nullopt);
  virtual void show(const settings config) const override;

  virtual void serialize(std::ostream &out) const override;
  static headed_note *deserialize(std::istream &in);
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
  virtual void
  change(const std::string *note = nullptr, date *date = nullptr,
         const std::optional<priority_gen> &priority = std::nullopt);
  virtual void show(const settings config) const override;
  virtual void serialize(std::ostream &out) const override;
  static date_note *deserialize(std::istream &in);
};

// <--------------------- SORING FUNCTIONS ---------------------->
// <--------------------- THAT WORK WITH SETTINGS ---------------------->

#include "utils.h"
#include <vector>

void sorting(std::vector<MyNote::note *> &vect, settings config);
void sort_notes(std::vector<MyNote::note *>::iterator begin,
                std::vector<MyNote::note *>::iterator end,
                settings::sorting sort_by, settings::time_sort time_order);
} // namespace MyNote
#endif // !CLASSES_H_
