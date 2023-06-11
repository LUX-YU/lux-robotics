find_package(Eigen3 REQUIRED CONFIG)

add_interface_component(
	COMPONENT_NAME					estimator
	NAMESPACE						lux::robotics
	BUILD_TIME_EXPORT_INCLUDE_DIRS  ${CMAKE_CURRENT_SOURCE_DIR}/include
    INSTALL_TIME_INCLUDE_PREFIX     include
	PUBLIC_LIBRARIES				Eigen3::Eigen
    TRANSITIVE_PACKAGES_COMMANDS    "find_package(Eigen3 REQUIRED CONFIG)"
)
