#include <driver/camera/ImageSequenceCapture.hpp>

int main(int argc, char* argv[])
{
    using namespace ::lux::robotics;

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
