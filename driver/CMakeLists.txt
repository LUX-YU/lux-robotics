set(ENABLE_ZED_CAMERA OFF CACHE BOOL "ENABLE ZED CAMERA SUPPORT")
find_package(OpenCV 4 REQUIRED)

add_component(
    COMPONENT_NAME                  driver
    NAMESPACE                       lux::robotics
    SOURCE_FILES                    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPSBase.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSequence.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSequenceCapture.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/ReplayGPS.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/VideoRecorder.cpp
									$<$<BOOL:${ENABLE_ZED_CAMERA}>:${CMAKE_CURRENT_SOURCE_DIR}/ZEDCamera.cpp>
    BUILD_TIME_EXPORT_INCLUDE_DIRS  ${CMAKE_CURRENT_SOURCE_DIR}/include
									${LUX_GENERATE_HEADER_DIR}
    INSTALL_TIME_INCLUDE_PREFIX     include
    PUBLIC_LIBRARIES                opencv_core
									opencv_features2d
									opencv_calib3d
									opencv_highgui
    PRIVATE_DEFINITIONS             LUX_ROBOTICS_LIBRARY
    TRANSITIVE_PACKAGES_COMMANDS	"find_package(OpenCV 4 CONFIG)"
)
