#pragma once
#include <filesystem>
#include <lux/cxx/visibility_control.h>

namespace lux::robotics
{
    class SyncVideoRecorder
    {
    public:
        LUX_EXPORT SyncVideoRecorder( 
            std::filesystem::path record_path
        );

        LUX_EXPORT virtual bool recordLoop();

    private:
        std::filesystem::path           _record_path;
    };
} // namespace lux::robotic
