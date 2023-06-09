project(lux-robotic)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX d)

find_package(Boost				REQUIRED)
find_package(lux-cmake-toolset	REQUIRED)
include(${LUX_CMAKE_COMPONENT_TOOLSET_DIR}/add_component.cmake)
include(${LUX_CMAKE_COMPONENT_TOOLSET_DIR}/install_project.cmake)

find_package(lux-cxx			REQUIRED COMPONENTS visibility)

# add_subdirectory(modules)
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

set(ENABLE_TEST OFF CACHE BOOL "ENABLE TEST")

if(ENABLE_TEST)
add_subdirectory(test)
endif()

add_subdirectory(executable)
