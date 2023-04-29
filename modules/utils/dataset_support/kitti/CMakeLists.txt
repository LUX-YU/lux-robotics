add_library(
	dataset_support_kitti
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/KITTI.cpp
)

target_include_directories(
	dataset_support_kitti
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	dataset_support_kitti
	PUBLIC
	gps_fake
)
