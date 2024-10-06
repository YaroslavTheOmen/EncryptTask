#include "../headers/classes.h"
#include "../headers/utils.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <optional>
#include <string>

// <--------------------- FOR SERIALIZATION BASE ---------------------->

MyNote::note *MyNote::note::deserialize(std::istream &in) {
  NoteType type;
  in.read(reinterpret_cast<char *>(&type), sizeof(type));

  switch (type) {
  case NoteType::HeadedNote:
    return headed_note::deserialize(in);
  case NoteType::DateNote:
    return date_note::deserialize(in);
  default:
    throw std::runtime_error("Unknown note type.");
  }
}

void MyNote::note::serialize_base(std::ostream &out) const {
  // Serialize time_
  auto time = time_.time_since_epoch().count();
  out.write(reinterpret_cast<const char *>(&time), sizeof(time));

  // Serialize time_m_
  auto time_m = time_m_.time_since_epoch().count();
  out.write(reinterpret_cast<const char *>(&time_m), sizeof(time_m));

  // Serialize priority_gen_
  auto priority = static_cast<uint8_t>(priority_gen_);
  out.write(reinterpret_cast<const char *>(&priority), sizeof(priority));

  // Serialize note_
  size_t note_length = note_->size();
  out.write(reinterpret_cast<const char *>(&note_length), sizeof(note_length));
  out.write(note_->data(), note_length);
}

void MyNote::note::deserialize_base(std::istream &in) {
  // Deserialize time_
  decltype(time_.time_since_epoch().count()) time_count;
  in.read(reinterpret_cast<char *>(&time_count), sizeof(time_count));
  time_ = std::chrono::system_clock::time_point(
      std::chrono::system_clock::duration(time_count));

  // Deserialize time_m_
  decltype(time_m_.time_since_epoch().count()) time_m_count;
  in.read(reinterpret_cast<char *>(&time_m_count), sizeof(time_m_count));
  time_m_ = std::chrono::system_clock::time_point(
      std::chrono::system_clock::duration(time_m_count));

  // Deserialize priority_gen_
  uint8_t priority_value;
  in.read(reinterpret_cast<char *>(&priority_value), sizeof(priority_value));
  priority_gen_ = static_cast<priority_gen>(priority_value);

  // Deserialize note_
  size_t note_length;
  in.read(reinterpret_cast<char *>(&note_length), sizeof(note_length));
  std::string note_text(note_length, '\0');
  in.read(&note_text[0], note_length);

  delete note_; // Clean up existing note_
  note_ = new std::string(note_text);
}

// <--------------------- FOR SERIALIZATION HEADED ---------------------->

void MyNote::headed_note::serialize(std::ostream &out) const {
  // Write type identifier
  NoteType type = NoteType::HeadedNote;
  out.write(reinterpret_cast<const char *>(&type), sizeof(type));

  // Serialize base class data
  serialize_base(out);

  // Serialize header_
  size_t header_length = header_->size();
  out.write(reinterpret_cast<const char *>(&header_length),
            sizeof(header_length));
  out.write(header_->data(), header_length);
}

MyNote::headed_note *MyNote::headed_note::deserialize(std::istream &in) {
  // Create a dummy headed_note object
  headed_note *new_note = new headed_note(new std::string(), new std::string());

  // Deserialize base class data
  new_note->deserialize_base(in);

  // Deserialize header_
  size_t header_length;
  in.read(reinterpret_cast<char *>(&header_length), sizeof(header_length));
  std::string header_text(header_length, '\0');
  in.read(&header_text[0], header_length);

  delete new_note->header_; // Clean up existing header_
  new_note->header_ = new std::string(header_text);

  return new_note;
}
// <--------------------- FOR SERIALIZATION DATED ---------------------->

void MyNote::date_note::serialize(std::ostream &out) const {
  // Write type identifier
  NoteType type = NoteType::DateNote;
  out.write(reinterpret_cast<const char *>(&type), sizeof(type));

  // Serialize base class data
  serialize_base(out);

  // Serialize date_
  out.write(reinterpret_cast<const char *>(&date_->year_),
            sizeof(date_->year_));
  out.write(reinterpret_cast<const char *>(&date_->month_),
            sizeof(date_->month_));
  out.write(reinterpret_cast<const char *>(&date_->day_), sizeof(date_->day_));
}

MyNote::date_note *MyNote::date_note::deserialize(std::istream &in) {
  // Create a dummy date_note object
  date_note *new_note = new date_note(new std::string(), new date(0, 0, 0));

  // Deserialize base class data
  new_note->deserialize_base(in);

  // Deserialize date_
  int year, month, day;
  in.read(reinterpret_cast<char *>(&year), sizeof(year));
  in.read(reinterpret_cast<char *>(&month), sizeof(month));
  in.read(reinterpret_cast<char *>(&day), sizeof(day));

  delete new_note->date_; // Clean up existing date_
  new_note->date_ = new date(year, month, day);

  return new_note;
}
// <--------------------- BASE CLASS NOTE (ABSTRACT) ---------------------->

MyNote::note::note(std::string *note, priority_gen priority_gen)
    : note_(new std::string(*note)), priority_gen_(priority_gen) {
  time_ = std::chrono::system_clock::now();
  time_m_ = time_;
}

