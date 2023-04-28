find_package(matplot++ CONFIG REQUIRED)

## Kalman filter test
add_executable(
    kalman_filter_test
    kalman_filter_test.cpp
)

target_link_libraries(
    kalman_filter_test
    PUBLIC
    estimator
    Matplot++::cimg
    Matplot++::matplot
    Matplot++::nodesoup
)

## ZED Camera Test
add_executable(
    zed_camera_test
    zed_camera_record.cpp
)

target_link_libraries(
    zed_camera_test
    PRIVATE
    camera_zed
)

## ZED Camera Viewer
add_executable(
    camera_viewer
    camera_viewer.cpp
)

target_link_libraries(
    camera_viewer
    PRIVATE
    camera_zed
)
