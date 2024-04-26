Paging Algorithms
This program simulates different paging algorithms used in operating systems to manage memory. It includes implementations of the First-In-First-Out (FIFO), Least Recently Used (LRU), and Belady's Optimal (OPT) page replacement algorithms.

Overview
The program reads an address stream from a file and simulates the paging algorithms to manage memory pages. It calculates the number of page faults incurred by each algorithm and displays the frame table after each memory access.

Features
Simulates FIFO, LRU, and OPT page replacement algorithms.
Reads address stream from a file.
Calculates and displays the frame table after each memory access.
Reports the number of page faults incurred by each algorithm.

How to Use
Compile the program.
Prepare an input file containing the address stream (e.g., "address.txt").
Run the program.
The program will display the address stream and simulate each paging algorithm, showing the frame table and reporting the number of page faults.

Usage
$ gcc -o paging paging.c
$ ./paging
Input File Format
The input file should contain a list of memory addresses separated by whitespace (e.g., newline or space).

Paging Algorithms
FIFO (First-In-First-Out)
This algorithm replaces the oldest page in memory when a page fault occurs.

LRU (Least Recently Used)
This algorithm replaces the least recently used page in memory when a page fault occurs.

OPT (Belady's Optimal)
This algorithm replaces the page that will not be used for the longest time in the future when a page fault occurs.

License
This project is licensed under the [MIT License](LICENSE).
