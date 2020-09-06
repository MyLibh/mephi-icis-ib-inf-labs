// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CommandCenter.hpp"

#include <typeinfo>

namespace MobileRobots
{
	bool MobileRobots::CommandCenter::aquireManager() noexcept
	{
		m_manager = nullptr;

		auto r = 0U;
		for (auto& module : m_modules)
			if (typeid(*module) == typeid(ManagerModule) && module->isActive() && std::dynamic_pointer_cast<ManagerModule>(module)->getRadius() > r)
			{
				m_manager = std::dynamic_pointer_cast<ManagerModule>(module);
				r = m_manager->getRadius();
			}

		return m_manager != nullptr;
	}

	bool CommandCenter::aquireDevice()
	{
		return false;
	}
} // namespace MobileRobots
