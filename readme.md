# Homework 2 — Banker's Algorithm (Process Safety)

# Author: Brendan Valleau

# Course: CS-OS — 10/17/2025

This repository contains two C++ programs that implement the Banker's safety algorithm to determine whether a system is in a safe state and to compute a safe sequence of process execution given resource allocation data in JSON form.

Contents
- `src/process0.cpp` — refactored implementation (modern C++ vectors, `computeSafety` function).
- `src/input.json` — example input used for testing.
- `json.hpp` — single-header nlohmann::json library used for parsing.

Prerequisites
- A C++17-capable compiler (`g++` on Linux/macOS, `cl.exe` on Windows), and a terminal/PowerShell.
- `json.hpp` must be present in `src/` (already included in this repo).

Build

Using g++ (Linux / MinGW / WSL / macOS):

```pwsh
g++ -std=c++17 -g ./src/process0.cpp -o process0.exe
```

Or build the other implementation:

```pwsh
g++ -std=c++17 -g ./src/process.cpp -o process.exe
```

Using Microsoft Visual C++ (Developer PowerShell):

```pwsh
cl.exe /EHsc /std:c++17 /Zi /Fe:process0.exe src\process0.cpp
```

Run

Both executables accept a path to a JSON input file as the first argument. If no argument is provided, `process.cpp` attempts to open `default.json`.

Example:

```pwsh
./process0.exe ./src/input.json
./process.exe  ./src/input.json
```

JSON input format

Two slightly different input key formats are used by the two programs in this repo — be careful which program you run.

- `process.cpp` expects keys: `processes` (array) and `resources` (array).
- `process0.cpp` expects keys: `Processes` (array) and `Resources` (array).

A compatible `Processes` entry looks like:

```json
{
  "Processes": [
    {
      "Name": "P0",
      "Max": [7, 5, 3],
      "Allocation": [0, 1, 0]
    },
    {
      "Name": "P1",
      "Max": [3, 2, 2],
      "Allocation": [2, 0, 0]
    }
  ],
  "Resources": [
    {"Name": "A","Max": [] "Available": 3 },
  ]
}
```

Notes on implementation

- `process.cpp` implements the safety algorithm inline in `main` using C-style arrays. It computes `Need = Max - Allocation`, iteratively tries to find processes whose need is <= available, and builds a safe sequence. 

- `process0.cpp` extracts the safety logic into `computeSafety(...)` and uses `std::vector` containers. The function reads `Available`, builds a working copy (`Work`), checks process `Need` against `Work`, and records a safe sequence. The version in the repository has some small issues (result vector initialization and noisy printing) which were addressed in a suggested patch in this session.

Testing

Run the executable with `./src/input.json` to see the computed safe sequence and whether the state is safe or unsafe. 

Example run (PowerShell):

```pwsh
g++ -std=c++17 -g ./src/process0.cpp -o process0.exe
./process0.exe ./src/input.json
```

Expected output (example):

```
Safe sequence computed.
P1 -> P3 -> P4 -> P0 -> P2
```