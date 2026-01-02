#include <benchmark/benchmark.h>
#include "common/types.h"
#include "utils.h"
#include "mpicpp-lite/mpicpp-lite.h"
#include <chrono>
#include <cmath>

namespace mpi = mpicpp_lite;

namespace {

double * x = nullptr;

} // namespace

static void
do_setup(const benchmark::State & state)
{
    std::srand(std::time({}));

    auto n = state.range(0);
    x = new double[n];

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
mpi_cpu_nrm2_weak(benchmark::State & state)
{
    mpi::Communicator comm;

    auto n = state.range(0);

    for (auto _ : state) {
        comm.barrier();
        auto t0 = mpi::wall_time();
        double nrm2 = 0.;
        for (i32 i = 0; i < n; ++i) {
            nrm2 += x[i] * x[i];
        }
        double gnrm2;
        comm.all_reduce(nrm2, gnrm2, mpi::op::sum<double>());
        gnrm2 = std::sqrt(gnrm2);
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);

        benchmark::DoNotOptimize(gnrm2);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(comm.size() * 2. * n, state.iterations()),
                           benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(mpi_cpu_nrm2_weak)
    ->Arg(1'000'000)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
// clang-format on
