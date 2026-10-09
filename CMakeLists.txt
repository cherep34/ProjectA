cmake_minimum_required(VERSION 3.20)
project(KVS_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wshadow)

# --- Библиотека ядра ---
add_library(kvs_core STATIC
    src/store.cpp
    src/owning_buffer.cpp
    src/parser.cpp
)

target_include_directories(kvs_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

# --- Консольный клиент ---
add_executable(kvs_cli src/main.cpp)
target_link_libraries(kvs_cli PRIVATE kvs_core)

# --- GoogleTest ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(kvs_tests
    tests/test_owning_buffer.cpp
    tests/test_parser.cpp
    tests/test_parser_expected.cpp
    tests/test_append_only_log.cpp
    tests/test_file_deleter.cpp
    tests/test_command_cache.cpp
)

target_link_libraries(kvs_tests PRIVATE kvs_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(kvs_tests)