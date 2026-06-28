#pragma once

#include <span>
#include "timer.h"

template <int N>
inline void
copy(std::span<double> & c, const std::span<double> & a)
{
    for (int j = 0; j < N; j++)
        c[j] = a[j];
}

template <int N>
inline void
scale(std::span<double> & b, double alpha, const std::span<double> & c)
{
    for (int j = 0; j < N; j++)
        b[j] = alpha * c[j];
}

template <int N>
inline void
add(std::span<double> & c, const std::span<double> & a, const std::span<double> & b)
{
    for (int j = 0; j < N; j++)
        c[j] = a[j] + b[j];
}

template <int N>
inline void
triad(std::span<double> & a, const std::span<double> & b, double alpha, const std::span<double> & c)
{
    for (int j = 0; j < N; j++)
        a[j] = b[j] + alpha * c[j];
}

template <typename FUNC, typename... ARGS>
inline double
run_prg(FUNC F, ARGS &&... args)
{
    Timer timer;
    F(std::forward<ARGS>(args)...);
    return timer.seconds();
}
