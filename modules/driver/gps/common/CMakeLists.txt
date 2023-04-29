add_library(
	gps_common
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/GPSBase.cpp
)

target_include_directories(
	gps_common
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	gps_common
	PUBLIC
	system
)