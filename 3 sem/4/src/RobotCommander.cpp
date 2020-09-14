// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "RobotCommander.hpp"

namespace MobileRobots
{
    std::map<Coord, std::shared_ptr<MapObject>> RobotCommander::getObjectsAround() const
    {
        auto allObjectsAround{ CommandCenter::collectObjectsAround() };
        allObjectsAround.merge(RobotScout::getObjectsAround());

        return std::move(allObjectsAround);
    }
} // namespace MobileRobots
