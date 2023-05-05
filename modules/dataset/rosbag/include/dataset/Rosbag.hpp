#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include <fstream>
#include <functional>
#include <visibility_control.h>

#include "Record.hpp"

namespace lux::robotics::ros
{
    enum class RosbagVersion
    {
        V1_1,
        V1_2,
        V2_0,
        UNKNOWN
    };

    class Rosbag
    {
    public:
        LUX_EXPORT explicit Rosbag(std::filesystem::path path);

        LUX_EXPORT ~Rosbag();

        [[nodiscard]] LUX_EXPORT bool is_open() const;

        [[nodiscard]] LUX_EXPORT bool eof() const;

        [[nodiscard]] LUX_EXPORT RosbagVersion version() const;

        LUX_EXPORT void init();

        LUX_EXPORT Record nextRecord();

        [[nodiscard]] LUX_EXPORT bool nextRawRecord(RosbagRawRecord& raw_record);

    private:

        void rosbagFromFile();

        void readRawRecord(RosbagRawRecord&);

        std::filesystem::path   _save_path;
        size_t                  _record_start;
        RosbagVersion           _version{ RosbagVersion ::UNKNOWN};
        std::ifstream           _ifs;
    };
}