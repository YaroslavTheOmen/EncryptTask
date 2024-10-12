from dataclasses import dataclass
from enum import Enum


__all__ = [
    "settings",
    "sorting",
    "priority_category",
    "time_type",
    "time_sort",
    "priority_gen",
    "date",
]


class sorting(Enum):
    time = 1
    time_m = 2
    priority_gen = 3


class priority_category(Enum):
    time = 1
    headed = 2
    no = 3


class time_type(Enum):
    twelve = 1
    twenty_four = 2


class time_sort(Enum):
    oldt = 1
    newt = 2


@dataclass
class settings:
    sorting: sorting
    priority_category: priority_category
    time_type: time_type
    time_sort: time_sort


class priority_gen(Enum):
    High = 1
    Middle = 2
    Low = 3


@dataclass
class date:
    _day: int
    _month: int
    _year: int

    def show(self) -> None:
        print(f"{self._year}.{self._month}.{self._day}")

    def __init__(self, year: int, month: int, day: int) -> None:
        self._day = day
        self._month = month
        self._year = year
