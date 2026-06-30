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
find_package(Threads REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter)

if (BUILD_WITH_BLAS)
    if (APPLE)
        set(BLA_VENDOR Apple)
    endif()
    find_package(BLAS REQUIRED)
endif()

if (BUILD_WITH_SYCL)
    find_package(SYCLCompiler REQUIRED)
endif()

add_executable(${PROJECT_NAME})

target_include_directories(crunch
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(crunch
    PRIVATE
        benchmark::benchmark
)

target_sources(crunch
    PRIVATE
        src/raw_flops.cpp
        src/cpu_axpy.cpp
        src/cpu_dot.cpp
        src/cpu_nrm2.cpp
        src/cpu_scale.cpp
        src/cpu_stream.cpp
        src/memory.cpp
        src/main.cpp
)

if (BUILD_WITH_BLAS)
    target_sources(crunch
        PRIVATE
            src/blas_axpy.cpp
            src/blas_dot.cpp
            src/blas_nrm2.cpp
            src/blas_scale.cpp
    )

    target_link_libraries(crunch
        PRIVATE
            BLAS::BLAS
    )
    if (APPLE)
        target_compile_definitions(crunch
            PRIVATE
                ACCELERATE_NEW_LAPACK
        )
    endif()
endif()

if (BUILD_WITH_SYCL)
    target_sources(crunch
        PRIVATE
            src/sycl_axpy.cpp
            src/sycl_dot.cpp
            src/sycl_nrm2.cpp
            src/sycl_scale.cpp
    )
endif()

# MPI
if (BUILD_WITH_MPI)
    add_subdirectory(mpi)
endif()

add_subdirectory(streams)
