cmake_minimum_required(VERSION 3.10)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Pobranie GoogleTest za pomocą FetchContent
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)
# Pobierz i zbuduj GoogleTest
FetchContent_MakeAvailable(googletest)

# Add main project source directory to include path
include_directories(${CMAKE_SOURCE_DIR})

# Create a library from main_functions.c for use in both application and tests
add_library(main_functions STATIC ../main_functions.c)

# Create test executable
add_executable(runTests 
    test_main.cpp
)

# Add UNIT_TEST definition for runTests target
target_compile_definitions(runTests PRIVATE UNIT_TEST)

# Link only Google Test libraries and main_functions (not main.c)
target_link_libraries(
    runTests 
    GTest::gtest_main
    main_functions
)

include(GoogleTest)
gtest_discover_tests(runTests)

