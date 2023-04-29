#pragma once
#include <string>
#include <functional>
#include <memory>
#include <opencv2/opencv.hpp>

namespace lux::robotics
{
    template<typename T>
    class MakePTRHelper
    {
    public:
        template<class... Args>
        static inline std::shared_ptr<T> make_shared(Args&&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

        template<class... Args>
        static inline std::shared_ptr<T> make_unique(Args&&... args)
        {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }
    };

    class CameraBase
    {
    public:
        virtual ~CameraBase() = default;

        virtual std::string sendCommand(const std::string& command)
        {
            static std::string default_result = "not support";
            return default_result;
        }

        // may be block
        virtual bool open() = 0;

        // may be block
        virtual bool close() = 0;

        [[nodiscard]] virtual bool isOpened() const = 0;

        [[nodiscard]] virtual bool isBusy() const = 0;
    };
} // namespace lux::robotic
