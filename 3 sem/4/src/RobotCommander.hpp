#ifndef __ROBOT_COMMANDER_HPP_INCLUDED__
#define __ROBOT_COMMANDER_HPP_INCLUDED__

#include "RobotScout.hpp"
#include "CommandCenter.hpp"

namespace MobileRobots
{
	class RobotCommander final : public RobotScout, public CommandCenter
	{
	public:
		RobotCommander() = delete;

		inline RobotCommander(const std::string_view description, const unsigned powerUsage, const unsigned maxSlots, const unsigned price, const std::vector<std::shared_ptr<Module>>& modules, const Coord& coord) :
			RobotScout(description, powerUsage, maxSlots, price, modules, coord),
			CommandCenter(description, powerUsage, maxSlots, price, modules, coord),
			ObservationCenter(description, powerUsage, maxSlots, price, modules, coord)
		{ }

		inline ~RobotCommander() noexcept override = default;

		inline QString toString() const override { return ObservationCenter::toStringHelper("RobotCommander"); }

		[[nodiscard]]
		std::map<Coord, std::shared_ptr<MapObject>> getObjectsAround() const override;
	};
} // namespace MobileRobots

#endif /* !__ROBOT_COMMANDER_HPP_INCLUDED__ */