set(ENABLE_DRIVER_CAMERA_ZED     OFF CACHE BOOL "Enable driver camera zed")
set(ENABLE_DRIVER_CAMERA_VIRTUAL ON CACHE BOOL  "Enable virtual camera")

add_subdirectory(common)
if(ENABLE_DRIVER_CAMERA_ZED)
    add_subdirectory(zed)
endif()
add_subdirectory(virtual)