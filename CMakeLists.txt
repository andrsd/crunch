cmake_minimum_required(VERSION 3.16)

project(crunch
    VERSION 1.0.0
    LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

option(BUILD_WITH_BLAS "Build with BLAS")
option(BUILD_WITH_SYCL "Build with SYCL")
option(BUILD_WITH_MPI "Build with MPI")

find_package(benchmark REQUIRED)

if (BUILD_WITH_BLAS)
    if (APPLE)
        set(BLA_VENDOR Apple)
    endif()
    find_package(BLAS REQUIRED)
endif()

if (BUILD_WITH_SYCL)
    find_package(SYCLCompiler REQUIRED)
endif()

set(SRCS
    src/cpu_axpy.cpp
    src/cpu_dot.cpp
    src/cpu_nrm2.cpp
    src/cpu_scale.cpp
    src/memory.cpp
    src/main.cpp
)

if (BUILD_WITH_BLAS)
    list(APPEND SRCS
        src/blas_axpy.cpp
        src/blas_dot.cpp
        src/blas_nrm2.cpp
        src/blas_scale.cpp
    )
endif()

if (BUILD_WITH_SYCL)
    list(APPEND SRCS
        src/sycl_axpy.cpp
        src/sycl_dot.cpp
        src/sycl_nrm2.cpp
        src/sycl_scale.cpp
    )
endif()

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark)
if (BUILD_WITH_BLAS)
    target_link_libraries(${PROJECT_NAME} PRIVATE BLAS::BLAS)
endif()

# MPI

if (BUILD_WITH_MPI)
    find_package(mpicpp-lite 2.2 REQUIRED)

    set(MPI_SRCS
        mpi/main.cpp
        mpi/all_reduce.cpp
        mpi/reduce.cpp
    )

    add_executable(mpi-${PROJECT_NAME} ${MPI_SRCS})

    target_link_libraries(mpi-${PROJECT_NAME}
        PRIVATE
            benchmark::benchmark
            mpicpp-lite::mpicpp-lite
    )
endif()
