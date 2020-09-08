#ifndef __ROBOT_SCOUT_HPP_INCLUDED__
#define __ROBOT_SCOUT_HPP_INCLUDED__

#include "ObservationCenter.hpp"

#include <vector>

namespace MobileRobots
{
	class RobotScout : virtual public ObservationCenter
	{
	public:
		RobotScout() = delete;

		inline RobotScout(const std::string_view description, const unsigned powerUsage, const unsigned maxSlots, const unsigned price, const std::vector<std::shared_ptr<Module>>& modules, const Coord& coord) :
			ObservationCenter(description, powerUsage, maxSlots, price, modules, coord),
			m_isFree{}
		{ }

		inline ~RobotScout() noexcept override = default;

		inline virtual QString toString() const override { return ObservationCenter::toStringHelper("RobotScout"); }

		[[nodiscard]]
		std::map<Coord, std::shared_ptr<MapObject>> getObjectsAround() const override final;

		inline void setPosition(const Coord& coord) noexcept { m_pos = coord; }

	protected:
		bool m_isFree;
	};
} // namespace MobileRobots

#endif /* !__ROBOT_SCOUT_HPP_INCLUDED__ */