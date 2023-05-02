project(lux-robotic)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_subdirectory(modules)

set(ENABLE_TEST ON CACHE BOOL "ENABLE TEST")

if(ENABLE_TEST)
add_subdirectory(test)
endif()

add_subdirectory(executable)
