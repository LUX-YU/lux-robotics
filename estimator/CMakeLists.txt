find_package(Eigen3 REQUIRED CONFIG)

add_interface_component(
	COMPONENT_NAME				estimator
	NAMESPACE					lux::robotics
	EXPORT_INCLUDE_DIRS			include
	PUBLIC_LIBRARIES			Eigen3::Eigen
)
