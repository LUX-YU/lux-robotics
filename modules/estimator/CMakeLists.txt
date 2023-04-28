find_package(Eigen3 REQUIRED CONFIG)

add_library(
    estimator
    INTERFACE
)

target_include_directories(
    estimator
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_directories(
    estimator
    INTERFACE
    Eigen3::Eigen
)

