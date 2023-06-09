#include <lux/driver/ReplayGPS.hpp>
#include <dataset/KITTI.hpp>
#include <matplot/matplot.h>
#include <fstream>
#include <string>
#include <string_view>

int main(int argc, char* argv[])
{
    using namespace ::lux::robotics;

    std::vector<OXTSFormat> buffer;
    OXTSLoader kitti("F:/Resource/DataSet/SLAM/KITTI/CITY/2011_09_26_drive_0001_extract/oxts");
    if (!kitti.isOpened()) return 0;

    int count = kitti.loadAll(buffer);
    std::vector<double> lat;
    std::vector<double> lon;
    /*
    OXTSFormat first = buffer[0];
    std::for_each(
        buffer.begin(),
        buffer.end(),
        [first](OXTSFormat& elem) {
            elem.lat = (elem.lat - first.lat) * 1e6;
            elem.lon = (elem.lon - first.lon) * 1e6;
        }
    );
    */
    for (const auto& data : buffer)
    {
        lat.push_back(data.lat * 1e9);
        lon.push_back(data.lon * 1e9);
    }

    matplot::plot(lat, lon, "g-*");
    matplot::show();
    return 0;
}
