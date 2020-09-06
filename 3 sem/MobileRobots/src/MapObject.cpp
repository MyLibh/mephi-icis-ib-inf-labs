#include "MapObject.hpp"
#include "EnvironmentDescriptor.hpp"

namespace MobileRobots
{
	std::shared_ptr<EnvironmentDescriptor> MapObject::sEnvDesc = nullptr;

	void MapObject::setEnvironmentDescriptor(std::shared_ptr<EnvironmentDescriptor> envDesc) noexcept
	{
		MapObject::sEnvDesc = envDesc;
	}
}