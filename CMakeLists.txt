cmake_minimum_required(VERSION 4.3)
project(marketSimulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

add_library(marketSim_core
  src/analytics.cpp
  src/backtest.cpp
  src/datafeed.cpp
  src/state.cpp
  src/strategy.cpp
)

target_include_directories(marketSim_core PUBLIC include)

add_executable(marketSim
  src/main.cpp
)

target_link_libraries(marketSim PRIVATE marketSim_core)

enable_testing()

add_executable(backtesting_test
  tests/backtesting_test.cc
)

target_link_libraries(backtesting_test PRIVATE_HEADER
  GTest::gtest_main
  marketSim_core
)

include (GoogleTest)
gtest_discover_tests(backtesting_test)

# setting up google benchmark

# configures google benchmark to skip its own tests and docs
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.5
)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(marketSim_bench benchmarks/main.cpp)
target_link_libraries(marketSim_bench PRIVATE
  benchmark::benchmark
  benchmark::benchmark_main
  marketSim_core
)

