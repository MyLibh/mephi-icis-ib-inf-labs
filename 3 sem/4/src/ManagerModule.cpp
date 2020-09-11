// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ManagerModule.hpp"
#include "AI.hpp"

namespace MobileRobots
{
	std::shared_ptr<AI> ManagerModule::sAI = nullptr;

	void ManagerModule::setAI(std::shared_ptr<AI> ai) noexcept
	{
		ManagerModule::sAI = std::move(ai);
	}

	bool ManagerModule::isExplored(const Coord& coord) const noexcept
	{
		return sAI->isExplored(coord);
	}
}
