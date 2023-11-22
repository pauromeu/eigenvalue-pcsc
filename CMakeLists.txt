cmake_minimum_required(VERSION 3.10)

project(Eigenvalue-PCSC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(
    Eigenvalue-PCSC 
    src/main.cpp 
    src/greetings.cpp
)

add_subdirectory(extern/googletest)

enable_testing()
add_executable(
  Eigenvalue-PCSC-tests
  src/greetings.cpp
  tests/greetings_test.cpp
)
target_link_libraries(
  Eigenvalue-PCSC-tests
  gtest_main
)

