/// benchmark raw floating point operations without touching memory

#include <benchmark/benchmark.h>
#include "common/types.h"
#include <chrono>

namespace {

double * x = nullptr;
double * y = nullptr;

void
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

void
do_teardown(const benchmark::State & state)
{
    delete[] x;
    delete[] y;
}

void
raw_flops(benchmark::State & state)
{
    auto N = state.range(0);

    double a = 1.;
    double b = 2.;
    double c = 3.;
    double d = 4.;
    for (auto _ : state) {
        double res = 0;
        for (i64 i = 0; i < N; ++i) {
            b += a;
            c *= b;
            d += c;
            a /= d;
            res += a;
        }
        benchmark::DoNotOptimize(res);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(5 * N, state.iterations()), benchmark::Counter::kIsRate);
}

} // namespace

// clang-format off
BENCHMARK(raw_flops)->
    Arg(1'000)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
