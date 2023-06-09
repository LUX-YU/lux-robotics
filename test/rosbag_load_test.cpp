#include <dataset/Rosbag.hpp>
#include <iostream>
#include <string_view>
#include <chrono>

static void record_print(::lux::robotics::ros::Record& record);

int main(int argc, char* argv[])
{
    using namespace ::lux::robotics::ros;

    Rosbag rosbag("F:\\Resource\\DataSet\\SLAM\\The EuRoC MAV Dataset\\V1_01_easy\\V1_01_easy.bag");
    std::cout << "Bag Size:" << rosbag.bagSize() << std::endl;
    
    {
        auto topic_views = rosbag.sTopicView();
        for (auto topic_view : topic_views)
        {
            std::cout << topic_view << std::endl;
        }
    }

    RosbagVersion version = rosbag.version();

    auto start_time = std::chrono::system_clock::now();
    BinaryMessageView msg_view;
    size_t i;
    for(i = 0; !rosbag.eof(); i++)
    {
        auto message = rosbag.nextBinaryMessageView(msg_view);
    }
    auto end_time = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1e6f;
    std::cout << std::dec << std::fixed << "Load:" << i << " Count, Spend time:" << time << std::endl;

    return 0;
}

#pragma pack(1)
struct IndexDataStruct
{
    uint32_t second;
    uint32_t nanosecond;
    uint32_t offset;
};
#pragma pack()

void record_print(::lux::robotics::ros::Record& record)
{
    using namespace ::lux::robotics::ros;
    switch (record.type())
    {
    case RecordEnum::MESSAGE_DATA:
    {
        const auto& data = record.view<RecordEnum::MESSAGE_DATA>();
        std::cout << "Message Data:" << std::endl;
        std::cout << "conn:" << data.conn << std::endl;
        std::cout << "time:" << data.second << std::endl;
        break;
    }
    case RecordEnum::BAG_HEADER:
    {
        const auto& data = record.view<RecordEnum::BAG_HEADER>();
        std::cout << "Bag Header:" << std::endl;
        std::cout << "chunk_count:" << data.chunk_count << std::endl;
        std::cout << "conn_count:" << data.conn_count << std::endl;
        std::cout << "index_pos:" << data.index_pos << std::endl;
        break;
    }
    case RecordEnum::CHUNK:
    {
        const auto& data = record.view<RecordEnum::CHUNK>();
        std::cout << "Chunk:" << std::endl;
        std::cout << "compression:" << data.compression << std::endl;
        std::cout << "size:" << data.size << std::endl;
        break;
    }
    case RecordEnum::CHUNK_INFO:
    {
        const auto& data = record.view<RecordEnum::CHUNK_INFO>();
        std::cout << "Chunk Info:" << std::endl;
        std::cout << "chunk_pos:" << data.chunk_pos << std::endl;
        std::cout << "conn_count:" << data.conn_count << std::endl;
        std::cout << "end_time:" << data.end_time_second << std::endl;
        std::cout << "start_time:" << data.start_time_second << std::endl;
        std::cout << "ver:" << data.ver << std::endl;
        break;
    }
    case RecordEnum::CONNECTION:
    {
        const auto& data = record.view<RecordEnum::CONNECTION>();
        std::cout << "Connection:" << std::endl;
        std::cout << "conn:" << data.conn << std::endl;
        std::cout << "topic:" << data.topic << std::endl;
        break;
    }
    case RecordEnum::INDEX_DATA:
    {
        const auto& data = record.view<RecordEnum::INDEX_DATA>();
        std::cout << "Index Data:" << std::endl;
        std::cout << "conn:" << data.conn << std::endl;
        std::cout << "count:" << data.count << std::endl;
        std::cout << "ver:" << data.ver << std::endl;
        const char* value_data = record.data();
        const IndexDataStruct* index_data_value = reinterpret_cast<const IndexDataStruct*>(value_data);
        for (uint32_t i = 0; i < data.count; i++)
        {
            std::cout << "second:" << index_data_value[i].second << std::endl;
            std::cout << "nanosecond:" << index_data_value[i].nanosecond << std::endl;
            std::cout << "offset:" << index_data_value[i].offset << std::endl;
        }
        break;
    }
    }
    std::cout << "Data Len:" << record.data_len() << std::endl;
    std::cout << std::endl;
}
