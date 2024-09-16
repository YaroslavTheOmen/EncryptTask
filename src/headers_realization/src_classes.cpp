#include "../../headers/classes.h"
#include "../../headers/functions.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>

// BASE CLASS (ABSTRACT) <--------------------------------------------->
note::note(std::string *note, priority_gen priority_gen)
    : note_(new std::string(*note)), priority_gen_(priority_gen) {
  time_ = std::chrono::system_clock::now();
  time_m_ = time_;
}

void note::change(std::string *note, priority_gen priority_gen) {
  *note_ = std::move(*note);
  priority_gen_ = priority_gen;
  time_m_ = std::chrono::system_clock::now();
}

void note::show() const {
  std::cout << "\033[35m" << "Created: " << "\033[0m" << time_to_string(time_)
            << '\n'
            << "\033[35m" << "Last modification: " << "\033[0m"
            << time_to_string(time_m_) << '\n';
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

note::~note() { delete note_; }

// HEADED NOTE <--------------------------------------------->
headed_note::headed_note(std::string *note, std::string *header,
                         priority_gen priority_gen)
    : note::note(note, priority_gen), header_(new std::string(*header)) {}

void headed_note::change(std::string *note, std::string *header,
                         priority_gen priority_gen) {
  note::change(note, priority_gen);
  header_ = std::move(header);
}

void headed_note::show_note() const { std::cout << *header_ << std::endl; }

void headed_note::show() const {
  this->show_note();
  this->note::show();
}

headed_note::~headed_note() { delete header_; }

// NOTE WITH DATE  <--------------------------------------------->
// TODO: create all methods

date_note::date_note(std::string *note, date *date, priority_gen priority_gen)
    : note::note(note, priority_gen), date_(date) {}

void date_note::change(std::string *note, date *date,
                       priority_gen priority_gen) {
  note::change(note, priority_gen);
  date_ = std::move(date);
}

void date_note::show_note() const { date_->show(); }

void date_note::show() const {
  this->show_note();
  this->note::show();
}

date_note::~date_note() { delete date_; }
