#ifndef __AI_HPP_INCLUDED__
#define __AI_HPP_INCLUDED__

#include "CommandCenter.hpp"
#include "Route.hpp"

#include <map>
#include <set>

namespace MobileRobots
{
	class EnvironmentDescriptor;

	class AI final : public std::enable_shared_from_this<AI>
	{
	private:
		Route makeRoute(const Coord& from, const Coord& to, const Coord& owner, const unsigned ownerRadius = std::numeric_limits<unsigned>::max()) const;

		inline Route makeRoute(const Coord& from, const Coord& to) const { return std::move(makeRoute(from, to, from)); }

		void explore();

		void move();

		void addExploredPoint(const Coord& coord, std::shared_ptr<MapObject> object = nullptr);

		inline bool hasTask(std::shared_ptr<MapObject> object) { return m_routes.find(object) == std::end(m_routes); }

	public:
		AI() = delete;

		explicit AI(std::shared_ptr<EnvironmentDescriptor> envDescr);

		[[nodiscard]]
		const auto& getExploredObjects() const noexcept { return m_map; }

		[[nodiscard]]
		inline auto getMapUpdates() noexcept { return std::move(m_mapUpdates); }

		inline bool isExplored(const Coord& coord) const noexcept { return m_map.find(coord) != std::end(m_map); }		

		

		void work();

	private:
		std::shared_ptr<EnvironmentDescriptor>                m_envDescr;
		std::map<Coord, const std::shared_ptr<MapObject>>     m_map;
		std::set<Coord>                                       m_tasks;
		std::vector<std::shared_ptr<ManagerModule>>           m_managers;
		std::vector<std::shared_ptr<CommandCenter>>           m_commanders;
		std::unordered_map<std::shared_ptr<MapObject>, Route> m_routes;
		std::set<Coord>                                       m_mapUpdates;
	};
} // namespace MobileRobots

#endif /* !__AI_HPP_INCLUDED__ */