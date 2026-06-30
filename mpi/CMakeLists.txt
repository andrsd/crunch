find_package(mpicpp-lite 2.2 REQUIRED)

add_executable(mpi-crunch)

target_sources(mpi-crunch
    PRIVATE
        main.cpp
        all_reduce.cpp
        broadcast.cpp
        reduce.cpp
        send_recv.cpp
        mpi_cpu_dot.cpp
        mpi_cpu_nrm2.cpp
        mpi_cpu_scale.cpp
)

target_include_directories(mpi-crunch
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(mpi-crunch
    PRIVATE
        benchmark::benchmark
        mpicpp-lite::mpicpp-lite
)
