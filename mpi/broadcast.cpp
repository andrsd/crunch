#include <benchmark/benchmark.h>
#include "utils.h"
#include "common/types.h"
#include "mpicpp-lite/mpicpp-lite.h"

namespace mpi = mpicpp_lite;

namespace {
char * x = nullptr;
}

static void
do_setup(const benchmark::State & state)
{
    std::srand(std::time({}));

    auto n = state.range(0);
    x = new char[n];
    for (i32 i = 0; i < n; ++i) {
        x[i] = std::rand();
    }
}

static void
do_teardown(const benchmark::State & state)
{
    delete[] x;
}

static void
broadcast(benchmark::State & state)
{
    mpi::Communicator comm;

    auto n = state.range(0);

    for (auto _ : state) {
        comm.barrier();
        auto t0 = mpi::wall_time();
        comm.broadcast(x, n, 0);
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }
}

// clang-format off
BENCHMARK(broadcast)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1'000)
    ->Arg(10'000)
    ->Arg(100'000)
    ->Arg(1'000'000)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
// clang-format on
