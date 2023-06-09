#pragma once
#include <cstdint>
#include <functional>
#include <lux/cxx/visibility_control.h>

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
		LUX_PUBLIC virtual ~GPSBase() = default;

		virtual bool run() = 0;

		virtual bool close() = 0;

		using RawDataUpdateCallback = std::function<void(const GPSRawData&)>;

		LUX_PUBLIC void subscribeRawDataUpdated(RawDataUpdateCallback callback);

	protected:
		LUX_PUBLIC void onUpdated(const GPSRawData& data);

	private:
		RawDataUpdateCallback _callback;
	};
}
