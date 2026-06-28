#include <array>
#include <string_view>
#include <format>
#include <iostream>
#include "fns.h"

template <int N>
std::array<double, 4>
find_minumum(const std::array<std::array<double, 4>, N> & times)
{
    constexpr auto MAX_DBL = std::numeric_limits<double>::max();
    std::array<double, 4> min_time = { MAX_DBL, MAX_DBL, MAX_DBL, MAX_DBL };
    for (int k = 0; k < N; k++)
        for (int j = 0; j < 4; j++)
            min_time[j] = std::min(min_time[j], times[k][j]);
    return min_time;
}

std::array<double, 4>
calculate_rate(const std::array<double, 4> & bytes, const std::array<double, 4> & min_time)
{
    std::array<double, 4> irate;
    for (int j = 0; j < 4; j++)
        irate[j] = 1e-6 * bytes[j] / min_time[j];
    return irate;
}

void
print_results(const std::array<double, 4> & irate)
{
    constexpr std::array<std::string_view, 4> label = { "Copy", "Scale", "Add", "Triad" };

    std::cout << std::format("{:<13} {:>7}\n", "Function", "Rate [MB/s]");
    std::cout << "-------------------------\n";
    for (int j = 0; j < 4; j++)
        std::cout << std::format("{:<13} {:>11.2f}\n", label[j], irate[j]);
}

void
benchmark()
{
    // this gives 1GB per array
    constexpr int N = 125'000'000;
    constexpr int NTIMES = 50;

    std::array<double, 4> BYTES = { 2 * sizeof(double) * N,
                                    2 * sizeof(double) * N,
                                    3 * sizeof(double) * N,
                                    3 * sizeof(double) * N };

    std::vector<double> A(N, 1.);
    std::vector<double> B(N, 2.);
    std::vector<double> C(N, 0.);
    const double SCALAR = 3.0;

    std::span<double> a(A);
    std::span<double> b(B);
    std::span<double> c(C);
    std::array<std::array<double, 4>, NTIMES> times;
    for (int k = 0; k < NTIMES; k++) {
        times[k][0] = run_prg(copy<N>, c, a);
        times[k][1] = run_prg(scale<N>, b, SCALAR, c);
        times[k][2] = run_prg(add<N>, c, a, b);
        times[k][3] = run_prg(triad<N>, a, b, SCALAR, c);
    }

    // process results
    auto min_time = find_minumum<NTIMES>(times);
    auto irate = calculate_rate(BYTES, min_time);

    print_results(irate);
}

int
main()
{
    benchmark();
    return 0;
}
