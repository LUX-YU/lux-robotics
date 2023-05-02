add_library(
	dataset_kitti
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/KITTI.cpp
)

target_include_directories(
	dataset_kitti
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	dataset_kitti
	PUBLIC
	system
)
