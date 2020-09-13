// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AI.hpp"
#include "EnvironmentDescriptor.hpp"
#include "CommandCenter.hpp"
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
	
		static auto tryAdd = [&](const Coord cur, const Coord coord)
		{
			if (m_envDescr->isInField(coord) && !used[coord] && isExplored(coord) && m_map.at(coord) == nullptr && owner.distanceTo(coord) < ownerRadius)
			{
				queue.push(coord);

				used[coord] = true;

				prev[coord] = cur;
			}
		};

		queue.push(from);
		used[from] = true;
		prev[from] = { -1U, -1U };
		while (!queue.empty())
		{
			auto&& cur = queue.front();
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
		while (prev[cur] != Coord{ -1U, -1U })
		{
			revPath.push_back(cur);
			cur = prev[cur];
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
			auto&& objectsAround{ commander->getObjectsAround() };
			for (const auto& [coord, object] : objectsAround)
			{
				if (!isExplored(coord))
					m_tasks.insert(coord);

				addExploredPoint(coord, object);
			}
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

	AI::AI(std::shared_ptr<EnvironmentDescriptor> envDescr) :
		m_envDescr(envDescr),
		m_map(),
		m_tasks(),
		m_managers(),
		m_commanders(),
		m_routes()
	{
		for (const auto& object : m_envDescr->getObjects())
		{
			if (typeid(*object) == typeid(CommandCenter) || typeid(*object) == typeid(RobotCommander))
			{
				auto& commandCenter{ std::dynamic_pointer_cast<CommandCenter>(object) };
				if (commandCenter->getManager())
					m_managers.push_back(commandCenter->getManager());

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
		explore();

		for (auto& commander : m_commanders) // Create route
		{
			if (typeid(*commander) == typeid(RobotCommander) && !hasTask(std::static_pointer_cast<MapObject>(commander)))
				for (auto& to : m_tasks)
					if (auto&& route = makeRoute(commander->getPos(), to); route)
					{
						m_routes.emplace(commander, route);

						break;
					}
		}

		move();
	}
} // namespace MobileRobots