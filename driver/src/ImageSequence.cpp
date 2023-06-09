//
// Created by ChenHui on 2023/4/28.
//

#include "lux/driver/camera/ImageSequence.hpp"
#include <string_view>

namespace lux::robotics
{
    static bool is_image(const std::filesystem::path& file)
    {
        static std::vector<std::string> support_extensions{
            ".jpg", ".jpeg", ".png", ".tif", ".tiff"
        };
        if (std::filesystem::is_directory(file)) return false;

        std::string extension = file.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(),
            [](unsigned char c) { return std::tolower(c); });

        auto iter = std::find(
            support_extensions.begin(),
            support_extensions.end(),
            extension
        );

        return iter != support_extensions.end();
    }

    class ImageSequence::Impl
    {
        friend class ImageSequence;
    public:
        explicit Impl(FilePathList list) : _file_path_list(std::move(list)) {}

        template<typename IteratorType>
        void search_image(const std::filesystem::path& directory)
        {
            for (const std::filesystem::path& file : IteratorType{ directory }) {
                if (!is_image(file)) continue;
                _file_path_list.push_back(file);
            }
        }

        Impl(const std::filesystem::path& directory, bool recursive)
        {
            recursive ?
                search_image<std::filesystem::recursive_directory_iterator>(directory) :
                search_image<std::filesystem::directory_iterator>(directory);
        }

        bool open()
        {
            return true;
        }

        // may be blocked
        bool close()
        {
            return true;
        }

        [[nodiscard]] bool isOpened() const
        {
            return true;
        }

        [[nodiscard]] bool isBusy() const
        {
            return _is_busy;
        }

    private:

        bool         _is_busy{false};
        FilePathList _file_path_list;
    };

    ImageSequence::ImageSequence(FilePathList list)
    {
        _impl = std::make_unique<Impl>(std::move(list));
    }

    ImageSequence::ImageSequence(const std::filesystem::path& path, bool recursive)
    {
        _impl = std::make_unique<Impl>(path, recursive);
    }

    ImageSequence::~ImageSequence() = default;

    bool ImageSequence::open()
    {
        return _impl->open();
    }

    void ImageSequence::sortBy(const std::function<void(FilePathList)>& func)
    {
        func(_impl->_file_path_list);
    }

    // may be blocked
    bool ImageSequence::close()
    {
        return _impl->close();
    }

    bool ImageSequence::isOpened() const
    {
        return _impl->isOpened();
    }

    bool ImageSequence::isBusy() const
    {
        return _impl->isBusy();
    }

    const FilePathList& ImageSequence::image_list() const {
        return _impl->_file_path_list;
    }

    void ImageSequence::setBusy(bool state)
    {
        _impl->_is_busy = state;
    }
}