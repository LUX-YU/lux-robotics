project(lux-robotic)
cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX d)

find_package(Boost				REQUIRED)
find_package(lux-cmake-toolset	REQUIRED)

generate_visibility_header(
    ENABLE_MACRO_NAME   LUX_ROBOTICS_LIBRARY
    PUBLIC_MACRO_NAME   LUX_ROBOTICS_PUBLIC
    GENERATE_FILE_PATH  lux/robotics/visibility.h
)

add_subdirectory(driver)
add_subdirectory(estimator)
add_subdirectory(odometer)

install_projects(
	PROJECT_NAME	lux-robotics
	VERSION			0.0.1
	NAMESPACE		lux::robotics
	COMPONENTS		driver
					estimator
					odometer
)

add_subdirectory(executable)
