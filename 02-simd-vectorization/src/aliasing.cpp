
#include <iostream>
#include <vector>
#include <chrono>

void add_normal(
    float* A,
    float* B,
    float* C,
    int N
) {
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }
}

void add_restrict(
    float* __restrict__ A,
    float* __restrict__ B,
    float* __restrict__ C,
    int N
) {
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    const int N = 10000000;

    std::vector<float> A(N, 10.0f);
    std::vector<float> B(N, 20.0f);
    std::vector<float> C(N);

    auto start = std::chrono::high_resolution_clock::now();

    add_normal(A.data(), B.data(), C.data(), N);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Normal: "
              << std::chrono::duration<double>(end - start).count()
              << " seconds\n";

    start = std::chrono::high_resolution_clock::now();

    add_restrict(A.data(), B.data(), C.data(), N);

    end = std::chrono::high_resolution_clock::now();

    std::cout << "Restrict: "
              << std::chrono::duration<double>(end - start).count()
              << " seconds\n";

    std::cout << "Result: " << C[0] << "\n";
}
