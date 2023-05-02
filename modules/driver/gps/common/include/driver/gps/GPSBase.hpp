#pragma once
#include <cstdint>
#include <functional>
#include <visibility_control.h>

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
		LUX_EXPORT virtual ~GPSBase() = default;

		virtual bool run() = 0;

		virtual bool close() = 0;

		using RawDataUpdateCallback = std::function<void(const GPSRawData&)>;

		LUX_EXPORT void subscribeRawDataUpdated(RawDataUpdateCallback callback);

	protected:
		LUX_EXPORT void onUpdated(const GPSRawData& data);

	private:
		RawDataUpdateCallback _callback;
	};
}
