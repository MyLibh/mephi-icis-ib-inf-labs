// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "AI.hpp"
#include "EnvironmentDescriptor.hpp"
#include "CommandCenter.hpp"

namespace detail
{
	using MobileRobots::Coord;

	std::queue<Coord> BFS(const Coord& to)
	{
		std::unordered_map<Coord, bool> visited;
		std::list<Coord> queue;

		while (!queue.empty());
	}
} // namespace detail

namespace MobileRobots
{
	Route AI::makeRoute(const Coord& from, const Coord& where)
	{
		return Route({});
	}

	AI::AI(std::shared_ptr<EnvironmentDescriptor> envDescr) :
		m_envDescr(envDescr),
		m_map(),
		m_tasks()
	{
		for (const auto& object : m_envDescr->getObjects())
		{
			if (typeid(*object) == typeid(RobotScout) || typeid(*object) == typeid(RobotCommander))
				m_map.emplace(object->getPos(), nullptr);
			else if (typeid(*object) == typeid(ObservationCenter) || typeid(*object) == typeid(CommandCenter))
				m_map.emplace(object->getPos(), object);
			else
				continue;

			for (const auto&[coord, _] : std::dynamic_pointer_cast<ObservationCenter>(object)->getObjectsAround())
				m_tasks.insert(coord);
		}
	}

	void AI::work()
	{
		/*
		for (auto& x : x)
			if (!x->isBusy() && !m_tasks.empty())
				x->addTask(makeRoute(*m_tasks.begin()));
		*/
	}
} // namespace MobileRobots