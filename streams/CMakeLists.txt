find_package(mpicpp-lite 2.2 REQUIRED)

# basic
add_executable(streams-basic streams-basic.cpp)
target_compile_features(streams-basic PUBLIC cxx_std_20)

# mpi version
if (BUILD_WITH_MPI)
    add_executable(streams-mpi streams-mpi.cpp)
    target_compile_features(streams-mpi PUBLIC cxx_std_20)
    target_link_libraries(streams-mpi PRIVATE mpicpp-lite::mpicpp-lite)

    add_custom_target(mpi
        COMMENT "Running STREAMS with MPI"
        COMMAND ${Python_INTERPRETER} ${CMAKE_SOURCE_DIR}/streams/streams-mpi.py
    )
endif()

# threads version
add_executable(streams-threads streams-threads.cpp)
target_compile_features(streams-threads PUBLIC cxx_std_20)
target_link_libraries(streams-threads PRIVATE Threads::Threads)
add_custom_target(threads
    COMMENT "Running STREAMS with threads"
    COMMAND ${Python_INTERPRETER} ${CMAKE_SOURCE_DIR}/streams/streams-threads.py
)
