#include <utils/dataset_support/KITTI.hpp>
#include <fstream>
#include <ctime>

namespace lux::robotics
{
	// Oxford Technical Solutions
	/* data format
	/* lat:				latitude of the oxts-unit (deg)
	/* lon:				longitude of the oxts-unit (deg)
	/* alt:				altitude of the oxts-unit (m)
	/* roll:			roll angle (rad),    0 = level, positive = left side up,      range: -pi   .. +pi
	/* pitch:			pitch angle (rad),   0 = level, positive = front down,        range: -pi/2 .. +pi/2
	/* yaw:				heading (rad),       0 = east,  positive = counter clockwise, range: -pi   .. +pi
	/* vn:				velocity towards north (m/s)
	/* ve:				velocity towards east (m/s)
	/* vf:				forward velocity, i.e. parallel to earth-surface (m/s)
	/* vl:				leftward velocity, i.e. parallel to earth-surface (m/s)
	/* vu:				upward velocity, i.e. perpendicular to earth-surface (m/s)
	/* ax:				acceleration in x, i.e. in direction of vehicle front (m/s^2)
	/* ay:				acceleration in y, i.e. in direction of vehicle left (m/s^2)
	/* az:				acceleration in z, i.e. in direction of vehicle top (m/s^2)
	/* af:				forward acceleration (m/s^2)
	/* al:				leftward acceleration (m/s^2)
	/* au:				upward acceleration (m/s^2)
	/* wx:				angular rate around x (rad/s)
	/* wy:				angular rate around y (rad/s)
	/* wz:				angular rate around z (rad/s)
	/* wf:				angular rate around forward axis (rad/s)
	/* wl:				angular rate around leftward axis (rad/s)
	/* wu:				angular rate around upward axis (rad/s)
	/* pos_accuracy:	velocity accuracy (north/east in m)
	/* vel_accuracy:	velocity accuracy (north/east in m/s)
	/* navstat:			navigation status (see navstat_to_string)
	/* numsats:			number of satellites tracked by primary GPS receiver
	/* posmode:			position mode of primary GPS receiver (see gps_mode_to_string)
	/* velmode:			velocity mode of primary GPS receiver (see gps_mode_to_string)
	/* orimode:			orientation mode of primary GPS receiver (see gps_mode_to_string)
	**/
	class OXTSLoader
	{
	public:
		struct DataFormat
		{
			double	lat;
			double	lon;
			double	alt;
			double	roll;
			double	pitch;
			double	yaw;
			double	vn;
			double	ve;
			double	vf;
			double	vl;
			double	vu;
			double	ax;
			double	ay;
			double	az;
			double	af;
			double	al;
			double	au;
			double	wx;
			double	wy;
			double	wz;
			double	wf;
			double	wl;
			double	wu;
			double	pos_accuracy;
			double	vel_accuracy;
			int		navstat;
			int		numsats;
			int		posmode;
			int		velmode;
			int		orimode;

			size_t	timestamp;
		};

		// typecally, oxts directory
		static std::vector<DataFormat> load(const std::filesystem::path& path)
		{
			std::vector<DataFormat> ret;
			// load date
			std::ifstream fstream(path / "timestamps.txt", std::ios::in);
			std::string single_line;
			int			year, month, day, hour, minute;
			float		seconds;
			DataFormat	data;
			while (std::getline(fstream, single_line))
			{
				auto nfield = std::sscanf(
					single_line.c_str(), 
					"%d-%d-%d %d:%d:%f", 
					&year, &month, &day, &hour, &minute, &seconds
				);
				if (nfield != 6) break;
				size_t nanoseconds = seconds * 1e9;

				struct tm tm;
				tm.tm_year	= year - 1900;
				tm.tm_mon	= month - 1;
				tm.tm_mday	= day;
				tm.tm_hour	= hour;
				tm.tm_min	= minute;
				tm.tm_isdst = 0;
				tm.tm_sec	= 0;

				size_t second_from_1970 = std::mktime(&tm);
				data.timestamp = second_from_1970 * 1e9 + nanoseconds;
				ret.push_back(data);
			}
			return ret;
		}
	};

	KITTIRawData::KITTIRawData(std::filesystem::path path)
	{
		OXTSLoader::load(path / "oxts");
	}
}