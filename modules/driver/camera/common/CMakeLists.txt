find_package(OpenCV 4 CONFIG REQUIRED)

set(
    CAMERA_COMMON
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VideoRecorder.cpp    
)

add_library(
    camera_common
    SHARED
    ${CAMERA_COMMON}
)

target_include_directories(
    camera_common
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    camera_common
    PUBLIC
    system
    ${OpenCV_LIBS}
)
