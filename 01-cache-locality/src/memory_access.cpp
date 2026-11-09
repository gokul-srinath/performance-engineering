
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

constexpr size_t N = 100'000'000;
constexpr int REPEATS = 5;
constexpr size_t ELEMENTS = N / 16;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "Usage: ./memory_access <sequential|random|stride16>\n";
        return 1;
    }

    string mode = argv[1];

    if (mode != "sequential" && mode != "random" && mode != "stride16")
    {
        cerr << "Invalid mode\n";
        return 1;
    }

    cout << "Mode: " << mode << "\n";

    // Allocate and initialize data
    vector<uint32_t> data(N);

    for (size_t i = 0; i < N; i++)
    {
        data[i] = static_cast<uint32_t>(i % 1024);
    }

    // Prepare random indices only when required
    vector<uint32_t> indices;

    if (mode == "random")
    {
        indices.resize(N);
        iota(indices.begin(), indices.end(), 0);

        mt19937 rng(42);
        shuffle(indices.begin(), indices.end(), rng);
    }
    cout << "PID: " << getpid() << "\n";
    cout << "Setup complete. Attach perf now.\n";
    cout << "Press ENTER to start benchmark...\n";
    cin.get();

    cout << "Elements per run: " << ELEMENTS << "\n";

    vector<double> times;
    uint64_t checksum = 0;

    for (int run = 0; run < REPEATS; run++)
    {

        uint64_t sum = 0;

        auto start = steady_clock::now();

        if (mode == "sequential")
        {
            for (size_t i = 0; i < ELEMENTS; i++)
            {
                sum += data[i];
            }
        }
        else if (mode == "random")
        {
            for (size_t i = 0; i < ELEMENTS; i++)
            {
                sum += data[indices[i]];
            }
        }
        else
        {
            for (size_t i = 0; i < ELEMENTS; i++)
            {
                sum += data[i * 16];
            }
        }

        auto end = steady_clock::now();

        double elapsed =
            duration<double, milli>(end - start).count();

        times.push_back(elapsed);
        checksum = sum;

        cout << "Run " << run + 1
             << ": " << elapsed << " ms\n";
    }

    sort(times.begin(), times.end());

    cout << "\nMedian: "
         << times[REPEATS / 2] << " ms\n";

    cout << "Checksum: " << checksum << "\n";

    return 0;
}
