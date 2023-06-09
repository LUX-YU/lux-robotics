set(ENABLE_ZED_CAMERA OFF CACHE BOOL "ENABLE ZED CAMERA SUPPORT")
find_package(OpenCV 4 REQUIRED)

add_component(
	COMPONENT_NAME				driver
	NAMESPACE					lux
	EXPORT_INCLUDE_DIRS			include
	SOURCE_FILES				${CMAKE_CURRENT_SOURCE_DIR}/src/GPSBase.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSequence.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSequenceCapture.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/ReplayGPS.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/VideoRecorder.cpp
								$<$<BOOL:${ENABLE_ZED_CAMERA}>:${CMAKE_CURRENT_SOURCE_DIR}/ZEDCamera.cpp>
	PUBLIC_LIBRARIES			${OpenCV_LIBS}
								lux::cxx::visibility
	PROJECT_SHARED_INCLUDE_DIRS	${OpenCV_INCLUDE_DIRS}
)

target_compile_definitions(
    driver
	PRIVATE
	LUX_LIBRARY
)
