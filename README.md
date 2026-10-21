# Design and Analysis of Algorithms (DAA) Laboratory

This repository contains my C++ implementations for the Design and Analysis of Algorithms laboratory assignments. Each assignment applies an algorithm or design technique to a practical problem, with source code and any related files organized in its assignment folder.

## Assignments

| No. | Assignment | Main topic | What it demonstrates |
| ---: | --- | --- | --- |
| 1 | [Merge Sort](Assignment%20no%201/) | Divide and conquer | Sorts a retail transaction dataset and considers algorithm performance. |
| 2 | [Quick Sort](Assignment%20no%202/) | Divide and conquer | Sorts movie data using a selected movie attribute as the key. |
| 3 | [Emergency Relief Supply Distribution](Assignment%20no%203/) | Fractional Knapsack | Selects divisible relief supplies by value-to-weight ratio to maximize utility under a truck-capacity limit. |
| 4 | [Emergency Vehicle Shortest Path](Assignment%20no%204/) | Dijkstra’s algorithm | Finds shortest travel-time paths through a weighted road network. |
| 5 | [Multistage Delivery Route](Assignment%20no%205/) | Dynamic programming | Finds a minimum-cost route through a directed, weighted multistage graph. |
| 6 | [Disaster Relief Resource Allocation](Assignment%20no%206/) | 0/1 Knapsack | Selects indivisible relief supplies to maximize total utility without exceeding truck capacity. |
| 7 | [University Timetable Scheduling](Assignment%20no%207/) | Graph coloring | Assigns non-conflicting exam time slots from student-course enrollment conflicts; considers Greedy Coloring, DSATUR, Welsh-Powell, and room capacity. |
| 8 | [SwiftShip Delivery Route Optimization](Assignment%20no%208/) | TSP with Least-Cost Branch and Bound | Searches for a minimum-cost tour that visits each city once and returns to the start, using reduced-matrix lower bounds to prune the search. |

## Repository Structure

```text
DAA-Laboratory-Assignment-/
├── Assignment no 1/
├── Assignment no 2/
├── Assignment no 3/
├── Assignment no 4/
├── Assignment no 5/
├── Assignment no 6/
├── Assignment no 7/
├── Assignment no 8/
└── README.md
```

The files and input formats vary by assignment. Refer to the source code and any accompanying dataset or instructions inside the relevant folder.

## Requirements

- A C++ compiler with C++17 support, such as GCC or Clang.
- A terminal or IDE for compiling and running the programs.
- Any assignment-specific input files included in its folder.

## Build and Run

Clone the repository:

```bash
git clone https://github.com/aniket-gawande/DAA-Laboratory-Assignment-.git
cd DAA-Laboratory-Assignment-
```

Open the folder for the assignment you want to run, then compile its C++ source file. For example:

```bash
cd "Assignment no 8"
g++ -std=c++17 -O2 -Wall -Wextra <source_file.cpp> -o assignment
./assignment
```

Replace `<source_file.cpp>` with the C++ filename in that folder. On Windows, run the compiled program with:

```powershell
./assignment.exe
```

Some programs may read a dataset or request input in a particular format. Follow the prompts printed by the program and check the assignment folder for its required input files.

## Algorithms at a Glance

- **Divide and conquer:** Merge Sort and Quick Sort split data into smaller parts, solve those parts, and combine or partition the results.
- **Greedy strategy:** Fractional Knapsack repeatedly selects the available item with the best value-to-weight ratio.
- **Shortest paths:** Dijkstra’s algorithm computes shortest paths for nonnegative edge weights; the multistage graph solution uses dynamic programming over the graph’s stages.
- **Dynamic programming:** The 0/1 Knapsack solution compares choices to include or exclude each item while respecting capacity; the multistage graph solution computes minimum costs through successive stages.
- **Graph coloring:** Courses that share enrolled students are connected by conflict edges and must receive different exam slots.
- **Branch and Bound:** The TSP solution expands promising partial tours and prunes nodes whose lower bound cannot improve the best complete tour.

## Notes

- Results depend on the input data supplied to each program.
- Assignment 7 uses heuristic coloring methods where applicable; these can produce valid schedules without guaranteeing the minimum number of slots.
- Assignment 8 is an exact search method, but Branch and Bound can still take a long time as the number of cities grows.

## Author

**Aniket Ashokrao Gawande**  
PRN: `124B1F024`  
Subject: Design and Analysis of Algorithms  
Year and course: TY B.Tech Information Technology  
Batch: A2
