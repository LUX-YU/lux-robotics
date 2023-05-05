#include "dataset/Record.hpp"
#include <bit>

namespace lux::robotics::ros
{
    /* Record Wrapper */
    Record::Record(const Record& other)
    {
        _raw.header_len     = other._raw.header_len;
        _raw.value_len      = other._raw.value_len;
        _raw.header_data    = new char[_raw.header_len];
        _raw.value_data     = new char[_raw.value_len];
        _type               = other._type;
        std::memcpy(_raw.header_data, other._raw.header_data, _raw.header_len);
        std::memcpy(_raw.value_data, other._raw.value_data, _raw.value_len);
    }

    Record& Record::operator=(const Record& other)
    {
        release();
        _raw.header_len     = other._raw.header_len;
        _raw.value_len      = other._raw.value_len;
        _raw.header_data    = new char[_raw.header_len];
        _raw.value_data     = new char[_raw.value_len];
        _type               = other._type;
        _header_view        = other._header_view;
        std::memcpy(_raw.header_data, other._raw.header_data, _raw.header_len);
        std::memcpy(_raw.value_data, other._raw.value_data, _raw.value_len);
        return *this;
    }

    Record::Record(Record&& other) noexcept
    {
        _raw            = other._raw;
        _type           = other._type;
        _header_view    = other._header_view;
        other._raw.header_len = 0;
        other._raw.value_len = 0;
    }

    Record& Record::operator=(Record&& other) noexcept
    {
        release();
        _raw            = other._raw;
        _type           = other._type;
        _header_view    = other._header_view;
        other._raw.header_len = 0;
        other._raw.value_len = 0;
        return *this;
    }

    Record::Record()
    {
        _raw.header_data    = nullptr;
        _raw.value_data     = nullptr;
        _raw.header_len     = 0;
        _raw.value_len      = 0;
        _type               = RecordEnum::UNKNOWN;
    }

    void Record::release()
    {
        if (_raw.header_len > 0)
        {
            delete[] _raw.header_data;
        }
        if (_raw.value_len > 0)
        {
            delete[] _raw.value_data;
        }
    }

    Record::Record(const RosbagRawRecord& raw)
    {
        _raw = raw;
        _type = parse_header(_header_view);
    }

    Record::~Record()
    {
        if (_raw.header_len > 0)
            delete[] _raw.header_data;
        if (_raw.value_len > 0)
            delete[] _raw.value_data;
    }

