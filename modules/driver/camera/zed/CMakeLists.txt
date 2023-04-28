find_package(ZED 3 REQUIRED)
find_package(CUDA ${ZED_CUDA_VERSION} REQUIRED)

set(ZED_LINK_SHARED_ZED ON CACHE BOOL "Link with the ZED SDK shared executable")

if (NOT ZED_LINK_SHARED_ZED AND MSVC)
    message(FATAL_ERROR "ZED_LINK_SHARED_ZED OFF : ZED SDK static libraries not available on Windows")
endif()

if (ZED_LINK_SHARED_ZED)
    SET(ZED_LIBS ${ZED_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY})
else()
    SET(ZED_LIBS ${ZED_STATIC_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_LIBRARY})
endif()

message("Find Cuda Include ${CUDA_INCLUDE_DIRS}")
message("Find ZED Include ${ZED_INCLUDE_DIRS}")
message("ZED Libraries ${ZED_LIBS}")

set(CAMERA_ZED_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ZEDCamera.cpp
)

add_library(
    camera_zed
    SHARED
    ${CAMERA_ZED_SOURCE}
)

target_include_directories(
    camera_zed
    PUBLIC
    ${CUDA_INCLUDE_DIRS}
    ${ZED_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    camera_zed
    PUBLIC
    system
    camera_common
    ${ZED_LIBS}
)
