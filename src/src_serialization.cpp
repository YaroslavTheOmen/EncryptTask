#include "../headers/serialization.h"
#include <stdexcept>

void save_data(const std::vector<MyNote::note *> &notes, const settings &config,
               std::ostream &out) {
  // Save settings
  config.serialize(out);

  // Save number of notes
  size_t num_notes = notes.size();
  out.write(reinterpret_cast<const char *>(&num_notes), sizeof(num_notes));

  // Save each note
  for (const auto &note : notes) {
    note->serialize(out);
  }
}

void load_data(std::vector<MyNote::note *> &notes, settings &config,
               std::istream &in) {
  // Load settings
  config = settings::deserialize(in);

  // Load number of notes
  size_t num_notes;
  in.read(reinterpret_cast<char *>(&num_notes), sizeof(num_notes));

  // Load each note
  for (size_t i = 0; i < num_notes; ++i) {
    MyNote::note *note = MyNote::note::deserialize(in);
    if (note) {
      notes.push_back(note);
    } else {
      throw std::runtime_error("Failed to deserialize note.");
    }
  }
}
