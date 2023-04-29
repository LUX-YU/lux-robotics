#pragma once
#include <filesystem>
#include <visibility_control.h>

namespace lux::robotics
{
	class LUX_EXPORT KITTIRawData
	{
	public:
		explicit KITTIRawData(std::filesystem::path);
	};
}
