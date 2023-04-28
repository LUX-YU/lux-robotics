#pragma once
#include <memory>
#include <opencv2/opencv.hpp>
#include <visibility_control.h>

namespace lux::robotic
{
    class VideoCapture
    {
    public:
        LUX_EXPORT virtual ~VideoCapture() = default;

        virtual bool grab() = 0;

        virtual bool retrieve(cv::Mat&, int extra_parameter = 0) = 0;

        virtual bool isOpened() const = 0;

        virtual VideoCapture& operator>>(cv::Mat& mat)
        {
            cv::VideoCapture vc;
            retrieve(mat);
            return *this;
        }
    };

    class CVVideoCapture : public VideoCapture
    {
    public:
        template<typename... T>
        CVVideoCapture(T&&... args)
        : _capture(std::forward<T>(args)...){}

        bool grab() override
        {
            return _capture.grab();
        }

        bool retrieve(cv::Mat& mat, int extra_parameter = 0) override
        {
            return _capture.retrieve(mat, extra_parameter);
        }

        bool isOpened() const override
        {
            return _capture.isOpened();
        }

    private:
        cv::VideoCapture _capture;
    };
} // namespace lux::robotic
