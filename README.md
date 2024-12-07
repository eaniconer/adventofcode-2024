# Advent of Code 2024 Solutions

This repository contains my C++ solutions to the Advent of Code 2024 challenges.

## Structure

Each day's solution is in a separate directory (`day-01`, `day-02`, etc.) and contains:

* `input.txt`: the input data for that day's challenge
* `part-1.cpp`: the C++ solution code for part 1 of the challenge
* `part-2.cpp`: the C++ solution code for part 2 of the challenge
* `CMakeLists.txt`: CMake configuration file (starting from day-06)

## Compiling and Running the Solutions

Use CMake to compile and run the solutions:

```bash
cmake -B./build -S.
cmake --build ./build
./build/part-1
./build/part-2
```

# Notes

- This repository is for personal use and learning purposes only.
- Solutions are provided as-is and may not be optimal or efficient.
- Feel free to use this repository as a reference or starting point for your own Advent of Code solutions.
- Check out the official Advent of Code 2024 website: https://adventofcode.com/2024