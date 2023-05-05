#pragma once
#include <cstdint>
#include <functional>
#include <string_view>
#include <variant>
#include <visibility_control.h>

namespace lux::robotics::ros
{
    struct RosbagRawRecord
    {
        uint32_t    header_len;
        char*       header_data;
        uint32_t    value_len;
        char*       value_data;
    };

    // name is not null terminator
    using SearchCallback = void (*)(const char* name, size_t name_len, const uint8_t* data, size_t data_len, void* user_data);

    LUX_EXPORT bool searchRecordHeader(const RosbagRawRecord&, SearchCallback callback, void* user_data);

    // record header format
    struct BagHeader
    {
        uint64_t index_pos;     // offset of first record after the chunk section
        uint32_t conn_count;    // number of unique connections in the file
        uint32_t chunk_count;   // number of chunk records in the file
    };

    // record header format
    struct Chunk
    {
        std::string_view compression;   // compression type for the data
        uint32_t         size;          // size in bytes of the uncompressed chunk
    };

    // record header format
    struct Connection
    {
        uint32_t conn; // unique connection ID
        std::string_view topic; // topic on which the messages are stored
    };

    // record header format
    struct MessageData
    {
        uint32_t conn;  // ID for connection on which message arrived
        uint64_t time;  // time at which the message was received
    };

    // record header format
    struct IndexData
    {
        uint32_t ver;   // index data record version
        uint32_t conn;  // connection ID
        uint32_t count; // number of messages on conn in the preceding chunk
    };

    // record header format
    struct ChunkInfo
    {
        uint32_t ver;           // chunk info record version
        uint64_t chunk_pos;     // offset of the chunk record
        uint64_t start_time;    // timestamp of earliest message in the chunk
        uint64_t end_time;      // timestamp of latest message in the chunk
        uint32_t count;         // number of connections in the chunk
    };

    // record header format
    using HeaderVariants = std::variant <
        MessageData,
        BagHeader,
        IndexData,
        Chunk,
        ChunkInfo,
        Connection
    >;

    enum class RecordEnum
    {
        MESSAGE_DATA    = 0x02,
        BAG_HEADER      = 0x03,
        INDEX_DATA      = 0x04,
        CHUNK           = 0x05,
        CHUNK_INFO      = 0x06,
        CONNECTION      = 0x07,
        UNKNOWN
    };
    
    template<RecordEnum> struct record_type;
    template<> struct record_type<RecordEnum::MESSAGE_DATA> { using type = MessageData; };
    template<> struct record_type<RecordEnum::BAG_HEADER>   { using type = BagHeader; };
    template<> struct record_type<RecordEnum::INDEX_DATA>   { using type = IndexData; };
    template<> struct record_type<RecordEnum::CHUNK>        { using type = Chunk; };
    template<> struct record_type<RecordEnum::CHUNK_INFO>   { using type = ChunkInfo; };
    template<> struct record_type<RecordEnum::CONNECTION>   { using type = Connection; };
    template<RecordEnum E> using record_type_t = typename record_type<E>::type;

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

        LUX_EXPORT bool searchHeader(SearchCallback callback, void* user_data);

        using TSearchCallback = std::function<void(const char* name, size_t name_len, const uint8_t* data, size_t data_len)>;
        bool TsearchHeader(TSearchCallback callback)
        {
            return searchHeader(&Record::header_search_callback_wrapper, (void*)&callback);
        }

        [[nodiscard]] LUX_EXPORT RecordEnum type() const;

        template<RecordEnum Type, typename Ret = record_type_t<Type>>
        const Ret& view() const
        {
            return std::get<Ret>(_header_view);
        }

        LUX_EXPORT const char* data() const;

        LUX_EXPORT size_t data_len() const;

    private:
        RecordEnum parse_header(HeaderVariants& var);

        void release();

        static void header_search_callback_wrapper(const char* name, size_t name_len, const uint8_t* data, size_t data_len, void* user_data)
        {
            auto func_data = static_cast<TSearchCallback*>(user_data);
            (*func_data)(name, name_len, data, data_len);
        }

        HeaderVariants  _header_view;
        RecordEnum      _type;
        RosbagRawRecord _raw;
    };

}
