add_library(
	dataset_rosbag
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/Rosbag.cpp
)

target_include_directories(
	dataset_rosbag
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	dataset_rosbag
	PUBLIC
	system
)
