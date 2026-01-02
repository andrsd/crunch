#include <benchmark/benchmark.h>
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
reduce_int_sum(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        int num = std::rand();
        int red;
        auto t0 = mpi::wall_time();
        comm.reduce(num, red, mpi::op::sum<int>(), 0);
        auto t1 = mpi::wall_time();

        double max_elapsed_secs;
        comm.all_reduce(t1 - t0, max_elapsed_secs, mpi::op::max<double>());
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
reduce_int_max(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        int num = std::rand();
        int red;
        auto t0 = mpi::wall_time();
        comm.reduce(num, red, mpi::op::max<int>(), 0);
        auto t1 = mpi::wall_time();

        double max_elapsed_secs;
        comm.all_reduce(t1 - t0, max_elapsed_secs, mpi::op::max<double>());
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
reduce_int_min(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        int num = std::rand();
        int red;
        auto t0 = mpi::wall_time();
        comm.reduce(num, red, mpi::op::min<int>(), 0);
        auto t1 = mpi::wall_time();

        double max_elapsed_secs;
        comm.all_reduce(t1 - t0, max_elapsed_secs, mpi::op::max<double>());
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
reduce_double_sum(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        double num = (double) std::rand() / RAND_MAX;
        double red;
        auto t0 = mpi::wall_time();
        comm.reduce(num, red, mpi::op::sum<double>(), 0);
        auto t1 = mpi::wall_time();

        double max_elapsed_secs;
        comm.all_reduce(t1 - t0, max_elapsed_secs, mpi::op::max<double>());
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
reduce_double_max(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        double num = std::rand();
        double red;
        auto t0 = mpi::wall_time();
        comm.reduce(num, red, mpi::op::max<double>(), 0);
        auto t1 = mpi::wall_time();

        double max_elapsed_secs;
        comm.all_reduce(t1 - t0, max_elapsed_secs, mpi::op::max<double>());
        state.SetIterationTime(max_elapsed_secs);
    }
}

static void
reduce_double_min(benchmark::State & state)
{
    mpi::Communicator comm;

    for (auto _ : state) {
        comm.barrier();
        double num = std::rand();
        double red;
        auto t0 = mpi::wall_time();
        comm.reduce(num, red, mpi::op::min<double>(), 0);
        auto t1 = mpi::wall_time();

        double max_elapsed_secs;
        comm.all_reduce(t1 - t0, max_elapsed_secs, mpi::op::max<double>());
        state.SetIterationTime(max_elapsed_secs);
    }
}

// clang-format off
BENCHMARK(reduce_int_sum)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(reduce_int_max)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(reduce_int_min)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;

BENCHMARK(reduce_double_sum)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(reduce_double_max)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
BENCHMARK(reduce_double_min)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
// clang-format on
