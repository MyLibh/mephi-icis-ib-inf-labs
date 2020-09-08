// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MapLoader.hpp"
#include "EnvironmentDescriptor.hpp"
#include "PowerGenerator.hpp"
#include "Sensor.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include <vector>

namespace detail
{
    using namespace MobileRobots;

#define loadUInt(name) static_cast<unsigned>(object.value(QString(name)).toInt())
#define loadString(name) object.value(QString(name)).toString().toUtf8()

    std::shared_ptr<Module> loadModule(const QJsonObject& object) 
    {
        if (const auto&& type = loadString("type"); type == "PowerGenerator")
            return std::make_shared<PowerGenerator>(loadUInt("powerSupplyLevel"), loadUInt("priority"));
        else if (type == "ManagerModule")
            return std::make_shared<ManagerModule>(loadUInt("maxDevices"), loadUInt("powerUsage"), loadUInt("radius"), loadUInt("priority"));
        else if (type == "Sensor")
            return std::make_shared<Sensor>(loadUInt("direction"), loadUInt("angle"), loadUInt("powerUsage"), loadUInt("radius"), loadUInt("priority"));

        return nullptr;
    }

    void loadObjects(const QJsonArray& array, std::shared_ptr<EnvironmentDescriptor> envDescr)
    {
        for (const auto& item : array)
        {
            const auto&& object = item.toObject();

            std::vector<std::shared_ptr<Module>> modules;
            for (const auto& item : object.value(QString("modules")).toArray())
                modules.push_back(loadModule(item.toObject()));

            if (const auto&& type = loadString("type"); type == "Barrier")
                envDescr->addObject<Barrier>(Coord{ loadUInt("x"), loadUInt("y") });
            else if (type == "InterestingObject")
                envDescr->addObject<InterestingObject>(Coord{ loadUInt("x"), loadUInt("y") });
            else if (type == "RobotScout")
                envDescr->addObject<RobotScout>(loadString("description").toStdString(), loadUInt("powerUsage"), loadUInt("maxSlots"), loadUInt("price"), modules, Coord{ loadUInt("x"), loadUInt("y") });
            else if (type == "RobotCommander")
                envDescr->addObject<RobotCommander>(loadString("description").toStdString(), loadUInt("powerUsage"), loadUInt("maxSlots"), loadUInt("price"), modules, Coord{ loadUInt("x"), loadUInt("y") });
            else if (type == "ObservationCenter")
                envDescr->addObject<ObservationCenter>(loadString("description").toStdString(), loadUInt("powerUsage"), loadUInt("maxSlots"), loadUInt("price"), modules, Coord{ loadUInt("x"), loadUInt("y") });
            else if (type == "CommandCenter")
                envDescr->addObject<CommandCenter>(loadString("description").toStdString(), loadUInt("powerUsage"), loadUInt("maxSlots"), loadUInt("price"), modules, Coord{ loadUInt("x"), loadUInt("y") });
        }
    }

#undef loadString
#undef loadUInt
} // namespace detail

std::shared_ptr<MobileRobots::EnvironmentDescriptor> MobileRobots::MapLoader::load(const QString& path)
{
    QJsonObject json;
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << file.errorString();

            return nullptr;
        }

        json = QJsonDocument::fromJson(file.readAll()).object();

        file.close();
    }

    auto envDescr = std::make_shared<EnvironmentDescriptor>(json.value(QString("width")).toInt(), json.value(QString("height")).toInt());
    MapObject::setEnvironmentDescriptor(envDescr->shared_from_this());

    detail::loadObjects(json.value(QString("objects")).toArray(), envDescr);

    return std::move(envDescr);
}