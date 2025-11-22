# Lexical-Analyzer
A C project created as part of the Emertxe Embedded Systems course.

## Introduction
This project implements a simple lexical analyzer in C. It reads a C source file and breaks it into tokens such as keywords, identifiers, numbers, operators, symbols, and more. This is the first stage of a compiler front end.

## Features
- Detects C keywords, identifiers, numbers, operators, and special symbols
- Ignores whitespace and comments
- Prints each lexeme with its token category
- Simple modular design suitable for embedded environments

## How to Compile
```bash
gcc main.c utility.c
```
## How to Run
```bash
./a.out test.c
```

## Usage
1. Prepare a C file (example: test.c)
2. Run the analyzer on it
3. The program prints token type + lexeme line by line


## Limitations
- Supports only basic C tokens
- No parsing or semantic analysis
- Limited error handling

## 🤝 Author and Acknowledgement

This project was developed by:
* **Adhish V** (GitHub Profile: [https://github.com/Adhish-ece](https://github.com/Adhish-ece))

### Course Acknowledgment
This work was completed as part of the **Emertxe Embedded System Course**.
