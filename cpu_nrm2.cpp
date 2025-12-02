#include <benchmark/benchmark.h>
#include "types.h"
#include <chrono>
#include <cmath>

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
cpu_nrm2(benchmark::State & state)
{
    auto n = state.range(0);

    for (auto _ : state) {
        double nrm2 = 0.;
        for (i32 i = 0; i < n; ++i) {
            nrm2 += x[i] * x[i];
        }
        nrm2 = std::sqrt(nrm2);

        benchmark::DoNotOptimize(nrm2);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(2. * n, state.iterations()), benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(cpu_nrm2)->
    Arg(1'000'000)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
