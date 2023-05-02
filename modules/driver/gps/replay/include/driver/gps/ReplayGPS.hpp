#pragma once
#include <visibility_control.h>
#include <driver/gps/GPSBase.hpp>
#include <vector>
#include <memory>

namespace lux::robotics
{
	class ReplayGPS : public GPSBase
	{
	public:
		LUX_EXPORT ReplayGPS();

		LUX_EXPORT ~ReplayGPS() override;

		using DataList = std::vector<GPSRawData>;

		LUX_EXPORT explicit ReplayGPS(DataList list);
		
		/**
		 * call this function before calling run
		 */
		LUX_EXPORT bool setReplayData(DataList list);

		LUX_EXPORT bool run() override;

		LUX_EXPORT bool close() override;

	private:
		class Impl;
		std::unique_ptr<Impl> _impl;
	};
}
