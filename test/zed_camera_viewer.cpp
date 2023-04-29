#include <driver/camera/ZEDCamera.hpp>
#include <iostream>

int main()
{
    using namespace lux::robotics;

    sl::InitParameters parameter;
    parameter.camera_fps = 15;
    parameter.camera_resolution = sl::RESOLUTION::HD1080;
    parameter.depth_mode = sl::DEPTH_MODE::NONE;

    auto zed_camera_ptr = ZEDCamera::make_shared(parameter);

    if(!zed_camera_ptr->open())
    {
        std::cerr << "Camera not open" << std::endl;
        return -1;
    }

    std::cerr << "Camera was opened" << std::endl;

    auto video_capture  = ZEDVideoCapture::make_shared(zed_camera_ptr, ZEDView::LEFT);

    cv::Mat mat;
    while(true)
    {
        if(!video_capture->grab())
        {
            break;
        }

        video_capture->retrieve(mat);

        cv::imshow("test", mat);

        if(cv::waitKey(3) == 'q')
        {
            break;
        }
    }

    return 0;
}
