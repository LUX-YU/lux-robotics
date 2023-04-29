//
// Created by ChenHui on 2023/4/28.
//

#pragma once
#include <driver/camera/VideoCapture.hpp>
#include <visibility_control.h>
#include "ImageSequence.hpp"

namespace lux::robotics
{
    class ImageSequenceCapture : public VideoCapture, public MakePTRHelper<ImageSequenceCapture>
    {
    public:
        LUX_EXPORT explicit ImageSequenceCapture(std::shared_ptr<ImageSequence>, size_t buffer_size = 10);

        LUX_EXPORT ~ImageSequenceCapture() override;

        LUX_EXPORT bool grab() override;

        LUX_EXPORT void reset();

        LUX_EXPORT bool retrieve(cv::Mat&, int extra_parameter = 0) override;

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
    };
}
