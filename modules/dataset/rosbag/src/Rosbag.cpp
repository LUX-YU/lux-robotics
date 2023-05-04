#include <dataset/Rosbag.hpp>
#include <memory>

namespace lux::robotics::ros
{
    /* Record Wrapper */
    Record::Record(const Record& other)
    {
        _raw.header_len = other._raw.header_len;
        _raw.value_len = other._raw.value_len;
        _raw.header_data = new char[_raw.header_len];
        _raw.value_data = new char[_raw.value_len];
        std::memcpy(_raw.header_data, other._raw.header_data, _raw.header_len);
        std::memcpy(_raw.value_data, other._raw.value_data, _raw.value_len);
    }

    Record& Record::operator=(const Record& other)
    {
        release();
        _raw.header_len = other._raw.header_len;
        _raw.value_len = other._raw.value_len;
        _raw.header_data = new char[_raw.header_len];
        _raw.value_data = new char[_raw.value_len];
        std::memcpy(_raw.header_data, other._raw.header_data, _raw.header_len);
        std::memcpy(_raw.value_data, other._raw.value_data, _raw.value_len);
        return *this;
    }

    Record::Record(Record&& other) noexcept
    {
        _raw = other._raw;
        other._raw.header_len = 0;
        other._raw.value_len = 0;
    }

    Record& Record::operator=(Record&& other) noexcept
    {
        release();
        _raw = other._raw;
        other._raw.header_len = 0;
        other._raw.value_len = 0;
        return *this;
    }

    Record::Record()
    {
        _raw.header_data = nullptr;
        _raw.value_data = nullptr;
        _raw.header_len = 0;
        _raw.value_len = 0;
    }

    void Record::release()
    {
        if (_raw.header_len > 0)
        {
            delete[] _raw.header_data;
            _raw.header_data = nullptr;
        }
        if (_raw.value_len > 0)
        {
            delete[] _raw.value_data;
            _raw.value_data = nullptr;
        }
    }

    Record::Record(const RosbagRawRecord& raw)
    {
        _raw = raw;
    }

    Record::~Record()
    {
        if (_raw.header_data)
            delete[] _raw.header_data;
        if (_raw.value_data)
            delete[] _raw.value_data;
    }

    bool searchRecordHeader(const RosbagRawRecord& record, SearchCallback callback)
    {
        uint32_t cur = 0;
        auto header_data = record.header_data;
        while (cur < record.header_len)
        {
            auto field_start = header_data[cur];
            auto field_len = *reinterpret_cast<uint32_t*>(field_start);
            char* name_start = &header_data[cur + 4];

            uint32_t i = 0;
            for (i = 0; i < field_len; i++)
            {
                if (name_start[i] == '=') break;
            }
            if (i == field_len)
            {
                return false;
            }
            uint32_t name_len = i;
            uint32_t data_len = field_len - name_len - 1;
            const char* value_start = (&name_start[name_len] + 1);
            callback(
                name_start,
                name_len,
                reinterpret_cast<const uint8_t*>(value_start),
                data_len
            );

            cur = cur + 4 + field_len;
        }
        return true;
    }

    bool Record::searchHeader(SearchCallback callback)
    {
        return searchRecordHeader(_raw, callback);
    }

    /* Rosbag implement */
	Rosbag::Rosbag(std::filesystem::path path)
	: _save_path(std::move(path)), _ifs(_save_path, std::ios::binary)
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

    static void raw_record_deleter(RosbagRawRecord* data)
    {
        if (data == nullptr) return;
        if (data->header_data) delete[] data->header_data;
        if (data->value_data) delete[] data->value_data;
        delete data;
    }

    Record Rosbag::nextRecord()
    {
        if (!is_open() || _ifs.eof() || _version == RosbagVersion::UNKNOWN)
        {
            return Record{};
        }
        RosbagRawRecord raw_record;
        readRawRecord(raw_record);

        return Record{raw_record};
    }

    bool Rosbag::nextRawRecord(RosbagRawRecord& raw_record)
    {
        return (is_open() || _ifs.eof() || _version == RosbagVersion::UNKNOWN) 
            ? (readRawRecord(raw_record), true) : false;
    }

    void Rosbag::readRawRecord(RosbagRawRecord& raw_record)
    {
        _ifs.read(reinterpret_cast<char*>(&raw_record.header_len), 4);
        raw_record.header_data = new char[raw_record.header_len];
        _ifs.read(reinterpret_cast<char*>(raw_record.header_data), raw_record.header_len);

        _ifs.read(reinterpret_cast<char*>(&raw_record.value_len), 4);
        raw_record.value_data = new char[raw_record.value_len];
        _ifs.read(reinterpret_cast<char*>(raw_record.value_data), raw_record.header_len);
    }

    void Rosbag::init()
    {
        _ifs.seekg(_record_start);
    }
}
