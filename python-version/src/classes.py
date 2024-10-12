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
from typing import List, TypeVar
from abc import ABC, abstractmethod
from datetime import datetime
from typing import Optional

# <--------------------- BASE CLASS NOTE (ABSTRACT) ---------------------->


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


# <--------------------- HEADED NOTE ---------------------->


class Headed_note(Note):
    def __init__(
        self, note: str, header: str, priority_gen: priority_gen = priority_gen.High
    ):
        # Call the base class (Note) constructor using super()
        super().__init__(note, priority_gen)
        self.header_ = header

    def change(
        self,
        note: Optional[str] = None,
        priority: Optional[priority_gen] = None,
        header: Optional[str] = None,  # Extra parameter specific to Headed_note
    ) -> None:
        """Override the change method to also modify the header."""
        # Call the base class change method to handle note and priority
        super().change(note, priority)

        # Add logic to handle changes to the header
        if header:
            self.header_ = header

    def show_note(self) -> None:
        """Display heading."""
        print(f"{self.header_}")  # Display the header

    def show(self, config: settings) -> None:
        self.show_note()
        super().show(config)


# <--------------------- DATE NOTE ---------------------->


class Date_note(Note):
    def __init__(
        self, note: str, date: date, priority_gen: priority_gen = priority_gen.High
    ):
        # Call the base class (Note) constructor using super()
        super().__init__(note, priority_gen)
        self.date_ = date

    def change(
        self,
        note: Optional[str] = None,
        priority: Optional[priority_gen] = None,
        date: Optional[date] = None,
    ) -> None:
        """Override the change method to also modify the header."""
        # Call the base class change method to handle note and priority
        super().change(note, priority)

        # Add logic to handle changes to the header
        if date:
            self.date_ = date

    def show_note(self) -> None:
        """Display date."""
        self.date_.show()

    def show(self, config: settings) -> None:
        self.show_note()
        super().show(config)


# <--------------------- SORING FUNCTIONS ---------------------->
# <--------------------- THAT WORK WITH SETTINGS ---------------------->

# Define a TypeVar bound to Note
T = TypeVar("T", bound=Note)


def sort_notes(
    vect: List[T], sort_by: settings_sorting, time_order: settings_time_sort
):
    match (sort_by):
        case settings_sorting.Time:
            if time_order == settings_time_sort.Newt:
                vect.sort(key=lambda note: note.time_, reverse=True)
            else:
                vect.sort(key=lambda note: note.time_, reverse=False)

        case settings_sorting.Time_m:
            if time_order == settings_time_sort.Newt:
                vect.sort(key=lambda note: note.time_m, reverse=True)
            else:
                vect.sort(key=lambda note: note.time_m, reverse=False)

        case settings_sorting.Priority_gen:
            # Priority_gen_ is an Enum, sort by its .value (integer comparison)
            vect.sort(key=lambda note: note.priority_gen_.value)


def sorting(vect: List[Note], config: settings) -> None:
    """Sort the notes based on the priority category in the config."""
    if config.priority_category == settings_priority_category.No:
        # Sort the entire list based on the sorting and time configuration
        sort_notes(vect, config.sorting, config.time_sort)

    elif config.priority_category == settings_priority_category.Time:
        # Partition into Date_note and others
        date_notes = [note for note in vect if isinstance(note, Date_note)]
        other_notes = [note for note in vect if not isinstance(note, Date_note)]
        # Sort each partition
        sort_notes(date_notes, config.sorting, config.time_sort)
        sort_notes(other_notes, config.sorting, config.time_sort)
        # Combine back into vect
        vect[:] = date_notes + other_notes

    elif config.priority_category == settings_priority_category.Headed:
        # Partition into Headed_note and others
        headed_notes = [note for note in vect if isinstance(note, Headed_note)]
        other_notes = [note for note in vect if not isinstance(note, Headed_note)]
        # Sort each partition
        sort_notes(headed_notes, config.sorting, config.time_sort)
        sort_notes(other_notes, config.sorting, config.time_sort)
        # Combine back into vect
        vect[:] = headed_notes + other_notes
