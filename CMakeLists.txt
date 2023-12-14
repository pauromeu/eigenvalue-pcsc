cmake_minimum_required(VERSION 3.10)
project(Eigenvalue-PCSC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recursively include all header files from the include directory
include_directories(include)

# Adding Eigen library
add_subdirectory(extern/eigen)

# Recursively find all source files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the main executable
add_executable(Eigenvalue-PCSC ${SOURCES})
target_include_directories(Eigenvalue-PCSC PRIVATE extern/eigen)

# Google Test setup
add_subdirectory(extern/googletest)

# Recursively find all test source files in the tests directory
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Create the test executable
add_executable(Eigenvalue-PCSC-tests ${TEST_SOURCES})
target_link_libraries(Eigenvalue-PCSC-tests gtest_main)

# Link Eigen library tests
target_include_directories(Eigenvalue-PCSC-tests PRIVATE extern/eigen)

enable_testing()


