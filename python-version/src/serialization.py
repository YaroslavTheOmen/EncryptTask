# serialization.py

import struct
from typing import List, BinaryIO

from classes import Note
from note_utils import (
    settings,
    settings_sorting,
    settings_priority_category,
    settings_time_type,
    settings_time_sort,
)


def save_data(notes: List[Note], config: settings, out: BinaryIO):
    # Save settings
    out.write(
        struct.pack(
            "<BBBB",
            config.sorting.value,
            config.priority_category.value,
            config.time_type.value,
            config.time_sort.value,
        )
    )

    # Save number of notes
    num_notes = len(notes)
    out.write(struct.pack("<Q", num_notes))  # uint64_t, little-endian

    # Save each note
    for note in notes:
        note.serialize(out)


def load_data(notes: List[Note], config: settings, inp: BinaryIO):
    # Load settings
    settings_data = inp.read(4)
    if len(settings_data) != 4:
        raise ValueError("Failed to read settings.")
    (
        sorting_value,
        priority_category_value,
        time_type_value,
        time_sort_value,
    ) = struct.unpack("<BBBB", settings_data)
    config.sorting = settings_sorting(sorting_value)
    config.priority_category = settings_priority_category(priority_category_value)
    config.time_type = settings_time_type(time_type_value)
    config.time_sort = settings_time_sort(time_sort_value)

    # Load number of notes
    num_notes_data = inp.read(8)
    if len(num_notes_data) != 8:
        raise ValueError("Failed to read number of notes.")
    num_notes = struct.unpack("<Q", num_notes_data)[0]

    for _ in range(num_notes):
        note = Note.deserialize(inp)
        if note:
            notes.append(note)
        else:
            raise ValueError("Failed to deserialize note.")
