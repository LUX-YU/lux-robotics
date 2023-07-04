#pragma once
#include "driver/camera/BinocularCamera.hpp"
#include "driver/camera/VideoCapture.hpp"
#include <visibility_control.h>
#include <sl/Camera.hpp> // ZED include
#include <memory>

namespace lux::robotics
{
    enum class ZEDView
    {
        LEFT,
        RIGHT
    };

    class ZEDCamera : public BinocularCamera, public MakePTRHelper<ZEDCamera>
    {
        friend class ZEDVideoCapture;
    public:
        LUX_ROBOTICS_PUBLIC explicit ZEDCamera(const sl::InitParameters& parameter);

        ~ZEDCamera() override;

        LUX_ROBOTICS_PUBLIC const sl::InitParameters& parameter() const;

        LUX_ROBOTICS_PUBLIC bool open() override;

        LUX_ROBOTICS_PUBLIC bool close() override;

        LUX_ROBOTICS_PUBLIC bool isOpened() const override;

        LUX_ROBOTICS_PUBLIC bool isBusy() const override;

        LUX_ROBOTICS_PUBLIC double camera_distance() const override;

    private:

        bool                _is_busy;
        sl::InitParameters  _init_parameters;
        mutable sl::Camera  _camera;
    };

    class ZEDVideoCapture : public VideoCapture, public MakePTRHelper<ZEDVideoCapture>
    {
    public:
        LUX_ROBOTICS_PUBLIC explicit ZEDVideoCapture(std::shared_ptr<ZEDCamera> camera, ZEDView view);

        LUX_ROBOTICS_PUBLIC ~ZEDVideoCapture();

        // The function `grab()` will grab two camera image, no matter the parameter passed to the constructor.
        LUX_ROBOTICS_PUBLIC bool grab() override;

        LUX_ROBOTICS_PUBLIC bool retrieve(cv::Mat&, int extra_parameter = 0) override;

        LUX_ROBOTICS_PUBLIC ZEDView view() const;

    private:
        ZEDView                     _view;
        std::shared_ptr<ZEDCamera>  _camera_wrapper;
    };
} // namespace lux::robotic
