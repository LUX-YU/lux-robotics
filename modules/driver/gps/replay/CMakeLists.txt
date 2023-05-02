add_library(
	gps_replay
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/ReplayGPS.cpp
)

target_include_directories(
	gps_replay
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	gps_replay
	PUBLIC
	system
	gps_common
)
