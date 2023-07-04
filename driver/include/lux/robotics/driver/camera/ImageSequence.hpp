//
// Created by ChenHui on 2023/4/28.
//

#ifndef LUX_ROBOTIC_IMAGESEQUENCE_H
#define LUX_ROBOTIC_IMAGESEQUENCE_H
#include <string>
#include <memory>
#include <filesystem>
#include <functional>
#include <lux/robotics/visibility.h>
#include <lux/robotics/driver/camera/CameraBase.hpp>

namespace lux::robotics
{
    using FilePathList = std::vector<std::filesystem::path>;

    class ImageSequence : public CameraBase, public MakePTRHelper<ImageSequence>
    {
        friend class ImageSequenceCapture;
    public:
        LUX_ROBOTICS_PUBLIC explicit ImageSequence(FilePathList list);

        LUX_ROBOTICS_PUBLIC explicit ImageSequence(const std::filesystem::path&, bool recursive = false);

        LUX_ROBOTICS_PUBLIC ~ImageSequence() override;

        LUX_ROBOTICS_PUBLIC void sortBy(const std::function<void(FilePathList)>& func);

        // may be blocked
        LUX_ROBOTICS_PUBLIC bool open() override;

        // may be blocked
        LUX_ROBOTICS_PUBLIC bool close() override;

        [[nodiscard]] LUX_ROBOTICS_PUBLIC bool isOpened() const override;

        [[nodiscard]] LUX_ROBOTICS_PUBLIC bool isBusy() const override;

    private:
        [[nodiscard]] const FilePathList& image_list() const;

        void setBusy(bool);

        class Impl;
        std::unique_ptr<Impl> _impl;
    };

}

#endif //LUX_ROBOTIC_IMAGESEQUENCE_H
