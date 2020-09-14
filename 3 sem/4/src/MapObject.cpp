// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MapObject.hpp"
#include "EnvironmentDescriptor.hpp"

namespace MobileRobots
{
	std::shared_ptr<EnvironmentDescriptor> MapObject::sEnvDesc = nullptr;

	void MapObject::setEnvironmentDescriptor(std::shared_ptr<EnvironmentDescriptor> envDesc) noexcept
	{
		MapObject::sEnvDesc = std::move(envDesc);
	}
}