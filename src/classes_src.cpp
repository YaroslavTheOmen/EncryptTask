#include "../headers/classes.h"
#include "../headers/structures.h"
#include <chrono>
#include <ctime>
#include <string>
#include <utility>

// BASE CLASS (ABSTRACT) <--------------------------------------------->
// TODO: end show() method
note::note(std::string *note, char priority_alphabatical)
    : note_(new std::string(*note)),
      priority_alphabatical_(priority_alphabatical) {
  time_ = std::chrono::system_clock::now();
  time_m_ = time_;
}

void note::change(std::string *note, char priority_alphabatical) {
  *note_ = std::move(*note);
  priority_alphabatical_ = priority_alphabatical;
  time_m_ = std::chrono::system_clock::now();
}

void note::show() const {}

note::~note() { delete note_; }

// HEADED NOTE <--------------------------------------------->
// TODO: create all methods

// NOTE WITH DATE  <--------------------------------------------->
// TODO: create all methods
