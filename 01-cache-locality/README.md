# Cache Locality Experiment

This benchmark compares how memory-access patterns affect the time needed to
read an array of 32-bit integers. It runs the same number of reads in three
modes:

- `sequential`: reads adjacent elements, making use of spatial locality.
- `random`: reads elements in a shuffled order, reducing predictable locality.
- `stride16`: reads every 16th element, a 64-byte step for 32-bit integers.

The program initializes an array of 100,000,000 integers (about 400 MB) and
performs 6,250,000 reads per run. It reports the elapsed time for five runs,
their median, and a checksum to keep the reads observable. Data and indices
are prepared before timing; the random mode's shuffled index array is also
prepared before the benchmark starts.

## Build

Requires a C++17 compiler and a POSIX-compatible system (the program uses
`unistd.h` to print its process ID). Run these commands from the
`01-cache-locality` directory:

```sh
g++ -O2 -std=c++17 src/memory_access.cpp -o src/memory_access
```

## Run

Choose one mode:

```sh
./src/memory_access sequential
./src/memory_access random
./src/memory_access stride16
```

After setup, the program prints its PID and waits for Enter before starting
the benchmark. This gives time to attach a profiler such as Linux `perf`.

The results depend on the machine, compiler, optimization flags, and system
load. Compare modes on the same machine and under similar conditions; timings
are an observation of the full access pattern, not a direct measurement of
cache misses.
