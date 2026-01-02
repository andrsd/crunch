#include <benchmark/benchmark.h>
#include "common/types.h"
#include <chrono>

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
cpu_dot(benchmark::State & state)
{
    auto n = state.range(0);

    for (auto _ : state) {
        double dot = 0.;
        for (i32 i = 0; i < n; ++i) {
            dot += x[i] * y[i];
        }

        benchmark::DoNotOptimize(dot);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(2. * n, state.iterations()), benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(cpu_dot)->
    Arg(1'000'000)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
