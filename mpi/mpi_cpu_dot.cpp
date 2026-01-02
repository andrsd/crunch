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

    for (i32 i = 0; i < n; ++i) {
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
mpi_cpu_dot_weak(benchmark::State & state)
{
    mpi::Communicator comm;

    auto n = state.range(0);

    for (auto _ : state) {
        comm.barrier();
        auto t0 = mpi::wall_time();
        double dot = 0.;
        for (i32 i = 0; i < n; ++i) {
            dot += x[i] * y[i];
        }
        double red;
        comm.all_reduce(dot, red, mpi::op::sum<double>());
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);

        benchmark::DoNotOptimize(red);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(comm.size() * 2. * n, state.iterations()),
                           benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(mpi_cpu_dot_weak)
    ->Arg(1'000'000)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
// clang-format on