    bool searchRecordHeader(const RosbagRawRecord& record, SearchCallback callback, void* user_data)
    {
        uint32_t cur = 0;
        auto header_data = record.header_data;
        while (cur < record.header_len)
        {
            auto field_start = &header_data[cur];
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
                data_len,
                user_data
            );

            cur = cur + 4 + field_len;
        }
        return true;
    }

    struct SearchPackedParam
    {
        size_t          name_len;
        const char*     name;
        size_t          data_len;
        const uint8_t*  data;
    };

    struct ParseResult
    {
        SearchPackedParam buffer[5];
        RecordEnum        type;
        char              count;
    };

    static void parse_method(const char* name, size_t name_len, const uint8_t* data, size_t data_len, void* user_data)
    {
        ParseResult* result = static_cast<ParseResult*>(user_data);
        if (std::strncmp(name, "op", 2) == 0)
        {
            // op : one byte
            result->type = static_cast<RecordEnum>(*data);
            return;
        }
        auto& buffer = result->buffer[result->count];
        buffer.name     = name;
        buffer.name_len = name_len;
        buffer.data     = data;
        buffer.data_len = data_len;

        result->count++;
    }

    static void __parse_header_buffer(ParseResult& result, HeaderVariants& var)
    {
        switch (result.type)
        {
        case RecordEnum::MESSAGE_DATA:
        {
            var = MessageData{};
            auto& view = std::get<MessageData>(var);
            for(char i = 0 ; i < result.count; i++)
            {
                auto& buffer = result.buffer[i];
                if (std::strncmp("conn", buffer.name, buffer.name_len) == 0)
                {
                    view.conn = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
                else if (std::strncmp("time", buffer.name, buffer.name_len) == 0)
                {
                    view.time = *reinterpret_cast<const uint64_t*>(buffer.data);
                }
            }
            break;
        }
        case RecordEnum::BAG_HEADER:
        {
            var = BagHeader{};
            auto& view = std::get<BagHeader>(var);
            for (char i = 0; i < result.count; i++)
            {
                auto& buffer = result.buffer[i];
                if (std::strncmp("index_pos", buffer.name, buffer.name_len) == 0)
                {
                    view.index_pos = *reinterpret_cast<const uint64_t*>(buffer.data);
                }
                else if (std::strncmp("conn_count", buffer.name, buffer.name_len) == 0)
                {
                    view.conn_count = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
                else if (std::strncmp("chunk_count", buffer.name, buffer.name_len) == 0)
                {
                    view.chunk_count = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
            }
            break;
        }
        case RecordEnum::INDEX_DATA:
        {
            var = IndexData{};
            auto& view = std::get<IndexData>(var);
            for (char i = 0; i < result.count; i++)
            {
                auto& buffer = result.buffer[i];
                if (std::strncmp("ver", buffer.name, buffer.name_len) == 0)
                {
                    view.ver = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
                else if (std::strncmp("conn", buffer.name, buffer.name_len) == 0)
                {
                    view.conn = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
                else if (std::strncmp("count", buffer.name, buffer.name_len) == 0)
                {
                    view.count = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
            }
            break;
        }
        case RecordEnum::CHUNK:
        {
            var = Chunk{};
            auto& view = std::get<Chunk>(var);
            for (char i = 0; i < result.count; i++)
            {
                auto& buffer = result.buffer[i];
                if (std::strncmp("compression", buffer.name, buffer.name_len) == 0)
                {
                    view.compression = std::string_view((const char*)buffer.data, buffer.data_len);
                }
                else if (std::strncmp("size", buffer.name, buffer.name_len) == 0)
                {
                    view.size = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
            }
            break;
        }
        case RecordEnum::CHUNK_INFO:
        {
            var = ChunkInfo{};
            auto& view = std::get<ChunkInfo>(var);
            for (char i = 0; i < result.count; i++)
            {
                auto& buffer = result.buffer[i];
                if (std::strncmp("ver", buffer.name, buffer.name_len) == 0)
                {
                    view.ver = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
                else if (std::strncmp("chunk_pos", buffer.name, buffer.name_len) == 0)
                {
                    view.chunk_pos = *reinterpret_cast<const uint64_t*>(buffer.data);
                }
                else if (std::strncmp("start_time", buffer.name, buffer.name_len) == 0)
                {
                    view.start_time = *reinterpret_cast<const uint64_t*>(buffer.data);
                }
                else if (std::strncmp("end_time", buffer.name, buffer.name_len) == 0)
                {
                    view.end_time = *reinterpret_cast<const uint64_t*>(buffer.data);
                }
                else if (std::strncmp("count", buffer.name, buffer.name_len) == 0)
                {
                    view.count = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
            }
            break;
        }
        case RecordEnum::CONNECTION:
        {
            var = Connection{};
            auto& view = std::get<Connection>(var);
            for (char i = 0; i < result.count; i++)
            {
                auto& buffer = result.buffer[i];
                if (std::strncmp("conn", buffer.name, result.buffer->name_len) == 0)
                {
                    view.conn = *reinterpret_cast<const uint32_t*>(buffer.data);
                }
                else if (std::strncmp("topic", buffer.name, buffer.name_len) == 0)
                {
                    view.topic = std::string_view((const char*)buffer.data, buffer.data_len);
                }
            }
            break;
        }
        }
    }

    RecordEnum Record::parse_header(HeaderVariants& var)
    {
        ParseResult result; // max field is `Chunk info`
        result.count = 0;
        if (!this->searchHeader(parse_method, &result))
        {
            return RecordEnum::UNKNOWN;
        }

        __parse_header_buffer(result, var);
        return result.type;
    }

    bool Record::searchHeader(SearchCallback callback, void* user_data)
    {
        return searchRecordHeader(_raw, callback, user_data);
    }

    RecordEnum Record::type() const
    {
        return _type;
    }

    const char* Record::data() const
    {
        return _raw.value_data;
    }

    size_t Record::data_len() const
    {
        return _raw.value_len;
    }
}