project(lux-robotic)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(modules)

set(ENABLE_TEST ON CACHE BOOL "ENABLE TEST")

if(ENABLE_TEST)
add_subdirectory(test)
endif()
