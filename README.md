# DSA0109 – Object Oriented Programming with C++ — Assignment 2 (CO2)

| Field | Details |
|---|---|
| Name | Shakthi M |
| Register Number | 192511197 |
| Course | DSA0109 – Object Oriented Programming with C++ |
| Assignment | Assignment 2 – CO2 (Functions and Object-Oriented Concepts) |
| Repository | https://github.com/shakthimani2703/DSA0109_C-_192511197_ASSIGNMENT_2 |

## Compiler and environment

* Compiler: **g++ (GCC) 13.3.0**, Ubuntu 24.04 (x86-64)
* Standard: C++17
* Compile command used for every program:

```bash
g++ -std=c++17 -Wall -Wextra <file>.cpp -o <file>
```

* Timing programs (`q1_alternative`, `q4_benchmark`, `q7_redesign`) add `-O2`.
* `q6_inline_benchmark` is built twice, with `-O0` and with `-O2`.
* `q3_ambiguous.cpp` is meant **not** to compile: it reproduces the ambiguous-call error.

## How to run

```bash
cd Task01
g++ -std=c++17 -Wall -Wextra q1_complete.cpp -o q1_complete
./q1_complete
```

Run any other program the same way from its task folder. Each folder has a `screenshots/` subfolder with a PNG of the terminal session and a `.txt` copy of the same output. `PREDICTIONS_BEFORE_RUNNING.txt` holds the output predictions, written before any program was compiled. None of the programs read input files; all test data is inside the programs.

## Folder structure

```
Task01/  Q1  Call by value, reference and pointer  (Hostel fee concession)
Task02/  Q2  Return by reference and its dangers   (Marks register)
Task03/  Q3  Default arguments and overloading     (Courier charges)
Task04/  Q4  Class with an array inside it         (Savings account mini-statement)
Task05/  Q5  Static members and arrays of objects  (College bus tickets)
Task06/  Q6  Friend and inline functions           (Train journey planner)
Task07/  Q7  Diagnose, redesign and validate       (Q4 redesigned as a ring buffer)
```

## Task summaries and results

| Task | Programs | Result |
|---|---|---|
| Q1 | `q1_three_ways`, `q1_predict`, `q1_complete`, `q1_alternative` | Call by value left the fee at 60000; reference and pointer changed it. The prediction `1 12 13 / 1 100 / 16 13 6` matched. The completed TODO prints 45000 then 40500. Passing a 4104-byte object by value cost about 30 ns per call, against about 0.6 ns by reference. |
| Q2 | `q2_return_ref`, `q2_dangling_local`, `q2_vector_growth`, `q2_alternative` | `highest(marks,N) = 95` changes the array. Returning a local by reference gives the `-Wreturn-local-addr` warning and crashed (segmentation fault). A vector reallocation moved the storage, so the old reference dangled; an index stayed valid. |
| Q3 | `q3_ambiguous`, `q3_redesign`, `q3_alternative` | The original design gave two "call of overloaded charge is ambiguous" errors. In the redesign (int grams / double kg / const Parcel&), all 7 predicted selections matched, including `charge(2)` → int and `charge(2.0f)` → double. |
| Q4 | `q4_savings_account`, `q4_benchmark` | Members defined both inside and outside the class (`::`). The shift and circular versions printed identical statements after 12 transactions. Shifting costs O(K): 8 ns at K=10 and 2138 ns at K=5000. Circular stays near 5 ns. |
| Q5 | `q5_bus_ticket_design`, `q5_predict`, `q5_complete`, `q5_function_twice`, `q5_alternative` | The prediction `101 103 104 4` matched. The completed TODOs give the expected output exactly. The static count survives function calls (3 issued after 2 calls). A TicketOffice object allows two buses at once. |
| Q6 | `q6_complete`, `q6_time_tests`, `q6_inline_benchmark`, `q6_alternative` | Prints `205 false`. All 7 tests pass (noon crossings, same time twice). At -O0 inline makes no difference; at -O2 inline/member/getter versions take about 15 ms per 10 million calls against about 18 ms for the forced non-inline call. |
| Q7 | `q7_redesign` | Q4's shifting array redesigned as a run-time-capacity ring buffer (`TransactionLog`). Statements are identical for 100,000 random transactions. It is 2.8x faster at K=10 and 639x faster at K=5000, for 16 extra bytes. |
