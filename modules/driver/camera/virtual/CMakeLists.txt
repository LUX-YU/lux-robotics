add_library(
        camera_virtual
        SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSequence.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSequenceCapture.cpp
)

target_link_libraries(
        camera_virtual
        PUBLIC
        camera_common
)

target_include_directories(
        camera_virtual
        PUBLIC
        include
)