#include <dataset/Rosbag.hpp>
#include <memory>
#include <iostream>

namespace lux::robotics::ros
{
    /* Rosbag implement */
	Rosbag::Rosbag(std::filesystem::path path)
	: _save_path(std::move(path)), _ifs(_save_path, std::ios::binary | std::ios::in)
	{
        rosbagFromFile();
        _record_start = _ifs.tellg();
	}

    Rosbag::~Rosbag()
    {
        if (_ifs.is_open()) {
            _ifs.close();
        }
    }

    bool Rosbag::is_open() const
    {
        return _ifs.is_open();
    }

    bool Rosbag::eof() const
    {
        return _ifs.eof();
    }

    RosbagVersion Rosbag::version() const
    {
        return _version;
    }

    void Rosbag::rosbagFromFile()
    {
        static const char* v1_1 = "#ROSRECORD V1.1"; // [16]
        static const char* v1_2 = "#ROSRECORD V1.2"; // [16]
        static const char* v2_0 = "#ROSBAG V2.0";    // [13]
        static constexpr char buffer_size = 17;
        char buffer[buffer_size];

        _ifs.getline(buffer, static_cast<std::streamsize>(buffer_size) - 1);

        if (!_ifs.good())
        {
            _ifs.close();
            return;
        }

        buffer[buffer_size - 1] = '\0';

        if (std::strcmp(buffer, v2_0) == 0)
        {
            _version = RosbagVersion::V2_0;
            return;
        }
        else if (std::strcmp(buffer, v1_2) == 0)
        {
            _version = RosbagVersion::V1_2;
            return;
        }
        else if (std::strcmp(buffer, v1_1) == 0)
        {
            _version = RosbagVersion::V1_1;
            return;
        }

        // if rosbag header was failed to read, close the file;
        _ifs.close();
    }

    Record Rosbag::nextRecord()
    {
        if (_ifs.eof() || _version == RosbagVersion::UNKNOWN)
        {
            return Record{};
        }
        RosbagRawRecord raw_record;
        readRawRecord(raw_record);

        return Record{raw_record};
    }

    bool Rosbag::nextRawRecord(RosbagRawRecord& raw_record)
    {
        return (_ifs.eof() || _version == RosbagVersion::UNKNOWN) 
            ? (readRawRecord(raw_record), true) : false;
    }

    void Rosbag::readRawRecord(RosbagRawRecord& raw_record)
    {
        _ifs.read(reinterpret_cast<char*>(&raw_record.header_len), 4);
        raw_record.header_data = new char[raw_record.header_len];
        _ifs.read(reinterpret_cast<char*>(raw_record.header_data), raw_record.header_len);

        _ifs.read(reinterpret_cast<char*>(&raw_record.value_len), 4);
        raw_record.value_data = new char[raw_record.value_len];
        _ifs.read(reinterpret_cast<char*>(raw_record.value_data), raw_record.value_len);
    }

    void Rosbag::init()
    {
        _ifs.seekg(_record_start);
    }
}
