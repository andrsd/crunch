#include <benchmark/benchmark.h>
#include <vector>

namespace {

void
mem_read(benchmark::State & state)
{
    const size_t N = state.range(0);
    std::vector<char> buf(N, 1);

    volatile size_t sum = 0; // prevent optimization

    for (auto _ : state) {
        for (size_t i = 0; i < N; i += 64) {
            // step = cache line
            sum += buf[i];
        }
    }

    // bytes read per iteration
    state.counters["Bytes/s"] = benchmark::Counter(
        N,
        benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
}

void
mem_write(benchmark::State & state)
{
    const size_t N = state.range(0);
    std::vector<char> buf(N, 1);

    for (auto _ : state) {
        for (size_t i = 0; i < N; i += 64) {
            buf[i] = 123;
        }
        benchmark::ClobberMemory(); // prevent write elimination
    }

    state.counters["Bytes/s"] = benchmark::Counter(
        N,
        benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
}

void
mem_copy(benchmark::State & state)
{
    const size_t N = state.range(0);
    std::vector<char> src(N, 1);
    std::vector<char> dst(N, 2);

    for (auto _ : state) {
        memcpy(dst.data(), src.data(), N);
        benchmark::ClobberMemory();
    }

    state.counters["Bytes/s"] = benchmark::Counter(
        N,
        benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
}

} // namespace

// clang-format off
BENCHMARK(mem_copy)->
    Arg(1 << 30)             // 1GB
;
BENCHMARK(mem_read)->
    Arg(1 << 30)             // 1GB
;
BENCHMARK(mem_write)->
    Arg(1 << 30)             // 1GB
;
// clang-format on
