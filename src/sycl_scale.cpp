#include <benchmark/benchmark.h>
#include "types.h"
#include <CL/sycl.hpp>

using namespace cl;
using namespace benchmark;

namespace {

sycl::queue * q;
double * y = nullptr;
double * d_y = nullptr;

void
do_setup(const State & state)
{
    q = new sycl::queue(sycl::default_selector_v, sycl::property::queue::in_order {});

    auto N = state.range(0);
    y = new double[N];
    d_y = sycl::malloc_device<double>(N, *q);

    q->copy(y, d_y, N);
    q->wait();
}

void
do_teardown(const State & state)
{
    delete[] y;
    sycl::free(d_y, *q);
    delete q;
}

void
sycl_scale(State & state)
{
    auto N = state.range(0);

    double alpha = std::rand();
    for (auto _ : state) {
        q->submit([&](sycl::handler & h) {
            h.parallel_for(sycl::range<1>(N), [=](sycl::id<1> idx) {
                //
                d_y[idx] *= alpha;
            });
        });
        q->wait();
    }

    state.counters["FLOP/s"] = Counter(N, Counter::kIsRate | Counter::kIsIterationInvariantRate);
}

} // namespace

// clang-format off
BENCHMARK(sycl_scale)->
    Arg(1 << 30)->
    Setup(do_setup)->
    Teardown(do_teardown)
;
// clang-format on
