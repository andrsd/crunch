#include <benchmark/benchmark.h>
#include "utils.h"
#include "mpicpp-lite/mpicpp-lite.h"

namespace mpi = mpicpp_lite;

static void
do_setup(const benchmark::State & state)
{
    std::srand(std::time({}));
}

static void
do_teardown(const benchmark::State & state)
{
}

static void
all_reduce_int_sum(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        int num = std::rand();
        int red;
        auto t0 = mpi::wall_time();
        comm.all_reduce(num, red, mpi::op::sum<int>());
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
all_reduce_int_max(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        int num = std::rand();
        int red;
        auto t0 = mpi::wall_time();
        comm.all_reduce(num, red, mpi::op::max<int>());
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
all_reduce_int_min(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        int num = std::rand();
        int red;
        auto t0 = mpi::wall_time();
        comm.all_reduce(num, red, mpi::op::min<int>());
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
all_reduce_double_sum(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        double num = (double) std::rand() / RAND_MAX;
        double red;
        auto t0 = mpi::wall_time();
        comm.all_reduce(num, red, mpi::op::sum<double>());
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
all_reduce_double_max(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        double num = std::rand();
        double red;
        auto t0 = mpi::wall_time();
        comm.all_reduce(num, red, mpi::op::max<double>());
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
all_reduce_double_min(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        double num = std::rand();
        double red;
        auto t0 = mpi::wall_time();
        comm.all_reduce(num, red, mpi::op::min<double>());
        auto t1 = mpi::wall_time();

        auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
        state.SetIterationTime(max_elapsed_secs);
    }
}

// clang-format off
BENCHMARK(all_reduce_int_sum)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(all_reduce_int_max)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(all_reduce_int_min)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;

BENCHMARK(all_reduce_double_sum)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(all_reduce_double_max)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(all_reduce_double_min)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
// clang-format on
