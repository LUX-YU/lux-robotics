#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include <fstream>
#include <visibility_control.h>

namespace lux::robotics::ros
{
    enum class RosbagVersion
    {
        V1_1,
        V1_2,
        V2_0,
        UNKNOWN
    };

    struct RosbagRawRecord
    {
        uint32_t    header_len;
        char*       header_data;
        uint32_t    value_len;
        char*       value_data;
    };

    // name is not null terminator
    using SearchCallback = void (*)(const char* name, size_t name_len, const uint8_t* data, size_t data_len);

    LUX_EXPORT bool searchRecordHeader(const RosbagRawRecord&, SearchCallback callback);

    // RAII Wrapper
    class Record
    {
        friend class Rosbag;
    public:
        LUX_EXPORT Record();
        LUX_EXPORT Record(const RosbagRawRecord& raw);

        LUX_EXPORT Record(const Record& other);
        LUX_EXPORT Record& operator=(const Record& other);

        LUX_EXPORT Record(Record&& other) noexcept;
        LUX_EXPORT Record& operator=(Record&& other) noexcept;

        LUX_EXPORT ~Record();

        LUX_EXPORT bool searchHeader(SearchCallback callback);

    private:
        void release();

        RosbagRawRecord _raw;
    };

    class Rosbag
    {
    public:
        LUX_EXPORT explicit Rosbag(std::filesystem::path path);

        LUX_EXPORT ~Rosbag();

        [[nodiscard]] LUX_EXPORT bool is_open() const;

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