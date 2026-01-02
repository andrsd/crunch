#include <benchmark/benchmark.h>
#include "utils.h"
#include "common/types.h"
#include "mpicpp-lite/mpicpp-lite.h"

namespace mpi = mpicpp_lite;

namespace {

char * x = nullptr;

const int TAG = 1111;

} // namespace

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
send_recv(benchmark::State & state)
{
    mpi::Communicator comm;

    if (comm.size() % 2 == 0) {
        const size_t N = state.range(0);

        for (auto _ : state) {
            comm.barrier();
            auto t0 = mpi::wall_time();
            if (comm.rank() % 2 == 0) {
                comm.send(comm.rank() + 1, TAG, x, N);
            }
            else {
                auto s = comm.recv(comm.rank() - 1, TAG, x, N);
            }
            auto t1 = mpi::wall_time();

            auto max_elapsed_secs = max_elapsed_time(comm, t1 - t0);
            state.SetIterationTime(max_elapsed_secs);
        }

        state.counters["Bandwidth"] = benchmark::Counter(
            N,
            benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
    }
    else {
        // empty
        for (auto _ : state) {
        }
    }
}

static void
p2p_ping_pong(benchmark::State & state)
{
    mpi::Communicator comm;

    if (comm.size() % 2 == 0) {
        const size_t N = state.range(0);

        for (auto _ : state) {
            comm.barrier();
            auto t0 = mpi::wall_time();
            if (comm.rank() % 2 == 0) {
                comm.send(comm.rank() + 1, TAG, x, N);
                auto s = comm.recv(comm.rank() + 1, TAG, x, N);
            }
            else {
                auto s = comm.recv(comm.rank() - 1, TAG, x, N);
                comm.send(comm.rank() - 1, TAG, x, N);
            }
            auto t1 = mpi::wall_time();

            auto max_elapsed_secs = max_elapsed_time(comm, (t1 - t0) / 2.);
            state.SetIterationTime(max_elapsed_secs);
        }

        state.counters["Bandwidth"] = benchmark::Counter(
            N,
            benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
    }
    else {
        // empty
        for (auto _ : state) {
        }
    }
}

// clang-format off
BENCHMARK(send_recv)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1'000)
    ->Arg(10'000)
    ->Arg(100'000)
    ->Arg(1'000'000)
    ->Arg(10'000'000)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;

BENCHMARK(p2p_ping_pong)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1'000)
    ->Arg(10'000)
    ->Arg(100'000)
    ->Arg(1'000'000)
    ->Arg(10'000'000)
    ->Setup(do_setup)
    ->Teardown(do_teardown)
    ->UseManualTime()
;
// clang-format on
