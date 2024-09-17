#ifndef NOTE_F_
#define NOTE_F_

#include "classes.h"
#include "utils.h"
#include <vector>

namespace MyNote {

// <--------------------- BODY FUNCTION ---------------------->
void actions(std::vector<MyNote::note *> vect, settings *config);

// <--------------------- BRANCH FUNCTIONS ---------------------->
void change(MyNote::note *);
void write(MyNote::note *);
void display(MyNote::note *);
void settings(settings *config);
void delete_n(int *n);

} // namespace MyNote

#endif // NOTE_F_
