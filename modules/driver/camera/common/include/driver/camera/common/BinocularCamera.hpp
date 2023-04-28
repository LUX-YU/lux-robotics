#pragma once
#include "CameraBase.hpp"

namespace lux::robotic
{
    class BinocularCamera : public CameraBase
    {
    public:
        // get distance between two camera, unit : m
        virtual double camera_distance() const = 0;
    };
} // namespace lux
