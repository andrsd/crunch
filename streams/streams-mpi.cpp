#include "timer.h"
#include "mpicpp-lite/mpicpp-lite.h"
#include <fcntl.h>
#include <unistd.h>

namespace mpi = mpicpp_lite;

/// Split global amount of work N into local amount of work
int
split_ownership(mpi::Communicator comm, int N)
{
    auto size = comm.size();
    auto rank = comm.rank();
    return N / size + ((N % size) > rank);
}

/// Return bandwidth in MB/s
double
benchmark(mpi::Communicator comm)
{
    // this gives 1GB per array
    constexpr int N = 125'000'000;
    constexpr int NTIMES_INNER = 1;
    constexpr int NTIMES = 50;
    const double BYTES = 3 * sizeof(double) * N;

    int n = split_ownership(comm, N);

    std::vector<double> A(n, 1.);
    std::vector<double> B(n, 2.);
    std::vector<double> C(n, 3.);
    const double ALPHA = 3.0;

    std::array<double, NTIMES> times;
    for (int k = 0; k < NTIMES; ++k) {
        comm.barrier();
        {
            Timer tmr;
            for (int l = 0; l < NTIMES_INNER; l++) {
                for (int j = 0; j < n; j++)
                    A[j] = B[j] + ALPHA * C[j];
            }
            times[k] = tmr.seconds();
        }
    }
    // comm.all_reduce(times, mpi::op::max<double>());
    MPI_Allreduce(MPI_IN_PLACE, times.data(), NTIMES, MPI_DOUBLE, MPI_MAX, comm);

    auto min_time = std::numeric_limits<double>::max();
    // note: skip first iteration
    for (int k = 1; k < NTIMES; ++k)
        min_time = std::min(min_time, times[k]);

    return 1e-6 * BYTES * NTIMES_INNER / min_time;
}

int
main(int argc, char * argv[])
{
    mpi::Environment env(argc, argv);
    mpi::Communicator comm;

    std::string fifo_path = "";
    if (argc > 1)
        fifo_path = argv[1];

    auto triad_bandwidth = benchmark(comm);

    if (comm.rank() == 0) {
        if (fifo_path.empty()) {
            std::cout << "Triad bandwidth: " << triad_bandwidth << " MB/s" << std::endl;
        }
        else {
            auto fd = open(fifo_path.c_str(), O_WRONLY);
            if (fd == -1) {
                std::cerr << "Failed to open FIFO for writing\n";
                return 1;
            }

            if (write(fd, &triad_bandwidth, sizeof(triad_bandwidth)) == -1) {
                std::cerr << "Error writing to FIFO\n";
            }

            close(fd);
        }
    }

    return 0;
}
