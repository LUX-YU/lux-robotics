#pragma once
#include <filesystem>
#include <lux/robotics/visibility.h>

namespace lux::robotics
{
    class SyncVideoRecorder
    {
    public:
        LUX_ROBOTICS_PUBLIC SyncVideoRecorder(
            std::filesystem::path record_path
        );

        LUX_ROBOTICS_PUBLIC virtual bool recordLoop();

    private:
        std::filesystem::path           _record_path;
    };
} // namespace lux::robotic
