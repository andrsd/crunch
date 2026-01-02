#include <benchmark/benchmark.h>
#include "common/types.h"
#include <CL/sycl.hpp>

using namespace cl;
using namespace benchmark;

namespace {

sycl::queue * q;
double * x = nullptr;
double * d_x = nullptr;

void
do_setup(const State & state)
{
    q = new sycl::queue(sycl::default_selector_v, sycl::property::queue::in_order {});

    auto N = state.range(0);
    x = new double[N];
    d_x = sycl::malloc_device<double>(N, *q);

    q->copy(x, d_x, N);
    q->wait();
}

void
do_teardown(const State & state)
{
    delete[] x;
    sycl::free(d_x, *q);
    delete q;
}

void
sycl_nrm2(State & state)
{
    auto N = state.range(0);

    double alpha = std::rand();
    for (auto _ : state) {
        double result = 0.;
        auto * d_result = sycl::malloc_device<double>(1, *q);
        q->copy<double>(&result, d_result, 1).wait();

        // clang-format off
        q->submit([&](sycl::handler & h) {
            h.parallel_for(sycl::range<1>(N),
                           sycl::reduction(d_result, std::plus<double>()), [=](sycl::id<1> idx, auto & res) {
                res.combine(d_x[idx] * d_x[idx]);
            });
        });
        // clang-format on
        q->wait();

        q->copy<double>(d_result, &result, 1).wait();
        sycl::free(d_result, *q);

        result = std::sqrt(result);
        DoNotOptimize(result);
    }

    state.counters["FLOP/s"] =
        Counter(2. * N, Counter::kIsRate | Counter::kIsIterationInvariantRate);
}

} // namespace

// clang-format off
BENCHMARK(sycl_nrm2)->
    Arg(1 << 30)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
