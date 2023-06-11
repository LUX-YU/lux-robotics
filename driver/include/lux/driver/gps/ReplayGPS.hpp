#pragma once
#include <lux/robotics/visibility.h>
#include <lux/driver/gps/GPSBase.hpp>
#include <vector>
#include <memory>

namespace lux::robotics
{
	class ReplayGPS : public GPSBase
	{
	public:
		LUX_ROBOTICS_PUBLIC ReplayGPS();

		LUX_ROBOTICS_PUBLIC ~ReplayGPS() override;

		using DataList = std::vector<GPSRawData>;

		LUX_ROBOTICS_PUBLIC explicit ReplayGPS(DataList list);

		/**
		 * call this function before calling run
		 */
		LUX_ROBOTICS_PUBLIC bool setReplayData(DataList list);

		LUX_ROBOTICS_PUBLIC bool run() override;

		LUX_ROBOTICS_PUBLIC bool close() override;

	private:
		class Impl;
		std::unique_ptr<Impl> _impl;
	};
}
