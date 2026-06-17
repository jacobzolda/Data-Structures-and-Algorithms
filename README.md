# Data Structures & Algorithms (C)

Three programming assignments from CS-260 (Data Structures & Algorithms) at Drexel University, implementing core data structures from scratch in C.

## PA1 — Dynamic Array List (`pa1-dynamic-list/`)
A generic resizable list (`List`/`Entry` structs) supporting insert/delete at head, tail, or an arbitrary position, plus positional search — doubling capacity on growth and shifting elements on insert/delete. Driven by a simple command file (`input1.txt`/`input2.txt`).

## PA2 — Movie Script Search: Quicksort + Binary Search (`pa2-movie-script-search/`)
Implements quicksort from scratch (`myQuickSort`/`partition`/`swap`) to sort a movie-script word list, then `myBinarySearch` to answer ~9,000 lookup queries (`queries_9k.txt`) against both a pre-sorted and a shuffled dataset.

## PA3 — Spell-Checker Hash Table (`pa3-hash-table/`)
An open-hashing hash table (linked-list buckets) built from a dictionary word list (`words.txt`, ~3.7k words), used to spell-check an input text file and optionally insert unrecognized words back into the dictionary.

## Tech

C

## Running

Each is a single-file C program:
```sh
gcc pa1-dynamic-list/main.c -o pa1 && ./pa1 input1.txt
gcc pa2-movie-script-search/main.c -o pa2 && ./pa2 movieScripts_alphaSorted.txt queries_9k.txt
gcc pa3-hash-table/main.c -o pa3 && ./pa3 words.txt test.txt add
```
(exact CLI args inferred from each `main()` — double-check against your own usage notes if these don't match.)

---
*Completed as coursework for CS-260 at Drexel University; shared for portfolio purposes. Assignment PDFs, lecture slides, and the auto-grading scripts provided with each assignment have been removed — this contains only my own implementation.*
