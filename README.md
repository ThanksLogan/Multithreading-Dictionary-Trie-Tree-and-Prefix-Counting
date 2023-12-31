# C++ Multithreading: Dictionary Tree and Prefix Counting

## Overview
This C++ project utilizes multithreading to build a dictionary tree from a vocabulary file and count the number of words starting with specific prefixes from a test file. It showcases thread synchronization, mutex locks, and condition variables to coordinate the shared data structure access among multiple threads.

## Getting Started

### Prerequisites
- A C++ compiler (e.g., g++)
- Standard C++ libraries
- Pthread library for multithreading
- Make utility (optional for build automation)

### Installation and Compilation
1. Ensure g++ or a similar C++ compiler and Pthread library are installed on your machine.
2. Clone or download this repository to your local machine.
3. Use the provided Makefile for easy compilation:
```
make
```

This will create the executable `countprefix`.

### Running the Program
Run the program with two arguments: the vocabulary file and the test file.
./countprefix [vocabulary_file] [test_file]


## Code Description

### Files and Modules
- `dictionarytree.h` & `dictionarytree.cpp`: Implements the dictionary tree data structure.
- `populatetree.cpp`, `readprefix.cpp`, `countprefix.cpp`: Implements the thread functions for tree population, reading prefixes, and counting prefixes.
- `shared.h`: Contains the shared data structure used across threads.
- `main.cpp`: The main driver of the program.
- `Makefile`: Script for compiling the program with the correct flags and dependencies.

### Key Components
- **Dictionary Tree**:
  - A trie-based implementation for efficient word storage and prefix-based searches.
  - Multithreaded construction of the dictionary tree from a vocabulary file.

- **Thread Functions**:
  - `populatetree()`: Populates the dictionary tree with words from the vocabulary file.
  - `readprefix()`: Reads prefixes from the test file and enqueues them for processing.
  - `countprefix()`: Counts the number of words starting with each prefix and writes the results to an output file.

### Multithreading Aspects
- Utilizes pthreads for creating and managing threads.
- Synchronization mechanisms like mutexes and condition variables to ensure safe access to shared resources.

### Compilation Flags
- `-std=c++11`: Uses C++11 standard.
- `-Wall`: Enables all compiler's warning messages.
- `-g3`: Includes extra debugging information.
- `-pthread`: Links the pthread library for multithreading support.

### Makefile Targets
- `$(PROGRAM)`: Compiles the main program.
- `clean`: Removes compiled objects and executable for cleanup.

## Testing
- The project includes test cases in the `main` function to demonstrate its functionality.
- It reads words from the vocabulary file, builds the dictionary tree, and counts word occurrences from the test file.
