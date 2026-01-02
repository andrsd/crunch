#pragma once

#include "mpicpp-lite/mpicpp-lite.h"

namespace mpi = mpicpp_lite;

inline double
max_elapsed_time(mpi::Communicator comm, double time)
{
    double max_elapsed_secs;
    comm.all_reduce(time, max_elapsed_secs, mpi::op::max<double>());
    return max_elapsed_secs;
}
