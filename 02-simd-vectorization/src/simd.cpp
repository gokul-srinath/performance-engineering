
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>

using Clock = std::chrono::steady_clock;

// Scalar C++ implementation.
// GCC may automatically vectorize this loop.
void add(
    const float* A,
    const float* B,
    float* C,
    int N
) {
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }
}

// Calculate median execution time.
double calculate_median(std::vector<double>& times) {

    std::sort(times.begin(), times.end());

    size_t n = times.size();

    if (n % 2 == 0) {
        return (times[n / 2 - 1] + times[n / 2]) / 2.0;
    }

    return times[n / 2];
}

int main() {

    const int N = 10000000;
    const int WARMUP = 5;
    const int ITERATIONS = 30;

    std::vector<float> A(N, 10.0f);
    std::vector<float> B(N, 20.0f);
    std::vector<float> C(N);

    std::vector<double> execution_times;

    // Warm-up
    for (int i = 0; i < WARMUP; i++) {
        add(A.data(), B.data(), C.data(), N);
    }

    // Benchmark
    for (int i = 0; i < ITERATIONS; i++) {

        auto start = Clock::now();

        add(A.data(), B.data(), C.data(), N);

        auto end = Clock::now();

        double elapsed =
            std::chrono::duration<double, std::milli>(
                end - start
            ).count();

        execution_times.push_back(elapsed);
    }

    // Correctness verification
    bool correct = true;

    for (int i = 0; i < N; i++) {
        if (std::abs(C[i] - (A[i] + B[i])) > 1e-6f) {
            correct = false;
            break;
        }
    }

    double median_ms = calculate_median(execution_times);

    // Two input arrays read + one output array written.
    double bytes_processed = 3.0 * N * sizeof(float);

    double bandwidth_gbps =
        (bytes_processed / (median_ms / 1000.0)) / 1e9;

    std::cout << std::fixed << std::setprecision(4);

    std::cout << "\n===== SIMD Benchmark =====\n";

    std::cout << "Elements       : " << N << "\n";
    std::cout << "Warmup runs    : " << WARMUP << "\n";
    std::cout << "Benchmark runs : " << ITERATIONS << "\n";

    std::cout << "Median time    : "
              << median_ms << " ms\n";

    std::cout << "Throughput     : "
              << bandwidth_gbps << " GB/s\n";

    std::cout << "Correctness    : "
              << (correct ? "PASSED" : "FAILED") << "\n";

    std::cout << "Sample output  : "
              << C[0] << "\n";

    return correct ? 0 : 1;
}
