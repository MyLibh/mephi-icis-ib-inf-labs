#ifndef __COMMAND_CENTER_HPP_INCLUDED__
#define __COMMAND_CENTER_HPP_INCLUDED__

#include "ObservationCenter.hpp"
#include "ManagerModule.hpp"

namespace MobileRobots
{
	class CommandCenter : virtual public ObservationCenter
	{
	public:
		CommandCenter() = delete;

		CommandCenter(const std::string_view description, const unsigned powerUsage, const unsigned maxSlots, const unsigned price, const std::vector<std::shared_ptr<Module>>& modules, const Coord& coord) :
			ObservationCenter(description, powerUsage, maxSlots, price, modules, coord),
			m_manager()
		{ }

		inline virtual ~CommandCenter() noexcept override = default;

		inline virtual QString toString() const override { return ObservationCenter::toStringHelper("CommandCenter"); }

		bool aquireManager() noexcept;

		bool aquireDevice();

	protected:
		std::shared_ptr<ManagerModule> m_manager;
	};
} // namespace MobileRobots

#endif /* !__COMMAND_CENTER_HPP_INCLUDED__ */