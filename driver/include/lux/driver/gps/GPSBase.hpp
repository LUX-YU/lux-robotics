#pragma once
#include <cstdint>
#include <functional>
#include <lux/robotics/visibility.h>

namespace lux::robotics
{
	struct GPSRawData
	{
		uint64_t	timestamp;
		
		double		lat;
		double		lon;
		double		alt;

		float		eph;
		float		epv;
	};

	class GPSBase
	{
	public:
		LUX_ROBOTICS_PUBLIC virtual ~GPSBase() = default;

		virtual bool run() = 0;

		virtual bool close() = 0;

		using RawDataUpdateCallback = std::function<void(const GPSRawData&)>;

		LUX_ROBOTICS_PUBLIC void subscribeRawDataUpdated(RawDataUpdateCallback callback);

	protected:
		LUX_ROBOTICS_PUBLIC void onUpdated(const GPSRawData& data);

	private:
		RawDataUpdateCallback _callback;
	};
}
