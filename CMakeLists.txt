cmake_minimum_required(VERSION 3.24)
project(csv_parser)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wextra-semi -O0 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


include_directories(src)

add_executable(main src/csv_parser.cpp src/main.cpp)
add_executable(csv_test src/csv_parser.cpp tests/test_csv_parser.cpp)

target_include_directories(csv_test PUBLIC ./yadro)

enable_testing()

target_link_libraries(
        csv_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(csv_test)
