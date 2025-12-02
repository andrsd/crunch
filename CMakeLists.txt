cmake_minimum_required(VERSION 3.16)

project(crunch
    VERSION 1.0.0
)

option(BUILD_WITH_BLAS "Build with BLAS")

find_package(benchmark REQUIRED)

if (BUILD_WITH_BLAS)
    if (APPLE)
        set(BLA_VENDOR Apple)
    endif()
    find_package(BLAS REQUIRED)
endif()

set(SRCS
    cpu_axpy.cpp
    cpu_dot.cpp
    cpu_nrm2.cpp
    cpu_scale.cpp
    memory.cpp
    main.cpp
)

if (BUILD_WITH_BLAS)
    list(APPEND SRCS
        blas_axpy.cpp
        blas_dot.cpp
        blas_nrm2.cpp
        blas_scale.cpp
    )
endif()

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark)
if (BUILD_WITH_BLAS)
    target_link_libraries(${PROJECT_NAME} PRIVATE BLAS::BLAS)
endif()
