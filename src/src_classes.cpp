#include "../headers/classes.h"
#include "../headers/utils.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <optional>
#include <string>

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
                        settings::sorting sort_by) {
  switch (sort_by) {
  case settings::sorting::time:
    std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
      return a->time_ < b->time_;
    }); // Assuming get_time() is a getter for time_
    break;
  case settings::sorting::priority_gen:
    std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
      return a->priority_gen_ < b->priority_gen_;
    }); // Assuming get_priority_gen() is a getter
    break;
  case settings::sorting::time_m:
    std::sort(begin, end, [](MyNote::note *a, MyNote::note *b) {
      return a->time_m_ < b->time_m_;
    }); // Assuming get_time_m() is a getter
    break;
  }
}

void MyNote::sorting(std::vector<MyNote::note *> &vect, settings config) {
  switch (config.prioriry_category_) {
  case settings::prioriry_category::no:
    // Sort the entire vector based on the sorting configuration
    MyNote::sort_notes(vect.begin(), vect.end(), config.sorting_);
    break;

  case settings::prioriry_category::time: {
    // Partition notes into MyNote::date_note and others
    auto it =
        std::partition(vect.begin(), vect.end(), [](MyNote::note *basePtr) {
          return dynamic_cast<MyNote::date_note *>(basePtr) != nullptr;
        });

    // Sort each partition based on the sorting configuration
    MyNote::sort_notes(vect.begin(), it,
                       config.sorting_); // Sort MyNote::date_note
    MyNote::sort_notes(it, vect.end(), config.sorting_); // Sort the rest
    break;
  }

  case settings::prioriry_category::headed: {
    // Partition notes into headed_note and others
    auto it =
        std::partition(vect.begin(), vect.end(), [](MyNote::note *basePtr) {
          return dynamic_cast<MyNote::headed_note *>(basePtr) != nullptr;
        });

    // Sort each partition based on the sorting configuration
    MyNote::sort_notes(vect.begin(), it, config.sorting_); // Sort headed_note
    MyNote::sort_notes(it, vect.end(), config.sorting_);   // Sort the rest
    break;
  }
  }
}
