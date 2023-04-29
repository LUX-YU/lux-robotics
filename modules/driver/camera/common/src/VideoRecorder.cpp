#include "driver/camera/VideoRecorder.hpp"

namespace lux::robotics
{
    SyncVideoRecorder::SyncVideoRecorder(std::filesystem::path record_path)
    : _record_path(std::move(record_path))
    {

    }

    bool SyncVideoRecorder::recordLoop()
    {
        return 0;
    }
}
