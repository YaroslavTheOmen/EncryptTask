#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include "classes.h"
#include "utils.h"
#include <iostream>
#include <vector>

// Serialization functions
void load_data(std::vector<MyNote::note *> &notes, settings &config,
               std::istream &in);
void save_data(const std::vector<MyNote::note *> &notes, const settings &config,
               std::ostream &out);
#endif // SERIALIZATION_H_
