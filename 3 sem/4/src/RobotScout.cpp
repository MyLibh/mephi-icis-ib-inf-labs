// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "RobotScout.hpp"
#include "Sensor.hpp"
#include "EnvironmentDescriptor.hpp"
#include "PowerGenerator.hpp"

namespace MobileRobots
{
    void RobotScout::redrawModules() noexcept
    {
        for (auto& module : m_modules) 
            if (module->isActive() && typeid(*module) != typeid(PowerGenerator)) 
                std::dynamic_pointer_cast<EnergyConsumer>(module)->getGraphicsItem()->setPos(getX() * 32, getY() * 32); 
    }

    std::map<Coord, std::shared_ptr<MapObject>> RobotScout::getObjectsAround() const
    {
        std::map<Coord, std::shared_ptr<MapObject>> objectsAround;
        for (const auto& module : m_modules)
            if (typeid(*module) == typeid(Sensor))
            {
                auto sensor{ std::dynamic_pointer_cast<Sensor>(module) };

                const auto r = sensor->getRadius();

                auto x = getX();              
                const auto xMin = x < r ? 0U : x - r;                      
                const auto xMax = std::min(x + r, sEnvDesc->getWidth() - 1);

                auto y = getY();
                const auto yMin = y < r ? 0U : y - r;
                const auto yMax = std::min(y + r, sEnvDesc->getHeight() - 1);

                for (x = xMin; x <= xMax; ++x) // TODO: check in circle
                    for (y = yMin; y <= yMax; ++y)
                        if (const Coord coord{ x, y }; coord != m_pos)
                            objectsAround.emplace(coord, sEnvDesc->getObject(coord));
            }

        return std::move(objectsAround);
    }
} // namespace MobileRobots