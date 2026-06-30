#include <benchmark/benchmark.h>
#include "common/types.h"
#include <ctime>

constexpr i64 N = 80'000'000;
constexpr i64 N_ITERS = 50;
/// Number of iteration to run before benchmarking to warm up caches
constexpr i64 N_PRIME_ITERS = 5;

namespace {

double * a = nullptr;
double * b = nullptr;
double * c = nullptr;

} // namespace

static void
do_setup(const benchmark::State & state)
{
    std::srand(std::time({}));

    auto n = state.range(0);
    a = new double[n];
    b = new double[n];
    c = new double[n];
}

static void
do_teardown(const benchmark::State & state)
{
    delete[] a;
    delete[] b;
    delete[] c;
}

static void
cpu_stream_copy(benchmark::State & state)
{
    auto n = state.range(0);

    for (i64 j = 0; j < N_PRIME_ITERS; j++) {
        for (i64 i = 0; i < n; ++i)
            c[i] = a[i];
    }

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i)
            c[i] = a[i];
    }

    i64 bytes = n * sizeof(double) * 2;
    state.SetBytesProcessed(state.iterations() * bytes);
}

static void
cpu_stream_scale(benchmark::State & state)
{
    auto n = state.range(0);

    const double alpha = 3.0;

    for (i64 j = 0; j < N_PRIME_ITERS; j++) {
        for (i64 i = 0; i < n; ++i)
            b[i] = alpha * c[i];
    }

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i)
            b[i] = alpha * c[i];
    }

    i64 bytes = n * sizeof(double) * 2;
    state.SetBytesProcessed(state.iterations() * bytes);
}

static void
cpu_stream_add(benchmark::State & state)
{
    auto n = state.range(0);

    for (i64 j = 0; j < N_PRIME_ITERS; j++) {
        for (i64 i = 0; i < n; ++i)
            c[i] = a[i] + b[i];
    }

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i)
            c[i] = a[i] + b[i];
    }

    i64 bytes = n * sizeof(double) * 3;
    state.SetBytesProcessed(state.iterations() * bytes);
}

static void
cpu_stream_triad(benchmark::State & state)
{
    auto n = state.range(0);

    const double alpha = 3.0;

    for (i64 j = 0; j < N_PRIME_ITERS; j++) {
        for (i64 i = 0; i < n; ++i)
            a[i] = b[i] + alpha * c[i];
    }

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i)
            a[i] = b[i] + alpha * c[i];
    }

    i64 bytes = n * sizeof(double) * 3;
    state.SetBytesProcessed(state.iterations() * bytes);
}

// clang-format off
BENCHMARK(cpu_stream_copy)->
    Arg(N)->
    Iterations(N_ITERS)->
    Setup(do_setup)->
    Teardown(do_teardown)
;

BENCHMARK(cpu_stream_scale)->
    Arg(N)->
    Iterations(N_ITERS)->
    Setup(do_setup)->
    Teardown(do_teardown)
;

BENCHMARK(cpu_stream_add)->
    Arg(N)->
    Iterations(N_ITERS)->
    Setup(do_setup)->
    Teardown(do_teardown)
;

BENCHMARK(cpu_stream_triad)->
    Arg(N)->
    Iterations(N_ITERS)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
