// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AI.hpp"
#include "EnvironmentDescriptor.hpp"
#include "CommandCenter.hpp"
#include "Coord.hpp"

#include <queue>

namespace MobileRobots
{
	std::queue<Coord> AI::BFS(const Coord& from, const Coord& to)
	{
		std::unordered_map<Coord, bool> visited;

		std::queue<Coord> queue;
		queue.push(from);

		while (!queue.empty())
		{
			auto&& cur = queue.front();
			queue.pop();

			visited[cur] = true;
			if (Coord left{ cur.x - 1, cur.y }; m_envDescr->isInField(left) && isExplored(left))
				queue.push(left);
			if (Coord above{ cur.x, cur.y + 1 }; m_envDescr->isInField(above) && isExplored(above))
				queue.push(above);
			if (Coord right{ cur.x + 1, cur.y }; m_envDescr->isInField(right) && isExplored(right))
				queue.push(right);
			if (Coord below{ cur.x, cur.y - 1 }; m_envDescr->isInField(below) && isExplored(below))
				queue.push(below);
		}
		
		return {};
	}

	Route AI::makeRoute(const Coord& from, const Coord& to)
	{
		return Route(std::move(BFS(from, to)));
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

			for (const auto&[coord, _] : std::dynamic_pointer_cast<ObservationCenter>(object)->getObjectsAround())
				m_tasks.insert(coord);
		}

		/*std::queue<Coord> q;
		q.push({ 0, 1 });
		q.push({ 0, 2 });
		q.push({ 0, 3 });
		q.push({ 0, 4 });
		q.push({ 1, 4 });
		q.push({ 2, 4 });
		q.push({ 3, 4 });
		q.push({ 3, 3 });
		q.push({ 3, 2 });
		q.push({ 3, 1 });

		m_routes.emplace(m_commanders[0], Route(std::move(q)));*/
	}

	void AI::addExploredPoint(const Coord& coord, std::shared_ptr<MapObject> object /* = nullptr */)
	{
		if (auto&& [_, suc] = m_map.try_emplace(coord, object); suc)
			m_mapUpdates.insert(coord);
	}

	void AI::work()
	{
		for (auto& commander : m_commanders)
		{
			auto&& objectsAround{ commander->getObjectsAround() };
			for (const auto& [coord, object] : objectsAround)
			{
				if (m_map.find(coord) == std::end(m_map))
					m_tasks.insert(coord);

				addExploredPoint(coord, object);
			}
		}

		for (auto& commander : m_commanders)
			if (typeid(*commander) == typeid(RobotCommander) && m_routes.find(commander) == std::end(m_routes))
				for (auto& to : m_tasks)
					if (auto&& route = makeRoute(commander->getPos(), to); !route.isFinished())
					{
						m_routes.emplace(commander, route);

						break;
					}

		for (auto&& [object, route] : m_routes)
		{
			if (route.isFinished())
			{
				auto&& to = route.to();

				// m_map.try_emplace(to, m_envDescr->getObject(to));
				m_tasks.erase(to);

				// TODO: try to give new task

				continue;
			}

			std::dynamic_pointer_cast<RobotScout>(object)->setPosition(route.getNext());
		}
	}
} // namespace MobileRobots