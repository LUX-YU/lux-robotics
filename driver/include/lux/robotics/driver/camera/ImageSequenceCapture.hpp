//
// Created by ChenHui on 2023/4/28.
//

#pragma once
#include <lux/robotics/driver/camera/VideoCapture.hpp>
#include <lux/robotics/visibility.h>
#include "ImageSequence.hpp"

namespace lux::robotics
{
    class ImageSequenceCapture : public VideoCapture, public MakePTRHelper<ImageSequenceCapture>
    {
    public:
        LUX_ROBOTICS_PUBLIC explicit ImageSequenceCapture(std::shared_ptr<ImageSequence>, size_t buffer_size = 10);

        LUX_ROBOTICS_PUBLIC ~ImageSequenceCapture() override;

        LUX_ROBOTICS_PUBLIC bool grab() override;

        LUX_ROBOTICS_PUBLIC void reset();

        LUX_ROBOTICS_PUBLIC bool retrieve(cv::Mat&, int extra_parameter = 0) override;

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };
}
