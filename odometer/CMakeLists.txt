find_package(OpenCV 4 CONFIG)

add_component(
    COMPONENT_NAME					odometer
    NAMESPACE						lux::robotics
    SOURCE_FILES					${CMAKE_CURRENT_SOURCE_DIR}/src/CVFeatureMatcher.cpp
									${CMAKE_CURRENT_SOURCE_DIR}/src/EpipolarGeometry.cpp
    EXPORT_INCLUDE_DIRS				include
	PROJECT_SHARED_INCLUDE_DIRS		${OpenCV_INCLUDE_DIRS}
    PUBLIC_LIBRARIES				lux::cxx::visibility
									opencv_core
									opencv_features2d
									opencv_calib3d
									opencv_highgui
	PRIVATE_DEFINITIONS				LUX_LIBRARY
	TRANSITIVE_PACKAGES_COMMANDS	"find_package(OpenCV 4 CONFIG)"
)
