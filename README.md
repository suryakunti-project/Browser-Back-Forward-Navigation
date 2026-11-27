# WebTrack – Browser History Management System Using Two Stacks in C

## Introduction

**WebTrack** is a terminal-based browser history simulator developed in **C (ISO C99)**. It demonstrates how modern web browsers manage **Back** and **Forward** page navigation using **exactly two array-based stacks** (`Back Stack` and `Forward Stack`), along with a separate chronological **Browsing History** log.

This project is designed as a beginner-friendly college mini project to illustrate the **LIFO (Last In, First Out)** principle of the stack data structure in a practical, real-world scenario.

---

## Abstract

Web browsers allow users to move backward and forward seamlessly across visited web pages. Behind these two buttons lies a classic data structure application: two stacks working together. In **WebTrack**, the `Back Stack` stores pages visited before the current page, while the `Forward Stack` stores pages moved away from during backward navigation. A separate string variable holds the currently active website, and an independent array maintains the full browsing history log for viewing, searching, and clearing. Implemented purely in standard C using `fgets()`-based safe input handling and strict bounds checking, WebTrack provides a reliable, crash-free simulation in the terminal.

---

## Problem Statement

Many beginners learn stacks only through abstract push/pop examples with numbers. It can be difficult to connect stack theory to real software systems. Additionally, simulating browser navigation requires handling tricky edge cases such as:

- Preventing stack underflow when pressing Back or Forward with no available pages.
- Preventing stack overflow when a fixed-size stack reaches capacity.
- Clearing forward navigation history whenever a user goes back and visits a new website.
- Keeping active tab navigation (stacks) separate from the permanent browsing history log.

WebTrack solves this by implementing a complete, menu-driven C simulation that handles all navigation rules and edge cases cleanly.

---

## Objectives

1. Demonstrate the **LIFO (Last In, First Out)** behavior of stacks using arrays in C.
2. Implement **exactly two navigation stacks** (`Back Stack` and `Forward Stack`) without introducing extra navigation stacks.
3. Simulate real browser navigation rules: Visit New Website, Go Back, Go Forward, and clearing the Forward Stack on branching visits.
4. Maintain a separate **Browsing History** array with viewing, case-insensitive searching, statistics, and confirmation-based clearing.
5. Practice safe C programming practices (`fgets()` input, URL validation, overflow/underflow checks, modular functions, and zero compiler warnings).

---

## Features

- **Menu-driven terminal interface** with options `0` to `10`.
- **Visit a New Website** with URL validation (`http://` or `https://`, non-empty, no spaces, length limit `255` characters).
- **Go Back** and **Go Forward** navigation using two array-based stacks (`MAX_STACK_SIZE = 50`).
- **Automatic Forward Stack reset** when visiting a new website after going back.
- **Inspect Current Website, Back Stack, and Forward Stack** at any time.
- **Separate Browsing History** (`MAX_HISTORY_SIZE = 200`) with a clear warning if the 200-entry limit is reached (without silently overwriting old entries).
- **Case-insensitive History Search** by keyword or URL.
- **Navigation Statistics** (total visits, back count, forward count, stack sizes, stored history count).
- **Confirmation prompt (`y/n`)** before clearing browsing history.

---

## Technologies Used

- **Language:** C (ISO C99)
- **Compiler:** GCC (MinGW-w64 via MSYS2 UCRT64)
- **Operating System:** Windows 11
- **Data Structures:** Array-based Stacks (2), Fixed-size String Array (Browsing History), Structures (`struct`)
- **Libraries:** Standard C headers only (`<stdio.h>`, `<string.h>`, `<ctype.h>`, `<stdlib.h>`)

---

## Requirements

- Windows 11 (or any OS with a C99 compiler)
- GCC supporting `-std=c99` (tested with `gcc.exe (Rev7, Built by MSYS2 project) 16.2.0`)
- Terminal / PowerShell / Command Prompt

---

## Installation and Compilation Instructions

1. Open a terminal inside the project directory `Browser-Back-Forward-Navigation`.
2. Compile `webtrack.c` using GCC with strict C99 warning flags:

```powershell
gcc -std=c99 -Wall -Wextra -Wpedantic webtrack.c -o webtrack
```

### What each flag means:
- `gcc`: Invokes the GNU C Compiler.
- `-std=c99`: Uses the ISO C99 language standard.
- `-Wall -Wextra -Wpedantic`: Enables helpful compiler warnings to catch mistakes early.
- `webtrack.c`: Input C source file.
- `-o webtrack`: Outputs the compiled executable `webtrack.exe`.

---

## How to Run the Program

After compilation succeeds, run:

```powershell
.\webtrack.exe
```

Use menu numbers `1`–`10` to test navigation and history features, and `0` to exit.

---

## Example Output

```text
============================================================
      WebTrack - Browser History Management System (C)      
         Simulating Back & Forward Using Two Stacks         
============================================================
  Stack Capacity : 50 URLs per stack
  History Limit  : 200 entries
  Accepted URLs  : Must start with http:// or https://
============================================================

-------------------- WEBTRACK MENU -------------------------
  1. Visit a New Website
  2. Go Back
  3. Go Forward
  4. Display Current Website
  5. Display Back Stack
  6. Display Forward Stack
  7. View Browsing History
  8. Search Browsing History
  9. Display Navigation Statistics
 10. Clear Browsing History
  0. Exit
------------------------------------------------------------
Enter your choice (0-10): 1

Enter website URL (starting with http:// or https://): https://www.google.com

[Success] Now visiting: https://www.google.com

Enter your choice (0-10): 1

Enter website URL (starting with http:// or https://): https://www.github.com

[Success] Now visiting: https://www.github.com

Enter your choice (0-10): 2

[Back] Current website: https://www.google.com

Enter your choice (0-10): 6

--- Forward Stack ---
Total items: 1 / 50
  [TOP] https://www.github.com

Enter your choice (0-10): 1

Enter website URL (starting with http:// or https://): https://www.stackoverflow.com

[Success] Now visiting: https://www.stackoverflow.com
[Info] Forward Stack was cleared because a new website was visited.
```

---

## Project Files

- `webtrack.c` — Complete C source code.
- `README.md` — Project documentation and usage guide.
- `algorithms.md` — Step-by-step algorithms for core operations.
- `flowcharts.md` — Mermaid flowcharts for main loop and navigation logic.
- `test_cases.md` — Verified test scenarios and edge-case checks.
- `viva_questions.md` — Viva preparation questions and answers.

---

## Conclusion

WebTrack clearly demonstrates how two simple array-based stacks work together to power browser Back and Forward navigation. Keeping the two navigation stacks separate from the browsing history array makes the architecture clean, easy to understand, and directly aligned with real-world browser behavior.
