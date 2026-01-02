#include <benchmark/benchmark.h>
#include "mpicpp-lite/mpicpp-lite.h"

namespace mpi = mpicpp_lite;

// This reporter does nothing.
// We can use it to disable output from all but the root process
class NullReporter : public ::benchmark::BenchmarkReporter {
public:
    NullReporter() {}

    virtual bool
    ReportContext(const Context &)
    {
        return true;
    }

    virtual void
    ReportRuns(const std::vector<Run> &)
    {
    }

    virtual void
    Finalize()
    {
    }
};

int
main(int argc, char ** argv)
{
    mpi::Environment env;

    mpi::Communicator comm;

    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;

    if (comm.rank() == 0)
        // root process will use a reporter from the usual set provided by
        benchmark::RunSpecifiedBenchmarks();
    else {
        // reporting from other processes is disabled by passing a custom reporter
        NullReporter null;
        benchmark::RunSpecifiedBenchmarks(&null);
    }
}
