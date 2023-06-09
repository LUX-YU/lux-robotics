#include "driver/camera/ZEDCamera.hpp"

namespace lux::robotics
{
    ZEDCamera::ZEDCamera(const sl::InitParameters& parameter)
        : _init_parameters(parameter), _is_busy(false)
    {
    }

    ZEDCamera::~ZEDCamera()
    {
        if (isOpened()) close();
    }

    const sl::InitParameters& ZEDCamera::parameter() const
    {
        return _init_parameters;
    }

    bool ZEDCamera::open()
    {
        return _camera.open() == sl::ERROR_CODE::SUCCESS;
    }

    bool ZEDCamera::close()
    {
        _camera.close();
        return true;
    }

    bool ZEDCamera::isOpened() const
    {
        bool is_opened = _camera.isOpened();
        return is_opened;
    }

    bool ZEDCamera::isBusy() const
    {
        return _is_busy;
    }

    double ZEDCamera::camera_distance() const
    {
        return 3;
    }

    /****** Video Capture ******/

    ZEDVideoCapture::ZEDVideoCapture(std::shared_ptr<ZEDCamera> camera, ZEDView view)
        : _camera_wrapper(std::move(camera)), _view(view)
    {
        _camera_wrapper->_is_busy = true;
    }

    ZEDVideoCapture::~ZEDVideoCapture()
    {
        _camera_wrapper->_is_busy = false;
    }

    bool ZEDVideoCapture::grab()
    {
        return _camera_wrapper->_camera.grab() == sl::ERROR_CODE::SUCCESS;
    }

    bool ZEDVideoCapture::retrieve(cv::Mat& mat, int extra_parameter)
    {
        sl::Mat sl_mat;
        auto view = _view == ZEDView::LEFT ? sl::VIEW::LEFT : sl::VIEW::RIGHT;
        _camera_wrapper->_camera.retrieveImage(sl_mat, view);

        // Convert sl::Mat to cv::Mat (share buffer)
        mat = cv::Mat(
            (int)sl_mat.getHeight(),
            (int)sl_mat.getWidth(),
            CV_8UC4,
            sl_mat.getPtr<sl::uchar1>(sl::MEM::CPU)
        );

        return true;
    }

    ZEDView ZEDVideoCapture::view() const
    {
        return this->_view;
    }
}