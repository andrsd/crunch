#include <benchmark/benchmark.h>
#ifdef __APPLE__
    #include <vecLib/cblas.h>
#else
    #include <cblas.h>
#endif
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
blas_dot(benchmark::State & state)
{
    auto n = state.range(0);

    for (auto _ : state) {
        double dot = 0.;
        dot = cblas_ddot(n, x, 1, y, 1);
        benchmark::DoNotOptimize(dot);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(2. * n, state.iterations()), benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(blas_dot)->
    Arg(1'000'000)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
