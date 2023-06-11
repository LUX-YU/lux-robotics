find_package(OpenCV 4 CONFIG)

add_component(
    COMPONENT_NAME                  odometer
    NAMESPACE                       lux::robotics
    SOURCE_FILES                    ${CMAKE_CURRENT_SOURCE_DIR}/src/CVFeatureMatcher.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/EpipolarGeometry.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/FeatureDetector.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/FeatureMatcher.cpp
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
