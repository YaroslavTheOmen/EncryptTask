# In classes.py

import struct
from typing import List, TypeVar, Optional, BinaryIO
from abc import ABC, abstractmethod
from datetime import datetime
from note_utils import (
    settings,
    settings_sorting,
    settings_time_sort,
    settings_priority_category,
    priority_gen,
    print_string,
    time_to_string,
    date,
    NoteType,
)


class Note(ABC):
    def __init__(self, note: str, priority_gen: priority_gen = priority_gen.High):
        self.note_ = note if note else ""
        self.priority_gen_ = priority_gen
        self.time_ = datetime.now()
        self.time_m = self.time_

    def change(
        self, note: Optional[str] = None, priority: Optional[priority_gen] = None
    ) -> None:
        if note:
            self.note_ = note
        if priority:
            self.priority_gen_ = priority
        self.time_m = datetime.now()

    @abstractmethod
    def show_note(self) -> None:
        pass

    def show(self, config: settings) -> None:
        print("\033[35m" + "Created: " + "\033[0m" + time_to_string(self.time_, config))
        print(
            "\033[35m"
            + "Last modification: "
            + "\033[0m"
            + time_to_string(self.time_m, config)
        )

        if self.priority_gen_ == priority_gen.High:
            print("\033[31m" + "Priority: High" + "\033[0m")
        elif self.priority_gen_ == priority_gen.Middle:
            print("\033[34m" + "Priority: Middle" + "\033[0m")
        elif self.priority_gen_ == priority_gen.Low:
            print("\033[32m" + "Priority: Low" + "\033[0m")

        print_string(self.note_)

    def serialize(self, out: BinaryIO):
        pass

    def serialize_base(self, out: BinaryIO):
        # Serialize time_ (microseconds since epoch)
        time_since_epoch = int(self.time_.timestamp() * 1e6)
        out.write(struct.pack("<q", time_since_epoch))  # int64_t, little-endian

        # Serialize time_m_ (microseconds since epoch)
        time_m_since_epoch = int(self.time_m.timestamp() * 1e6)
        out.write(struct.pack("<q", time_m_since_epoch))  # int64_t, little-endian

        # Serialize priority_gen_
        out.write(struct.pack("<B", self.priority_gen_.value))  # uint8_t

        # Serialize note_
        note_bytes = self.note_.encode("utf-8")
        note_length = len(note_bytes)
        out.write(struct.pack("<Q", note_length))  # uint64_t, little-endian
        out.write(note_bytes)

    def deserialize_base(self, inp: BinaryIO):
        # Deserialize time_
        time_data = inp.read(8)
        if len(time_data) != 8:
            raise ValueError("Failed to read time.")
        time_since_epoch = struct.unpack("<q", time_data)[0]
        self.time_ = datetime.fromtimestamp(time_since_epoch / 1e6)

        # Deserialize time_m_
        time_m_data = inp.read(8)
        if len(time_m_data) != 8:
            raise ValueError("Failed to read time_m.")
        time_m_since_epoch = struct.unpack("<q", time_m_data)[0]
        self.time_m = datetime.fromtimestamp(time_m_since_epoch / 1e6)

        # Deserialize priority_gen_
        priority_data = inp.read(1)
        if len(priority_data) != 1:
            raise ValueError("Failed to read priority_gen.")
        priority_value = struct.unpack("<B", priority_data)[0]
        self.priority_gen_ = priority_gen(priority_value)

        # Deserialize note_
        note_length_data = inp.read(8)
        if len(note_length_data) != 8:
            raise ValueError("Failed to read note length.")
        note_length = struct.unpack("<Q", note_length_data)[0]
        note_bytes = inp.read(note_length)
        if len(note_bytes) != note_length:
            raise ValueError("Failed to read note data.")
        self.note_ = note_bytes.decode("utf-8")

    @staticmethod
    def deserialize(inp: BinaryIO) -> "Note":
        # Read NoteType (1 byte)
        note_type_data = inp.read(1)
        if len(note_type_data) != 1:
            raise ValueError("Failed to read note type.")
        note_type_value = struct.unpack("<B", note_type_data)[0]
        note_type = NoteType(note_type_value)

        if note_type == NoteType.HeadedNote:
            return Headed_note.deserialize(inp)
        elif note_type == NoteType.DateNote:
            return Date_note.deserialize(inp)
        else:
            raise ValueError("Unknown note type.")


