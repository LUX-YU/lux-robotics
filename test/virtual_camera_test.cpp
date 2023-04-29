#include <driver/camera/ImageSequenceCapture.hpp>
#include <utils/dataset_support/KITTI.hpp>

int main(int argc, char* argv[])
{
    using namespace ::lux::robotics;

    KITTIRawData kitti("F:/Resource/DataSet/SLAM/KITTI/CITY/2011_09_26_drive_0001_sync");

    auto virtual_camera = ImageSequence::make_shared(R"(F:\Resource\DataSet\SLAM\The EuRoC MAV Dataset\V1_03_difficult\mav0\cam0\data)");

    ImageSequenceCapture capture{virtual_camera};
    cv::Mat mat;
    while(capture.grab())
    {
        capture.retrieve(mat);
        cv::imshow("test", mat);

        if(cv::waitKey(33) == 'q') break;
    }

    return 0;
}
