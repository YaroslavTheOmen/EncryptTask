#include "../headers/classes.h"
#include "../headers/functions.h"
#include <string>

int main() {
  std::string *headert = new std::string("Hello world");
  std::string *note = new std::string(
      "This is the test note, This is the test noteThis is the test noteThis "
      "is the test note This is the test note This is the test note This is "
      "the "
      "test note This is the test note This is the test note This is the test "
      "note This is the test note");

  headed_note test(note, headert, priority_gen::Low);

  test.show();

  return 0;
}
