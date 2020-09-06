// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "EnvironmentDescriptor.hpp"

namespace MobileRobots
{
    const std::shared_ptr<MapObject> EnvironmentDescriptor::getObject(const Coord& coord) const noexcept
    {
        for (const auto& object : m_objects)
            if (object->getPos() == coord)
                return object;

        return nullptr;
    }
} // namespace MobileRobots
