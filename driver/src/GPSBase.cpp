#include <lux/driver/gps/GPSBase.hpp>

namespace lux::robotics
{
	void GPSBase::subscribeRawDataUpdated(RawDataUpdateCallback callback)
	{
		_callback = std::move(callback);
	}

	void GPSBase::onUpdated(const GPSRawData& data)
	{
		if (_callback)
		{
			_callback(data);
		}
	}
}