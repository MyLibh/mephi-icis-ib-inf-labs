// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ObservationCenter.hpp"
#include "PowerGenerator.hpp"
#include "Sensor.hpp"

#include <typeinfo>
#include <algorithm>

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

    std::map<Coord, std::weak_ptr<MapObject>> ObservationCenter::getObjectsAround() const
    {
        std::map<Coord, std::weak_ptr<MapObject>> objectsAround;
        for (const auto& module : m_modules)
            if (typeid(*module) == typeid(Sensor))
            {
                auto sensor{ std::dynamic_pointer_cast<Sensor>(module) };

                // TODO: check surroundings
            }

        return std::move(objectsAround);
    }
}