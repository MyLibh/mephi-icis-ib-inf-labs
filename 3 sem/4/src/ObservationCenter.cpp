// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ObservationCenter.hpp"
#include "PowerGenerator.hpp"
#include "Sensor.hpp"
#include "EnvironmentDescriptor.hpp"

#include <typeinfo>
#include <algorithm>

namespace detail
{
    inline bool isInSector(const MobileRobots::Coord& coord, const unsigned radius, const unsigned startAngle, const unsigned angle) noexcept
    {
        auto ang = Math::rad2deg(std::atan(coord.y / coord.x));

        return Math::hypot(coord.x, coord.y) < radius && startAngle <= ang && ang <= startAngle + angle;
    }
} // namespace detail

namespace MobileRobots
{
    void ObservationCenter::initModules()
    {
        unsigned powerSupplySum{};
        unsigned powerUsageSum{ m_powerUsage };

        for (const auto& module : m_modules)
            if (typeid(*module) == typeid(PowerGenerator))
                powerSupplySum += std::dynamic_pointer_cast<PowerGenerator>(module)->getPowerSupplyLevel();
            else
                powerUsageSum += std::dynamic_pointer_cast<EnergyConsumer>(module)->getPowerUsage();

        std::sort(std::begin(m_modules), std::begin(m_modules), [](const auto& lhs, const auto& rhs) { return lhs->getPriority() > rhs->getPriority(); });

        for (size_t i{}; i < m_modules.size() && powerUsageSum > powerSupplySum; ++i)
            if (typeid(*m_modules[i]) != typeid(PowerGenerator))
            {
                m_modules[i]->turnOff();

                powerUsageSum -= std::dynamic_pointer_cast<EnergyConsumer>(m_modules[i])->getPowerUsage();
            }

        // TODO: check bad conditions
    }

    QString ObservationCenter::toStringHelper(const QString& type) const
    {
        QString str = MapObject::toStringHelper(type);
        str.append(QString("<b>description:</b> %1<br/><b>powerUsage:</b> %2<br/><b>maxSlots:</b> %3<br/><b>price:</b> %4<br/><b>modules:</b><br/><br>").arg(m_descr.c_str()).arg(m_powerUsage).arg(m_maxSlots).arg(m_price));

        for (const auto& module : m_modules)
            str.append(module->toString() + "<br/>");

        return std::move(str);
    }

    std::map<Coord, std::shared_ptr<MapObject>> ObservationCenter::getObjectsAround() const
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

                for (x = xMin; x <= xMax; ++x)
                    for (y = yMin; y <= yMax; ++y)
                        if (const Coord coord{ x, y }; coord != m_pos && detail::isInSector(coord, r, sensor->getDirection() * 90, sensor->getAngle()))
                            objectsAround.emplace(coord, sEnvDesc->getObject(coord));
            }

        return std::move(objectsAround);
    }
}