#include "../headers/classes.h"
#include "../headers/utils.h"
#include <string>

int main() {
  // Creating heap-allocated strings
  std::string *headert = new std::string("Hello world");
  std::string *note =
      new std::string("********** ******* ************* ********** *********** "
                      "******** **** *****"
                      "********** ******* ************* ********** *********** "
                      "******** **** *****"
                      "********** ******* ************* ********** *********** "
                      "******** **** *****"
                      "********** ******* ************* ********** *********** "
                      "******** **** *****"
                      "********** ******* ************* ********** *********** "
                      "******** **** *****");

  // Creating heap-allocated date
  date *date = new struct date(2005, 05, 14);

  settings config;
  config.sorting_ = settings::sorting::priority_gen;

  // Creating heap-allocated note objects
  MyNote::headed_note *test =
      new MyNote::headed_note(note, headert, priority_gen::Low);
  MyNote::headed_note *test1 =
      new MyNote::headed_note(note, headert, priority_gen::High);
  MyNote::headed_note *test2 =
      new MyNote::headed_note(note, headert, priority_gen::High);
  MyNote::date_note *test3 =
      new MyNote::date_note(note, date, priority_gen::Middle);

  // Creating a vector of note pointers
  std::vector<MyNote::note *> vect;

  vect.push_back(test);
  vect.push_back(test1);
  vect.push_back(test2);
  vect.push_back(test3);

  MyNote::sorting(vect, config);

  for (auto x : vect) {
    x->show(config);
  }

  return 0;
}