class Headed_note(Note):
    def __init__(
        self, note: str, header: str, priority_gen: priority_gen = priority_gen.High
    ):
        super().__init__(note, priority_gen)
        self.header_ = header

    def change(
        self,
        note: Optional[str] = None,
        priority: Optional[priority_gen] = None,
        header: Optional[str] = None,
    ) -> None:
        super().change(note, priority)
        if header:
            self.header_ = header

    def show_note(self) -> None:
        print(f"{self.header_}")

    def show(self, config: settings) -> None:
        self.show_note()
        super().show(config)

    def serialize(self, out: BinaryIO):
        # Write NoteType as uint8_t
        out.write(struct.pack("<B", NoteType.HeadedNote.value))  # 1 byte, little-endian
        self.serialize_base(out)

        # Serialize header_
        header_bytes = self.header_.encode("utf-8")
        header_length = len(header_bytes)
        out.write(struct.pack("<Q", header_length))  # uint64_t, little-endian
        out.write(header_bytes)

    @classmethod
    def deserialize(cls, inp: BinaryIO) -> "Headed_note":
        new_note = cls(note="", header="")
        new_note.deserialize_base(inp)

        # Deserialize header_
        header_length_data = inp.read(8)
        if len(header_length_data) != 8:
            raise ValueError("Failed to read header length.")
        header_length = struct.unpack("<Q", header_length_data)[0]
        header_bytes = inp.read(header_length)
        if len(header_bytes) != header_length:
            raise ValueError("Failed to read header data.")
        new_note.header_ = header_bytes.decode("utf-8")
        return new_note


class Date_note(Note):
    def __init__(
        self, note: str, date: date, priority_gen: priority_gen = priority_gen.High
    ):
        super().__init__(note, priority_gen)
        self.date_ = date

    def change(
        self,
        note: Optional[str] = None,
        priority: Optional[priority_gen] = None,
        date: Optional[date] = None,
    ) -> None:
        super().change(note, priority)
        if date:
            self.date_ = date

    def show_note(self) -> None:
        self.date_.show()

    def show(self, config: settings) -> None:
        self.show_note()
        super().show(config)

    def serialize(self, out: BinaryIO):
        # Write NoteType as uint8_t
        out.write(struct.pack("<B", NoteType.DateNote.value))  # 1 byte, little-endian
        self.serialize_base(out)

        # Serialize date_
        out.write(struct.pack("<i", self.date_._year))  # int32_t, little-endian
        out.write(struct.pack("<i", self.date_._month))  # int32_t, little-endian
        out.write(struct.pack("<i", self.date_._day))  # int32_t, little-endian

    @classmethod
    def deserialize(cls, inp: BinaryIO) -> "Date_note":
        new_note = cls(note="", date=date(0, 0, 0))
        new_note.deserialize_base(inp)

        # Deserialize date_
        year_data = inp.read(4)
        if len(year_data) != 4:
            raise ValueError("Failed to read date year.")
        year = struct.unpack("<i", year_data)[0]

        month_data = inp.read(4)
        if len(month_data) != 4:
            raise ValueError("Failed to read date month.")
        month = struct.unpack("<i", month_data)[0]

        day_data = inp.read(4)
        if len(day_data) != 4:
            raise ValueError("Failed to read date day.")
        day = struct.unpack("<i", day_data)[0]

        new_note.date_ = date(_year=year, _month=month, _day=day)
        return new_note


# Sorting Functions
T = TypeVar("T", bound=Note)


def sort_notes(
    vect: List[T], sort_by: settings_sorting, time_order: settings_time_sort
):
    if sort_by == settings_sorting.Time:
        vect.sort(
            key=lambda note: note.time_, reverse=(time_order == settings_time_sort.Newt)
        )
    elif sort_by == settings_sorting.Time_m:
        vect.sort(
            key=lambda note: note.time_m,
            reverse=(time_order == settings_time_sort.Newt),
        )
    elif sort_by == settings_sorting.Priority_gen:
        vect.sort(key=lambda note: note.priority_gen_.value)


def sorting(vect: List[Note], config: settings) -> None:
    """Sort the notes based on the priority category in the config."""
    if config.priority_category == settings_priority_category.No:
        sort_notes(vect, config.sorting, config.time_sort)
    elif config.priority_category == settings_priority_category.Time:
        date_notes = [note for note in vect if isinstance(note, Date_note)]
        other_notes = [note for note in vect if not isinstance(note, Date_note)]
        sort_notes(date_notes, config.sorting, config.time_sort)
        sort_notes(other_notes, config.sorting, config.time_sort)
        vect[:] = date_notes + other_notes
    elif config.priority_category == settings_priority_category.Headed:
        headed_notes = [note for note in vect if isinstance(note, Headed_note)]
        other_notes = [note for note in vect if not isinstance(note, Headed_note)]
        sort_notes(headed_notes, config.sorting, config.time_sort)
        sort_notes(other_notes, config.sorting, config.time_sort)
        vect[:] = headed_notes + other_notes
