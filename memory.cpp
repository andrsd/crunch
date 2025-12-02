#include <benchmark/benchmark.h>
#include <vector>

namespace {

void
mem_read_seq(benchmark::State & state)
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
mem_write_seq(benchmark::State & state)
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
mem_copy_seq(benchmark::State & state)
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

void
mem_read_random(benchmark::State & state)
{
    const size_t N = state.range(0);
    std::vector<char> buf(N, 1);

    volatile size_t sum = 0; // prevent optimization

    for (auto _ : state) {
        for (size_t i = 0; i < N; i += 64) {
            auto j = std::rand() % N;
            sum += buf[j];
        }
    }

    // bytes read per iteration
    state.counters["Bytes/s"] = benchmark::Counter(
        N,
        benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
}

void
mem_write_random(benchmark::State & state)
{
    const size_t N = state.range(0);
    std::vector<char> buf(N, 1);

    for (auto _ : state) {
        for (size_t i = 0; i < N; i += 64) {
            auto j = std::rand() % N;
            buf[j] = 123;
        }
        benchmark::ClobberMemory(); // prevent write elimination
    }

    state.counters["Bytes/s"] = benchmark::Counter(
        N,
        benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
}

void
mem_copy_random(benchmark::State & state)
{
    const size_t N = state.range(0);
    std::vector<char> src(N, 1);
    std::vector<char> dst(N, 2);

    for (auto _ : state) {
        for (size_t i = 0; i < N; i += 64) {
            auto j = std::rand() % N;
            dst[j] = src[j];
        }
        benchmark::ClobberMemory();
    }

    state.counters["Bytes/s"] = benchmark::Counter(
        N,
        benchmark::Counter::kIsRate | benchmark::Counter::kIsIterationInvariantRate);
}

} // namespace

// clang-format off
BENCHMARK(mem_copy_seq)->
    Arg(1 << 30)             // 1GB
;
BENCHMARK(mem_read_seq)->
    Arg(1 << 30)             // 1GB
;
BENCHMARK(mem_write_seq)->
    Arg(1 << 30)             // 1GB
;

BENCHMARK(mem_copy_random)->
    Arg(1 << 30)             // 1GB
;
BENCHMARK(mem_read_random)->
    Arg(1 << 30)             // 1GB
;
BENCHMARK(mem_write_random)->
    Arg(1 << 30)             // 1GB
;
// clang-format on
