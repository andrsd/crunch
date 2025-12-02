# Find SYCL compiler

if (NOT DEFINED HAS_SYCL_SUPPORT)
    set(SYCL_COMPILER_FOUND FALSE)
    set(SYCL_COMPILER_VERSION "unknown")

    # Try AdaptiveCpp
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} --acpp-version
        OUTPUT_VARIABLE OUTPUT
    )
    if (OUTPUT MATCHES "AdaptiveCpp version")
        find_package(AdaptiveCpp REQUIRED)
        set(SYCL_COMPILER_FOUND TRUE)
        # get version
        string(REGEX MATCH "([0-9]+\\.[0-9]+\\.[0-9]+)" SYCL_COMPILER_VERSION "${OUTPUT}")

        message(STATUS "Found SYCL compiler: AdaptiveCpp ${ACPP_VERSION}")
    endif()

    if (SYCL_COMPILER_FOUND)
        # try to compile a SYCL program
        set(SYCL_TEST_CODE "
            #include <CL/sycl.hpp>
            int main() {
                cl::sycl::queue queue;
                return 0;
            }
        ")

        file(WRITE "${CMAKE_BINARY_DIR}/sycl_test.cpp" "${SYCL_TEST_CODE}")

        # Attempt to compile the test program
        try_compile(SYCL_SUPPORT_DETECTED
            ${CMAKE_BINARY_DIR}
            ${CMAKE_BINARY_DIR}/sycl_test.cpp
        )

        set(HAS_SYCL_SUPPORT ${SYCL_SUPPORT_DETECTED} CACHE BOOL "Does the compiler support SYCL?")
        if (NOT HAS_SYCL_SUPPORT)
            message(FATAL_ERROR "SYCL compiler found, but unable to compile a SYCL program")
        endif()
    else()
        message(FATAL_ERROR "Need a SYCL-compatible compiler to build akceli")
    endif()
endif()
