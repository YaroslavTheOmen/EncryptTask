from dataclasses import dataclass
from datetime import datetime
from enum import Enum


class NoteType(Enum):
    HeadedNote = 0
    DateNote = 1


class settings_sorting(Enum):
    Time = 0
    Priority_gen = 1
    Time_m = 2


class settings_priority_category(Enum):
    Time = 0
    Headed = 1
    No = 2


class settings_time_type(Enum):
    Twelve = 0
    Twenty_four = 1


class settings_time_sort(Enum):
    Newt = 0
    Oldt = 1


@dataclass
class settings:
    sorting: settings_sorting
    priority_category: settings_priority_category
    time_type: settings_time_type
    time_sort: settings_time_sort


class priority_gen(Enum):
    High = 0
    Middle = 1
    Low = 2


@dataclass
class date:
    _day: int
    _month: int
    _year: int

    def show(self) -> None:
        print(f"{self._year}.{self._month}.{self._day}")


def print_string(s: str) -> None:
    words = s.split()
    current_line = ""

    for word in words:
        if len(current_line) + len(word) + 1 > 45:
            print(current_line)
            current_line = word
        else:
            if current_line:
                current_line += " "
            current_line += word

    if current_line:
        print(current_line)


def time_to_string(time: datetime, config: settings) -> str:
    if config.time_type == settings_time_type.Twenty_four:
        return time.strftime("%Y-%m-%d %H:%M:%S")
    else:
        return time.strftime("%Y-%m-%d %I:%M:%S %p")
