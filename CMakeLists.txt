cmake_minimum_required(VERSION 3.10)
project(TapeProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(/opt/homebrew/opt/nlohmann-json/include)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(TapeProject
    main.cpp
    TapeSorter.cpp
    TapeDevice.cpp
    TapeUtils.cpp
)

add_executable(TapeTests
    TapeTests.cpp
    TapeSorter.cpp
    TapeDevice.cpp
    TapeUtils.cpp
)

target_link_libraries(TapeProject ${GTEST_LIBRARIES} pthread)
target_link_libraries(TapeTests ${GTEST_LIBRARIES} pthread)