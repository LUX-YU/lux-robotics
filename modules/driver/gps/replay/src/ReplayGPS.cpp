#include <driver/gps/ReplayGPS.hpp>
#include <mutex>
#include <thread>
#include <atomic>

namespace lux::robotics
{
	class ReplayGPS::Impl
	{
	public:
		Impl(ReplayGPS* owner)
		: _owner(owner){
			_should_quit = false;
			_is_run = false;
		}

		explicit Impl(DataList list, ReplayGPS* owner)
		: _replay_list(std::move(list)), _owner(owner)
		{
			_should_quit = false;
			_is_run = false;
		}

		~Impl()
		{
			close();
		}

		bool run()
		{
			if (_is_run)
				return false;
			_is_run = true;

			std::thread([this]() {this->thead_method(); }).swap(_replay_thread);
			return true;
		}

		bool close()
		{
			if (!_is_run)
				return false;
			_should_quit = true;

			if (_replay_thread.joinable())
			{
				_replay_thread.join();
				_is_run = false;
			}

			return true;
		}

		bool setReplayData(DataList list)
		{
			if (_is_run) 
				return false;
			_replay_list = std::move(list);

			return true;
		}

		void thead_method()
		{
			size_t		index = 0;
			uint64_t	last_timestamp = !_replay_list.empty() ? _replay_list[0].timestamp : 0;
			while (!_should_quit && index < _replay_list.size())
			{
				const auto& gps_data = _replay_list[index++];

				std::this_thread::sleep_for(std::chrono::nanoseconds(gps_data.timestamp - last_timestamp));

				last_timestamp = gps_data.timestamp;

				_owner->onUpdated(gps_data);
			}

			_is_run = false;
		}

	private:
		ReplayGPS*			_owner;
		DataList			_replay_list;
		std::atomic_bool	_is_run;
		std::atomic_bool	_should_quit;
		std::thread			_replay_thread;
	};

	ReplayGPS::ReplayGPS()
	{
		_impl = std::make_unique<Impl>(this);
	}

	ReplayGPS::ReplayGPS(DataList list)
	{
		_impl = std::make_unique<Impl>(std::move(list),this);
	}

	ReplayGPS::~ReplayGPS() = default;

	bool ReplayGPS::setReplayData(DataList list)
	{
		return _impl->setReplayData(std::move(list));
	}

	bool ReplayGPS::run()
	{
		return _impl->run();
	}

	bool ReplayGPS::close()
	{
		return _impl->close();
	}
}
