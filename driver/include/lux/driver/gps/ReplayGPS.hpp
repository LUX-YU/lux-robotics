#pragma once
#include <lux/cxx/visibility_control.h>
#include <lux/driver/gps/GPSBase.hpp>
#include <vector>
#include <memory>

namespace lux::robotics
{
	class ReplayGPS : public GPSBase
	{
	public:
		LUX_PUBLIC ReplayGPS();

		LUX_PUBLIC ~ReplayGPS() override;

		using DataList = std::vector<GPSRawData>;

		LUX_PUBLIC explicit ReplayGPS(DataList list);
		
		/**
		 * call this function before calling run
		 */
		LUX_PUBLIC bool setReplayData(DataList list);

		LUX_PUBLIC bool run() override;

		LUX_PUBLIC bool close() override;

	private:
		class Impl;
		std::unique_ptr<Impl> _impl;
	};
}
