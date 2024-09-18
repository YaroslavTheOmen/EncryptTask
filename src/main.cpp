#include "../headers/classes.h"
#include "../headers/commands.h"
#include "../headers/utils.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::string *tests1 = new std::string("This is test string");
  MyNote::headed_note *test1 =
      new MyNote::headed_note(tests1, tests1, priority_gen::High);
  MyNote::headed_note *test2 = new MyNote::headed_note(tests1, tests1);

  std::vector<MyNote::note *> notes;
  notes.push_back(test1);
  notes.push_back(test2);

  settings config;

  try {
    command_loop(notes, config);
  } catch (const std::exception &e) {
    std::cout << "A fatal error occurred: " << e.what() << std::endl;
  }

  // Clean up memory
  for (auto note : notes) {
    delete note;
  }

  return 0;
}
