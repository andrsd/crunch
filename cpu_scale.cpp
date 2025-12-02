#include <benchmark/benchmark.h>
#include "types.h"

namespace {

double * y = nullptr;

} // namespace

static void
do_setup(const benchmark::State & state)
{
    std::srand(std::time({}));

    auto n = state.range(0);
    y = new double[n];

    for (i32 i = 0; i < n; ++i) {
        y[i] = std::rand();
    }
}

static void
do_teardown(const benchmark::State & state)
{
    delete[] y;
}

static void
cpu_scale(benchmark::State & state)
{
    auto n = state.range(0);

    double alpha = std::rand();
    for (auto _ : state) {
        for (i32 i = 0; i < n; ++i) {
            y[i] += alpha * y[i];
        }
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(n, state.iterations()), benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(cpu_scale)->
    Arg(1'000'000)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
