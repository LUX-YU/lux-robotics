add_library(
	gps_fake
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/FakeGPS.cpp
)

target_include_directories(
	gps_fake
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	gps_fake
	PUBLIC
	system
)