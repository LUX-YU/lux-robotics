//
// Created by ChenHui on 2023/4/28.
//

#ifndef LUX_ROBOTIC_IMAGESEQUENCE_H
#define LUX_ROBOTIC_IMAGESEQUENCE_H
#include <string>
#include <memory>
#include <filesystem>
#include <functional>
#include <visibility_control.h>
#include <driver/camera/CameraBase.hpp>

namespace lux::robotics
{
    using FilePathList = std::vector<std::filesystem::path>;

    class ImageSequence : public CameraBase, public MakePTRHelper<ImageSequence>
    {
        friend class ImageSequenceCapture;
    public:
        LUX_EXPORT explicit ImageSequence(FilePathList list);

        LUX_EXPORT explicit ImageSequence(const std::filesystem::path&, bool recursive = false);

        LUX_EXPORT ~ImageSequence() override;

        LUX_EXPORT void sortBy(const std::function<void (FilePathList)>& func);

        // may be blocked
        LUX_EXPORT bool open() override;

        // may be blocked
        LUX_EXPORT bool close() override;

        [[nodiscard]] LUX_EXPORT bool isOpened() const override;

        [[nodiscard]] LUX_EXPORT bool isBusy() const override;

    private:
        [[nodiscard]] const FilePathList& image_list() const;

        void setBusy(bool);

        class Impl;
        std::unique_ptr<Impl> _impl;
    };

}

#endif //LUX_ROBOTIC_IMAGESEQUENCE_H
