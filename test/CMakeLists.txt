find_package(matplot++ CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

## Kalman filter test
add_executable(
    kalman_filter_test
    kalman_filter_test.cpp
)

target_link_libraries(
    kalman_filter_test
    PUBLIC
    estimator
    ZLIB::ZLIB
    Matplot++::cimg
    Matplot++::matplot
    Matplot++::nodesoup
)

if(TARGET camera_zed)
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
    zed_camera_viewer
    zed_camera_viewer.cpp
)

target_link_libraries(
    zed_camera_viewer
    PRIVATE
    camera_zed
)
endif()

add_executable(
    virtual_camera_test
    virtual_camera_test.cpp
)

target_link_libraries(
    virtual_camera_test
    PUBLIC
    camera_virtual
    dataset_support_kitti
)