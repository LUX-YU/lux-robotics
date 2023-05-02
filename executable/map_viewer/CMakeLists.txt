project(MapViewer LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(IOS)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum iOS deployment version" FORCE)
elseif(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS deployment version" FORCE)
endif()

find_package(Qt6 COMPONENTS REQUIRED Core Widgets Multimedia Positioning Sensors OpenGLWidgets WebSockets Qml Quick)
find_package(ArcGISRuntime 200.1.0 COMPONENTS REQUIRED Cpp)

set(SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/MapViewer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MapViewer.h
)

qt_add_executable(MapViewer ${SOURCE_FILES})

if(DEFINED ArcGISRuntime_LIBRARIES)
	add_custom_command(TARGET MapViewer POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${ArcGISRuntime_LIBRARIES}
		$<TARGET_FILE_DIR:MapViewer>
	)
endif()

target_include_directories(
	MapViewer 
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	MapViewer 
	PRIVATE
	ArcGISRuntime::Cpp
	Qt6::Core
	Qt6::Widgets
	Qt6::Multimedia
	Qt6::Positioning
	Qt6::Sensors
	Qt6::OpenGLWidgets
	Qt6::WebSockets
	Qt6::Qml
	Qt6::Quick
)
