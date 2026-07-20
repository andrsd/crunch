#include <benchmark/benchmark.h>
#include "common/types.h"
#include "utils.h"
#include "mpicpp-lite/mpicpp-lite.h"
#include <chrono>

namespace mpi = mpicpp_lite;

namespace {

double * x = nullptr;
double * y = nullptr;

} // namespace

static void
do_setup(const benchmark::State & state)
{
    std::srand(std::time({}));

    auto n = state.range(0);
    x = new double[n];
    y = new double[n];

    for (i64 i = 0; i < n; ++i) {
        x[i] = std::rand();
        y[i] = std::rand();
    }
}

static void
do_teardown(const benchmark::State & state)
{
    delete[] x;
    delete[] y;
}

static void
mpi_cpu_axpy_weak(benchmark::State & state)
{
    mpi::Communicator comm;

    auto n = state.range(0);

    double alpha = std::rand();
    for (auto _ : state) {
        comm.barrier();
        auto t0 = mpi::wall_time();
        for (i64 i = 0; i < n; ++i) {
            y[i] = alpha * x[i] + y[i];
        }
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(comm.size() * 2 * n, state.iterations()),
                           benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(mpi_cpu_axpy_weak)
    ->Arg(1'000'000)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
// clang-format on
