#include "timer.h"
#include <thread>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

unsigned int n_threads = 1;

template <typename Index, typename Func>
void
parallel_for(Index start, Index end, Func && func)
{
    // const unsigned int num_threads = std::max(1u, std::thread::hardware_concurrency());
    const Index total_elements = end - start;

    if (total_elements <= 0)
        return;

    // If work is too small, just run sequentially on the calling thread
    if (total_elements < n_threads) {
        for (Index i = start; i < end; ++i) {
            func(i);
        }
        return;
    }

    const Index chunk_size = total_elements / n_threads;
    std::vector<std::jthread> workers;
    workers.reserve(n_threads);

    for (unsigned int t = 0; t < n_threads; ++t) {
        Index chunk_start = start + t * chunk_size;
        // Ensure the last thread catches any remainder elements
        Index chunk_end = (t == n_threads - 1) ? end : chunk_start + chunk_size;

        workers.emplace_back([chunk_start, chunk_end, &func]() {
            for (Index i = chunk_start; i < chunk_end; ++i) {
                func(i);
            }
        });
    }
}

/// Return bandwidth in MB/s
double
benchmark()
{
    // this gives 1GB per array
    constexpr int N = 125'000'000;
    constexpr int NTIMES = 50;
    const double BYTES = 3 * sizeof(double) * N;

    std::vector<double> A(N, 1.);
    std::vector<double> B(N, 2.);
    std::vector<double> C(N, 3.);
    const double ALPHA = 3.0;

    std::array<double, NTIMES> times;
    for (int k = 0; k < NTIMES; ++k) {
        Timer tmr;
        parallel_for(0, N, [&](int idx) {
            //
            A[idx] = B[idx] + ALPHA * C[idx];
        });
        times[k] = tmr.seconds();
    }

    auto min_time = std::numeric_limits<double>::max();
    // note: skip first iteration
    for (int k = 1; k < NTIMES; ++k)
        min_time = std::min(min_time, times[k]);

    return 1e-6 * BYTES / min_time;
}

int
main(int argc, char * argv[])
{
    char * env = getenv("STREAMS_NUM_THREADS");
    if (env) {
        sscanf(env, "%d", &n_threads);
    }

    std::string fifo_path = "";
    if (argc > 1)
        fifo_path = argv[1];

    auto triad_bandwidth = benchmark();

    if (fifo_path.empty()) {
        std::cout << "Triad bandwidth: " << triad_bandwidth << " MB/s" << std::endl;
    }
    else {
        auto fd = open(fifo_path.c_str(), O_WRONLY);
        if (fd == -1) {
            std::cerr << "Failed to open FIFO for writing\n";
            return 1;
        }

        if (write(fd, &triad_bandwidth, sizeof(triad_bandwidth)) == -1) {
            std::cerr << "Error writing to FIFO\n";
        }

        close(fd);
    }

    return 0;
}
