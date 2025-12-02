#include <benchmark/benchmark.h>
#ifdef __APPLE__
    #include <vecLib/cblas.h>
#else
    #include <cblas.h>
#endif
#include "types.h"
#include <chrono>

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
blas_nrm2(benchmark::State & state)
{
    auto n = state.range(0);

    for (auto _ : state) {
        double nrm2 = 0.;
        nrm2 = cblas_dnrm2(n, x, 1);
        benchmark::DoNotOptimize(nrm2);
    }

    state.counters["FLOP/s"] =
        benchmark::Counter(calc_flops(2. * n, state.iterations()), benchmark::Counter::kIsRate);
}

// clang-format off
BENCHMARK(blas_nrm2)->
    Arg(1'000'000)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
