#pragma once

#include <benchmark/benchmark.h>
#include <cstdlib>
#include <cstdint>

using i8 = std::int8_t;
using i32 = std::uint32_t;

inline double
calc_flops(double flops_per_iter, benchmark::IterationCount iters)
{
    return flops_per_iter * iters;
}
