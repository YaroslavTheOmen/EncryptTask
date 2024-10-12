# main.py

from datetime import datetime
from typing import List
from note_utils import (
    settings,
    settings_sorting,
    settings_time_sort,
    settings_time_type,
    settings_priority_category,
    priority_gen,
    date,
)
from classes import Headed_note, Date_note, Note
from classes import sorting


def main():
    # Create some date instances
    date1 = date(_day=14, _month=5, _year=2024)
    date2 = date(_day=20, _month=6, _year=2024)

    # Create some notes
    headed_note1 = Headed_note(
        note="Discuss project milestones.",
        header="Project Meeting",
        priority_gen=priority_gen.High,
    )
    headed_note2 = Headed_note(
        note="Plan team building activities.",
        header="Team Building",
        priority_gen=priority_gen.Middle,
    )
    dated_note1 = Date_note(
        note="Submit project report.",
        date=date1,
        priority_gen=priority_gen.High,
    )
    dated_note2 = Date_note(
        note="Finalize budget.",
        date=date2,
        priority_gen=priority_gen.Low,
    )
    regular_note = Date_note(
        note="General note without specific date.",
        date=date1,
        priority_gen=priority_gen.Middle,
    )

    # Combine all notes into a list
    notes: List[Note] = [
        headed_note1,
        dated_note1,
        headed_note2,
        dated_note2,
        regular_note,
    ]

    # Define sorting settings
    config = settings(
        sorting=settings_sorting.Time_m,  # Sort by modification time
        priority_category=settings_priority_category.Time,  # Prioritize Date_note
        time_type=settings_time_type.Twenty_four,
        time_sort=settings_time_sort.Newt,  # Newest first
    )

    # Before sorting
    print("Before sorting:")
    for note in notes:
        note.show(config)
        print("-" * 40)

    # Sort the notes based on the configuration
    sorting(notes, config)

    config1 = settings(
        sorting=settings_sorting.Priority_gen,  # Sort by modification time
        priority_category=settings_priority_category.No,  # Prioritize Date_note
        time_type=settings_time_type.Twenty_four,
        time_sort=settings_time_sort.Newt,  # Newest first
    )

    sorting(notes, config1)

    # After sorting
    print("\nAfter sorting:")
    for note in notes:
        note.show(config)
        print("-" * 40)


if __name__ == "__main__":
    main()