void MyNote::note::change(const std::string *note,
                          const std::optional<priority_gen> &priority) {
  if (note) {
    *note_ = *note;
  }
  if (priority.has_value()) {
    priority_gen_ = priority.value();
  }
  time_m_ = std::chrono::system_clock::now();
}

void MyNote::note::show(const settings config) const {
  std::cout << "\033[35m" << "Created: " << "\033[0m"
            << time_to_string(time_, config) << '\n'
            << "\033[35m" << "Last modification: " << "\033[0m"
            << time_to_string(time_m_, config) << '\n';
  switch (priority_gen_) {

  case priority_gen::High:
    std::cout << "\033[31m" << "Priority: High" << "\033[0m" << std::endl;
    break;
  case priority_gen::Middle:
    std::cout << "\033[34m" << "Priority: Middle" << "\033[0m" << std::endl;
    break;
  case priority_gen::Low:
    std::cout << "\033[32m" << "Priority: Low" << "\033[0m" << std::endl;
    break;
  }
  print_string(*note_);
}

MyNote::note::~note() { delete note_; }

// <--------------------- HEADED NOTE ---------------------->

MyNote::headed_note::headed_note(std::string *note, std::string *header,
                                 priority_gen priority_gen)
    : note::note(note, priority_gen), header_(new std::string(*header)) {}

void MyNote::headed_note::change(const std::string *note,
                                 const std::string *header,
                                 const std::optional<priority_gen> &priority) {
  this->note::change(note, priority);
  if (header) {
    *header_ = *header;
  }
}

void MyNote::headed_note::show_note() const {
  std::cout << *header_ << std::endl;
}

void MyNote::headed_note::show(const settings config) const {
  this->show_note();
  this->note::show(config);
}

MyNote::headed_note::~headed_note() { delete header_; }

// <--------------------- DATE NOTE ---------------------->

MyNote::date_note::date_note(std::string *note, date *date,
                             priority_gen priority_gen)
    : note::note(note, priority_gen), date_(date) {}

void MyNote::date_note::change(const std::string *note, date *date,
                               const std::optional<priority_gen> &priority) {
  this->note::change(note, priority);
  if (date) {
    delete date_; // Delete the old date object
    date_ = date; // Assign the new date object
  }
}

void MyNote::date_note::show_note() const { date_->show(); }

void MyNote::date_note::show(const settings config) const {
  this->show_note();
  this->note::show(config);
}

MyNote::date_note::~date_note() { delete date_; }

// <--------------------- SORING FUNCTIONS ---------------------->
// <--------------------- THAT WORK WITH SETTINGS ---------------------->

// Refactored sorting function to reduce redundancy
void MyNote::sort_notes(std::vector<MyNote::note *>::iterator begin,
                        std::vector<MyNote::note *>::iterator end,
                        settings::sorting sort_by,
                        settings::time_sort time_order) {
  switch (sort_by) {
  case settings::sorting::time:
    if (time_order == settings::time_sort::newt) {
      // Sort from new to old
      std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
        return a->time_ > b->time_;
      });
    } else {
      // Sort from old to new
      std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
        return a->time_ < b->time_;
      });
    }
    break;
  case settings::sorting::priority_gen:
    std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
      return a->priority_gen_ < b->priority_gen_;
    });
    break;
  case settings::sorting::time_m:
    if (time_order == settings::time_sort::newt) {
      // Sort from new modification time to old
      std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
        return a->time_m_ > b->time_m_;
      });
    } else {
      // Sort from old modification time to new
      std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
        return a->time_m_ < b->time_m_;
      });
    }
    break;
  }
}

void MyNote::sorting(std::vector<MyNote::note *> &vect, settings config) {
  switch (config.prioriry_category_) {
  case settings::prioriry_category::no:
    // Sort the entire vector based on the sorting configuration
    MyNote::sort_notes(vect.begin(), vect.end(), config.sorting_,
                       config.time_sort_);
    break;

  case settings::prioriry_category::time: {
    // Partition notes into MyNote::date_note and others
    auto it =
        std::partition(vect.begin(), vect.end(), [](MyNote::note *basePtr) {
          return dynamic_cast<MyNote::date_note *>(basePtr) != nullptr;
        });

    // Sort each partition based on the sorting configuration
    MyNote::sort_notes(vect.begin(), it, config.sorting_,
                       config.time_sort_); // Sort MyNote::date_note
    MyNote::sort_notes(it, vect.end(), config.sorting_,
                       config.time_sort_); // Sort the rest
    break;
  }

  case settings::prioriry_category::headed: {
    // Partition notes into headed_note and others
    auto it =
        std::partition(vect.begin(), vect.end(), [](MyNote::note *basePtr) {
          return dynamic_cast<MyNote::headed_note *>(basePtr) != nullptr;
        });

    // Sort each partition based on the sorting configuration
    MyNote::sort_notes(vect.begin(), it, config.sorting_,
                       config.time_sort_); // Sort headed_note
    MyNote::sort_notes(it, vect.end(), config.sorting_,
                       config.time_sort_); // Sort the rest
    break;
  }
  }
}
