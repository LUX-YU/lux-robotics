#pragma once
#include <memory>
#include <opencv2/opencv.hpp>
#include <lux/cxx/visibility_control.h>

namespace lux::robotics
{
    class VideoCapture
    {
    public:
        LUX_EXPORT virtual ~VideoCapture() = default;

        virtual bool grab() = 0;

        virtual bool retrieve(cv::Mat&, int extra_parameter) = 0;

        virtual VideoCapture& operator>>(cv::Mat& mat)
        {
            cv::VideoCapture vc;
            grab();
            retrieve(mat, 0);
            return *this;
        }
    };

    class CVVideoCapture : public VideoCapture
    {
    public:
        template<typename... T>
        explicit CVVideoCapture(T&&... args)
        : _capture(std::forward<T>(args)...){}

        bool grab() override
        {
            return _capture.grab();
        }

        bool retrieve(cv::Mat& mat, int extra_parameter = 0) override
        {
            return _capture.retrieve(mat, extra_parameter);
        }

        [[nodiscard]] bool isOpened() const
        {
            return _capture.isOpened();
        }

        VideoCapture& operator>>(cv::Mat& mat) override
        {
            _capture >> mat;
        }

    private:
        cv::VideoCapture _capture;
    };
} // namespace lux::robotic
