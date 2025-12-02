#pragma once

#include <benchmark/benchmark.h>
#include <cstdlib>

using i32 = std::uint32_t;

inline double
calc_gflops(double flops_per_iter, benchmark::IterationCount iters)
{
    return flops_per_iter * iters / 1e9;
}
