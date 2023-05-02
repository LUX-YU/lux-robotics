#pragma once
#include <filesystem>
#include <visibility_control.h>

namespace lux::robotics
{
	// Oxford Technical Solutions
	struct OXTSFormat
	{
		double	lat;			// latitude of the oxts-unit (deg)
		double	lon;			// longitude of the oxts-unit (deg)
		double	alt;			// altitude of the oxts-unit (m)
		double	roll;			// roll angle (rad),    0 = level, positive = left side up,      range: -pi   .. +pi
		double	pitch;			// pitch angle (rad),   0 = level, positive = front down,        range: -pi/2 .. +pi/2
		double	yaw;			// heading (rad),       0 = east,  positive = counter clockwise, range: -pi   .. +pi
		double	vn;				// velocity towards north (m/s)
		double	ve;				// velocity towards east (m/s)
		double	vf;				// forward velocity, i.e. parallel to earth-surface (m/s)
		double	vl;				// leftward velocity, i.e. parallel to earth-surface (m/s)
		double	vu;				// upward velocity, i.e. perpendicular to earth-surface (m/s)
		double	ax;				// acceleration in x, i.e. in direction of vehicle front (m/s^2)
		double	ay;				// acceleration in y, i.e. in direction of vehicle left (m/s^2)
		double	az;				// acceleration in z, i.e. in direction of vehicle top (m/s^2)
		double	af;				// forward acceleration (m/s^2)
		double	al;				// leftward acceleration (m/s^2)
		double	au;				// upward acceleration (m/s^2)
		double	wx;				// angular rate around x (rad/s)
		double	wy;				// angular rate around y (rad/s)
		double	wz;				// angular rate around z (rad/s)
		double	wf;				// angular rate around forward axis (rad/s)
		double	wl;				// angular rate around leftward axis (rad/s)
		double	wu;				// angular rate around upward axis (rad/s)
		double	pos_accuracy;	// velocity accuracy (north/east in m)
		double	vel_accuracy;	// velocity accuracy (north/east in m/s)
		int		navstat;		// navigation status (see navstat_to_string)
		int		numsats;		// number of satellites tracked by primary GPS receiver
		int		posmode;		// position mode of primary GPS receiver (see gps_mode_to_string)
		int		velmode;		// velocity mode of primary GPS receiver (see gps_mode_to_string)
		int		orimode;		// orientation mode of primary GPS receiver (see gps_mode_to_string)

		uint64_t	timestamp;
	};

	class OXTSLoader
	{
	public:
		LUX_EXPORT explicit OXTSLoader(std::filesystem::path path);

		LUX_EXPORT ~OXTSLoader();

		LUX_EXPORT bool isOpened() const;

		LUX_EXPORT size_t updated(std::vector<OXTSFormat>&, size_t number);

		LUX_EXPORT size_t append(std::vector<OXTSFormat>&, size_t number);

		LUX_EXPORT size_t loadAll(std::vector<OXTSFormat>&);

		[[nodiscard]] LUX_EXPORT  bool eof() const;

	private:
		class Impl;
		std::unique_ptr<Impl> _impl;
	};

	class KITTIRawData
	{
	public:
		LUX_EXPORT explicit KITTIRawData(std::filesystem::path);

		// LUX_EXPORT bool 

	};
}
