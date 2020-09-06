#ifndef __AI_HPP_INCLUDED__
#define __AI_HPP_INCLUDED__

#include "Coord.hpp"

#include <map>

namespace MobileRobots
{
	class AI final : public std::enable_shared_from_this<AI>
	{
	public:
		inline bool isExplored(const Coord& coord) const noexcept { return m_map.find(coord) != std::end(m_map); }

	private:
		std::map<Coord, std::weak_ptr<MapObject>> m_map;
	};
} // namespace MobileRobots

#endif /* !__AI_HPP_INCLUDED__ */