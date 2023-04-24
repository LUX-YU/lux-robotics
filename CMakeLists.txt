project(lux-robotic)
cmake_minimum_required(VERSION 3.10)

find_package(Eigen3    CONFIG REQUIRED)
find_package(matplot++ CONFIG REQUIRED)

add_executable(
    test_main
    src/main.cpp
)

target_include_directories(
        test_main
        PRIVATE
        include
)

target_link_libraries(
        test_main
        PRIVATE
        Eigen3::Eigen
        Matplot++::cimg
        Matplot++::matplot
        Matplot++::nodesoup
)
