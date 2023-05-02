#include <dataset/KITTI.hpp>
#include <string>
#include <fstream>
#include <ctime>

namespace lux::robotics
{
	class OXTSLoader::Impl
	{
	public:
		explicit Impl(std::filesystem::path oxfs_directory)
		: _oxts_directory(std::move(oxfs_directory)),
		  _data_directory_iterator(_oxts_directory / "data"),
		  _timestamp_fstream(_oxts_directory / "timestamps.txt", std::ios::in)
		{
			_is_open = std::filesystem::is_directory(_oxts_directory / "data") && _timestamp_fstream.is_open();
		}

		bool isOpened() const
		{
			return _is_open;
		}

		bool read_one(OXTSFormat& data)
		{
			namespace fs = ::std::filesystem;
			// end of directory
			if (fs::begin(_data_directory_iterator) == fs::end(_data_directory_iterator))
				return false;
			// get file path
			const auto& data_file_path = *(_data_directory_iterator++);

			// end of file
			if (_timestamp_fstream.eof())
				return false;

			std::string timestamp_line;
			getline(_timestamp_fstream, timestamp_line);

			data.timestamp = parse_timestamp(timestamp_line);
			return parse_data_file(data_file_path, data) && data.timestamp != 0;
		}

		size_t updated(std::vector<OXTSFormat>& list, size_t number)
		{
			if (list.size() < number) list.resize(number);

			size_t i = 0;
			for (; i < number; i++)
			{
				auto& last_elem = list[i];
				if (!read_one(last_elem))
				{
					break;
				}
			}
			return i;
		}

		size_t append(std::vector<OXTSFormat>& list, size_t number)
		{
			size_t i = 0;
			for (; i < number; i++)
			{
				OXTSFormat data;
				if (!read_one(data))
				{
					break;
				}
				list.push_back(std::move(data));
			}
			return i;
		}

		size_t loadAll(std::vector<OXTSFormat>& list)
		{
			size_t i = 0;
			while (true)
			{
				OXTSFormat data;
				if (!read_one(data))
				{
					break;
				}
				list.push_back(std::move(data));
				i++;
			}
			return i;
		}

		bool eof()
		{
			return _timestamp_fstream.eof();
		}

	private:

		static uint64_t parse_timestamp(const std::string& line)
		{
			int			year, month, day, hour, minute;
			double		seconds;
			auto nfield = std::sscanf(
				line.c_str(),
				"%d-%d-%d %d:%d:%lf",
				&year, &month, &day, &hour, &minute, &seconds
			);
			if (nfield != 6) return 0;
			uint64_t nanoseconds = static_cast<uint64_t>(seconds * 1e9);

			struct tm tm;
			tm.tm_year = year - 1900;
			tm.tm_mon = month - 1;
			tm.tm_mday = day;
			tm.tm_hour = hour;
			tm.tm_min = minute;
			tm.tm_isdst = 0;
			tm.tm_sec = 0;

			auto second_from_1970 = std::mktime(&tm);
			return static_cast<uint64_t>(second_from_1970 * 1e9) + nanoseconds;
		}

		static bool parse_data_file(const std::filesystem::path& data_file_path, OXTSFormat& out)
		{
			static const int   number_of_field = 30;
			static const char* format_str =
				"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf"
				"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d %d %d %d %d";

			std::ifstream fstream(data_file_path);

			if (!fstream.is_open()) return false;

			std::string buffer;
			std::getline(fstream, buffer);

			int field = sscanf(buffer.c_str(), format_str,
				&out.lat,	&out.lon,   &out.alt,
				&out.roll,	&out.pitch, &out.yaw,
				&out.vn,	&out.ve,	&out.vf,	&out.vl,	&out.vu,
				&out.ax,	&out.ay,	&out.az,
				&out.af,	&out.al,	&out.au,
				&out.wx,	&out.wy,	&out.wz,
				&out.wf,	&out.wl,	&out.wu,
				&out.pos_accuracy,		&out.vel_accuracy,
				&out.navstat,			&out.numsats,
				&out.posmode,			&out.velmode,
				&out.orimode
			);

			return field == number_of_field;
		}

		bool									_is_open;
		std::filesystem::path					_oxts_directory;
		std::ifstream							_timestamp_fstream;
		std::filesystem::directory_iterator		_data_directory_iterator;
	};

	OXTSLoader::OXTSLoader(std::filesystem::path path)
	{
		_impl = std::make_unique<Impl>(std::move(path));
	}

	bool OXTSLoader::isOpened() const
	{
		return _impl->isOpened();
	}

	OXTSLoader::~OXTSLoader() = default;

	size_t OXTSLoader::updated(std::vector<OXTSFormat>& list, size_t number)
	{
		return _impl->updated(list, number);
	}

	size_t OXTSLoader::append(std::vector<OXTSFormat>& list, size_t number)
	{
		return _impl->append(list, number);
	}

	bool OXTSLoader::eof() const
	{
		return _impl->eof();
	}

	size_t OXTSLoader::loadAll(std::vector<OXTSFormat>& list)
	{
		return _impl->loadAll(list);
	}

	KITTIRawData::KITTIRawData(std::filesystem::path path)
	{
		
	}
}