// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "RobotCommander.hpp"

#include <typeinfo>

namespace MobileRobots
{
    std::shared_ptr<RobotScout> RobotCommander::getControlledDevice() const
    {
        for (const auto& device : m_manager->getDevices())
            if (device->getOwner() == shared_from_this() && typeid(*device) == typeid(RobotScout))
                return std::dynamic_pointer_cast<RobotScout>(device);

        return nullptr;
    }

    std::map<Coord, std::shared_ptr<MapObject>> RobotCommander::getObjectsAround() const
    {
        auto allObjectsAround{ collectObjectsAround() };
        allObjectsAround.merge(RobotScout::getObjectsAround());

        return std::move(allObjectsAround);
    }

    bool RobotCommander::addControlledDevice(std::shared_ptr<RobotScout> robotScout)
    {
        if (!robotScout->getOwner())
        {
            if (!m_manager->addDevice(robotScout))
                return false;

            robotScout->setOwner(std::static_pointer_cast<ObservationCenter>(shared_from_this()));
            ObservationCenter::setOwner(std::static_pointer_cast<ObservationCenter>(shared_from_this()));

            return true;
        }

        return false;
    }
} // namespace MobileRobots
