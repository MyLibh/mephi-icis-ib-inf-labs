// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AI.hpp"
#include "EnvironmentDescriptor.hpp"
#include "Coord.hpp"
#include "Utility.hpp"

namespace MobileRobots
{
	Route AI::makeRoute(const Coord& from, const Coord& to, const Coord& owner, const unsigned ownerRadius) const
	{
		if (owner.distanceTo(to) > ownerRadius)
			return { };

		std::unordered_map<Coord, bool>  used;
		std::unordered_map<Coord, Coord> prev;
		std::queue<Coord>                queue;
	
		auto wasUsed = [&](const Coord& coord) { return used.find(coord) != std::end(used); };

		auto tryAdd = [&](const Coord& cur, const Coord coord)
		{
			if (m_envDescr->isInField(coord) &&
				!wasUsed(coord) &&
				isExplored(coord) &&
				m_map.at(coord) == nullptr &&
				owner.distanceTo(coord) <= ownerRadius
				)
			{
				queue.push(coord);

				used.emplace(coord, true);

				prev.emplace(coord, cur);
			}
		};

		queue.push(from);
		used.emplace(from, true);
		prev.insert({ from, { -1U, -1U } });
		while (!queue.empty())
		{
			auto cur = queue.front();
			queue.pop();

			tryAdd(cur, { cur.x - 1, cur.y     }); // left
			tryAdd(cur, { cur.x    , cur.y + 1 }); // below
			tryAdd(cur, { cur.x + 1, cur.y     }); // right
			tryAdd(cur, { cur.x    , cur.y - 1 }); // above
		}
		
		if (prev.find(to) == std::end(prev))
			return { };

		Coord cur = to;
		std::vector<Coord> revPath;
		while (prev.at(cur) != Coord{ -1U, -1U })
		{
			revPath.push_back(cur);
			cur = prev.at(cur);
		}

		std::queue<Coord> path;
		for (auto coord : Utility::reverse(revPath))
			path.push(coord);

		return Route(std::move(path));
	}

	void AI::explore()
	{
		for (auto& commander : m_commanders)
		{
			if (typeid(*commander) == typeid(RobotCommander))
				commander->aquireDevices();

			auto&& objectsAround{ commander->getObjectsAround() };
			for (const auto& [coord, object] : objectsAround)
			{
				if (!isExplored(coord))
				{
					m_tasks.insert(coord);

					if (typeid(*commander) == typeid(RobotCommander))
					{
						auto& [it, _] = m_cache.try_emplace(std::dynamic_pointer_cast<RobotCommander>(commander), std::stack<Coord>{});
						it->second.push(coord);
					}
				}

				addExploredPoint(coord, object);
			}

			if (typeid(*commander) == typeid(RobotCommander))
				const_cast<std::vector<std::shared_ptr<ObservationCenter>>&>(commander->getManager()->getDevices()).clear();
		}
	}

	void AI::move()
	{
		for (auto it = std::begin(m_routes); it != std::end(m_routes);)
			if (auto& [object, route] = *it; route.isFinished())
			{
				auto&& to = route.to();

				addExploredPoint(to); // envDescr->getObject
				m_tasks.erase(to);

				m_routes.erase(it++);
			}
			else
			{
				auto scout = std::dynamic_pointer_cast<RobotScout>(object);
				m_tasks.erase(scout->getPos());
				scout->setPosition(route.getNext());

				++it;
			}
	}

	void AI::addExploredPoint(const Coord& coord, std::shared_ptr<MapObject> object /* = nullptr */)
	{
		if (auto&& [_, suc] = m_map.try_emplace(coord, object); suc)
			m_mapUpdates.insert(coord);
	}

	bool AI::makeTask(std::shared_ptr<CommandCenter> commander)
	{
		if (hasTask(std::dynamic_pointer_cast<RobotScout>(commander)))
			return false;

		if (auto robotCommander = std::dynamic_pointer_cast<RobotCommander>(commander); m_cache.find(robotCommander) != std::end(m_cache))
			while (!m_cache.at(robotCommander).empty())
			{
				auto to = m_cache.at(robotCommander).top();
				m_cache.at(robotCommander).pop();

				if (auto&& route = makeRoute(robotCommander->getPos(), to); route)
				{
					m_routes.emplace(std::dynamic_pointer_cast<RobotScout>(robotCommander), route);

					return true;
				}
			}
	
		for (auto& to : m_tasks)
			if (auto&& route = makeRoute(commander->getPos(), to); route)
			{
				m_routes.emplace(std::dynamic_pointer_cast<RobotScout>(commander), route);

				return true;
			}

		return false;
	}

	bool AI::makeTask(std::shared_ptr<CommandCenter> commander, std::shared_ptr<ObservationCenter> device)
	{
		if (hasTask(std::dynamic_pointer_cast<RobotScout>(device)))
			return false;

		for (auto& to : m_tasks)
			if (auto&& route = makeRoute(device->getPos(), to, commander->getPos(), commander->getManager()->getRadius()); route)
			{
				m_routes.emplace(std::dynamic_pointer_cast<RobotScout>(device), route);

				return true;
			}

		return false;
	}

	AI::AI(std::shared_ptr<EnvironmentDescriptor> envDescr) :
		m_envDescr(envDescr),
		m_map(),
		m_tasks(),
		m_commanders(),
		m_routes(),
		m_finished{},
		m_cache()
	{
		for (const auto& object : m_envDescr->getObjects())
		{
			if (typeid(*object) == typeid(CommandCenter) || typeid(*object) == typeid(RobotCommander))
			{
				auto& commandCenter{ std::dynamic_pointer_cast<CommandCenter>(object) };
				if (typeid(*object) == typeid(CommandCenter))
					commandCenter->aquireDevices();

				if (commandCenter->getManager())
					m_commanders.push_back(commandCenter);
			}

			if (typeid(*object) == typeid(RobotScout) || typeid(*object) == typeid(RobotCommander))
				addExploredPoint(object->getPos());
			else if (typeid(*object) == typeid(ObservationCenter) || typeid(*object) == typeid(CommandCenter))
				addExploredPoint(object->getPos(), object);
			else
				continue;
		}
	}

	void AI::work()
	{
		move();

		explore();

		for (auto& commander : m_commanders)
		{
			if (typeid(*commander) == typeid(RobotCommander))
				makeTask(commander);
			else if (typeid(*commander) == typeid(CommandCenter))
				for (auto& device : commander->getManager()->getDevices())
					if (typeid(*device) == typeid(RobotScout))
						makeTask(commander, device);
		}

		if (m_tasks.empty())
			m_finished = true;
	}
} // namespace MobileRobots