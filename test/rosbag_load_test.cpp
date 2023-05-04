#include <dataset/Rosbag.hpp>
#include <iostream>
#include <string_view>

void search_method(const char* name, size_t name_len, const uint8_t* data, size_t data_len)
{
    std::cout << "name:" << std::string_view(name, name_len) << " data:";
    for (size_t i = 0; i < data_len; i++)
    {
        std::cout << std::hex << std::uppercase << (int)data[i];
    }

    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    using namespace ::lux::robotics::ros;

    Rosbag rosbag("F:\\Resource\\DataSet\\SLAM\\The EuRoC MAV Dataset\\V1_01_easy\\V1_01_easy.bag");
    
    RosbagVersion version = rosbag.version();

    auto record = rosbag.nextRecord();

    record.searchHeader(search_method);

    record = rosbag.nextRecord();
    record.searchHeader(search_method);

    record = rosbag.nextRecord();
    record.searchHeader(search_method);

    record = rosbag.nextRecord();
    record.searchHeader(search_method);

    record = rosbag.nextRecord();
    record.searchHeader(search_method);

    record = rosbag.nextRecord();
    record.searchHeader(search_method);
    return 0;
}
