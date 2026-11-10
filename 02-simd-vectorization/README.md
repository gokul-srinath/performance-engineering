# SIMD Vectorization

This experiment benchmarks element-wise addition of two arrays of 10 million
floats. The `add` loop is written as ordinary scalar C++; GCC can automatically
vectorize it when optimization is enabled. The program performs warm-up and
timed iterations, reports the median time and estimated memory throughput, and
checks the output.

## Build and inspect vectorization

Run from the `02-simd-vectorization` directory. This builds with `-O3`, enables
optimizations for the current machine with `-march=native`, and asks GCC to
print loops it successfully vectorizes:

```sh
g++ -O3 -march=native \
    -fopt-info-vec-optimized \
    src/simd.cpp -o simd_O3
```

## Run

```sh
./simd_O3
```

The program prints the median execution time, estimated throughput, and a
correctness result. The vectorization report is emitted during compilation.
Because `-march=native` targets the machine compiling the program, the
resulting binary may not run on older or different CPU architectures.

## Related aliasing example

`src/aliasing.cpp` compares an ordinary addition function with one whose
pointers use GCC's `__restrict__` extension:

```sh
g++ -O3 -march=native src/aliasing.cpp -o aliasing
./aliasing
```

Benchmark results depend on the compiler, CPU, and system load. Compare runs
under similar conditions.
