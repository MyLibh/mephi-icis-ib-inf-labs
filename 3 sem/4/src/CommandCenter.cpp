// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CommandCenter.hpp"
#include "EnvironmentDescriptor.hpp"

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

	std::map<Coord, std::shared_ptr<MapObject>> CommandCenter::collectObjectsAround() const
	{
		if (!m_manager)
			return {};

		std::map<Coord, std::shared_ptr<MapObject>> devicesObjectsAround;
		for (const auto& device : m_manager->getDevices())
		{
			auto objectsAround{ device->getObjectsAround() };
			devicesObjectsAround.insert(std::make_move_iterator(std::begin(objectsAround)), std::make_move_iterator(std::end(objectsAround)));
		}

		return std::move(devicesObjectsAround);
	}

	std::map<Coord, std::shared_ptr<MapObject>> CommandCenter::getObjectsAround() const
	{
		auto objectsAround = ObservationCenter::getObjectsAround();

		auto allObjectsAround = collectObjectsAround();
		allObjectsAround.insert(std::make_move_iterator(std::begin(objectsAround)), std::make_move_iterator(std::end(objectsAround)));

		return std::move(allObjectsAround);
	}

	void CommandCenter::aquireDevices() noexcept
	{
		if (!m_manager)
			return;

		ObservationCenter::setOwner(std::static_pointer_cast<MapObject>(shared_from_this()));

		const auto r = m_manager->getRadius();

		auto x = getX();
		const auto xMin = x < r ? 0U : x - r;
		const auto xMax = std::min(x + r, sEnvDesc->getWidth() - 1);

		auto y = getY();
		const auto yMin = y < r ? 0U : y - r;
		const auto yMax = std::min(y + r, sEnvDesc->getHeight() - 1);

		for (x = xMin; x <= xMax; ++x)
			for (y = yMin; y <= yMax; ++y)
				if (const Coord coord{ x, y }; coord != m_pos)
						if (auto device = std::dynamic_pointer_cast<ObservationCenter>(sEnvDesc->getObject(coord)); 
							device &&
							!device->getOwner() &&
							m_manager->addDevice(device)
							)
						{
							device->setOwner(shared_from_this());
						}
	}
} // namespace MobileRobots
