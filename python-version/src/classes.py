from note_utils import (
    settings,
    settings_sorting,
    settings_time_sort,
    settings_priority_category,
    priority_gen,
    print_string,
    time_to_string,
    date,
)
from typing import List, TypeVar, Optional
from abc import ABC, abstractmethod
from datetime import datetime


# Base Class Note (Abstract)
class Note(ABC):
    def __init__(self, note: str, priority_gen: priority_gen = priority_gen.High):
        self.note_ = note if note else ""
        self.priority_gen_ = priority_gen
        self.time_ = datetime.now()
        self.time_m = self.time_

    def change(
        self, note: Optional[str] = None, priority: Optional[priority_gen] = None
    ) -> None:
        """Change the note content and/or priority, and update the modification time."""
        if note:
            self.note_ = note
        if priority:
            self.priority_gen_ = priority
        self.time_m = datetime.now()

    @abstractmethod
    def show_note(self) -> None:
        """Abstract method to show the note's content in a customized way (protected in C++)."""
        pass

    def show(self, config: settings) -> None:
        """Display the note's details, including timestamps and priority."""
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


# Headed Note
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


# Date Note
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
