#include <lux/driver/camera/ImageSequenceCapture.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace lux::robotics
{
    class ImageSequenceCapture::Impl
    {
        friend class ImageSequenceCapture;
    public:
        Impl(std::shared_ptr<ImageSequence> image_sequence, size_t buffered_image)
            : _virtual_camera(std::move(image_sequence)), _buffer_size(buffered_image), _read_index(0)
        {
            _thread_quit = false;
            std::thread([this]() {load_image_thread(); }).swap(_load_thread);
        }

        ~Impl()
        {
            {
                std::scoped_lock lck(_pool_mutex);
                _thread_quit = true;
                _cv.notify_all();
            }
            if (_load_thread.joinable())
                _load_thread.join();
        }

        bool grab()
        {
            const auto& file_list = _virtual_camera->image_list();
            std::unique_lock lck(_pool_mutex);
            if (_read_index == file_list.size() && _image_pool.empty())
                return false;

            while (_image_pool.empty() && !_thread_quit)
            {
                _cv.wait(lck);
            }

            if (_thread_quit) return false;

            _mat = _image_pool.front();
            _image_pool.pop();

            _cv.notify_one();

            return true;
        }

        bool retrieve(cv::Mat& mat, int extra_parameter)
        {
            mat = _mat;
            return !_mat.empty();
        }

    private:

        void load_image_thread()
        {
            const auto& file_list = _virtual_camera->image_list();
            while (!_thread_quit)
            {
                std::unique_lock lck(_pool_mutex);
                if (_read_index >= file_list.size()) break;

                while (_image_pool.size() >= _buffer_size && !_thread_quit)
                {
                    _cv.wait(lck);
                }

                if (_thread_quit)
                    break;

                lck.unlock();
                cv::Mat tmp = cv::imread(file_list[_read_index].string());
                lck.lock();

                _image_pool.push(std::move(tmp));
                _read_index++;

                _cv.notify_one();
            }
        }

        void reset()
        {
            std::scoped_lock lck(_pool_mutex);
            std::queue<cv::Mat> new_pool;
            _image_pool.swap(new_pool);
            _read_index = 0;
        }

        size_t                          _buffer_size;
        /** protect variable **/
        std::queue<cv::Mat>             _image_pool;
        size_t                          _read_index;
        std::shared_ptr<ImageSequence>  _virtual_camera;
        /** protect variable **/

        std::thread                     _load_thread;
        std::atomic<bool>               _thread_quit;
        std::condition_variable         _cv;
        std::mutex                      _pool_mutex;

        cv::Mat                         _mat;
    };

    ImageSequenceCapture::ImageSequenceCapture(std::shared_ptr<ImageSequence> _camera, size_t buffer_size)
    {
        _impl = std::make_unique<Impl>(std::move(_camera), buffer_size);
        _impl->_virtual_camera->setBusy(true);
    }

    ImageSequenceCapture::~ImageSequenceCapture()
    {
        _impl->_virtual_camera->setBusy(false);
    }

    bool ImageSequenceCapture::grab()
    {
        return _impl->grab();
    }

    bool ImageSequenceCapture::retrieve(cv::Mat& mat, int extra_parameter)
    {
        return _impl->retrieve(mat, extra_parameter);
    }

    void ImageSequenceCapture::reset()
    {

    }
}