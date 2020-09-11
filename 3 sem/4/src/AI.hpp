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
		Route makeRoute(const Coord& from, const Coord& where);

	public:
		AI() = delete;

		explicit AI(std::shared_ptr<EnvironmentDescriptor> envDescr);

		[[nodiscard]]
		const auto& getExploredObjects() const noexcept { return m_map; }

		inline bool isExplored(const Coord& coord) const noexcept { return m_map.find(coord) != std::end(m_map); }		

		void work();

	private:
		std::shared_ptr<EnvironmentDescriptor>                m_envDescr;
		std::map<Coord, const std::shared_ptr<MapObject>>     m_map;
		std::set<Coord>                                       m_tasks;
		std::vector<std::shared_ptr<ManagerModule>>           m_managers;
		std::vector<std::shared_ptr<CommandCenter>>           m_commanders;
		std::unordered_map<std::shared_ptr<MapObject>, Route> m_routes;
	};
} // namespace MobileRobots

#endif /* !__AI_HPP_INCLUDED__ */