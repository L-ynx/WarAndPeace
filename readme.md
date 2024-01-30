# War and Peace Chapter Categorizer
This C++ program applies a functional programming approach to analyze the novel "War and Peace." It categorizes each chapter as war-related or peace-related based on term densities and occurences related to war and peace.

Employing lambda expressions, higher-order functions, and immutability, the program reads the novel and term lists from files, tokenizes chapters, calculates term densities and categorizes and prints the chapters. It leverages parallel execution for efficiency in certain operations, adhering to functional programming principles of immutability, avoidance of side effects, use of pure functions and treating functions as first-class citizens.

## Setup
### Compile
```g++ -std=c++17 -I/include/war_and_peace.h src/war_and_peace.cpp -o main```

### Run
```./main```

### Run (Windows)
Simply double-click the run.bat file to execute the program including its tests.
