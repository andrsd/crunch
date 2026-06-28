#pragma once

#include <chrono>

class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    // get number of elapsed seconds since "start"
    double
    seconds() const
    {
        std::chrono::duration<double> diff =
            std::chrono::high_resolution_clock::now() - this->start_;
        return diff.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};
