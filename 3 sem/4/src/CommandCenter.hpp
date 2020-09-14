#ifndef __COMMAND_CENTER_HPP_INCLUDED__
#define __COMMAND_CENTER_HPP_INCLUDED__

#include "ObservationCenter.hpp"
#include "ManagerModule.hpp"

namespace MobileRobots
{
	class CommandCenter : virtual public ObservationCenter, public std::enable_shared_from_this<CommandCenter>
	{
	private:
		bool aquireManager() noexcept;

	protected:
		[[nodiscard]]
		std::map<Coord, std::shared_ptr<MapObject>> collectObjectsAround() const;

	public:
		CommandCenter() = delete;

		CommandCenter(const std::string_view description, const unsigned powerUsage, const unsigned maxSlots, const unsigned price, const std::vector<std::shared_ptr<Module>>& modules, const Coord& coord) :
			ObservationCenter(description, powerUsage, maxSlots, price, modules, coord),
			m_manager()
		{ aquireManager(); }

		inline virtual ~CommandCenter() noexcept override = default;

		inline virtual QString toString() const override { return ObservationCenter::toStringHelper("CommandCenter"); }

		[[nodiscard]]
		inline auto getManager() const noexcept { return m_manager; }

		[[nodiscard]]
		virtual std::map<Coord, std::shared_ptr<MapObject>> getObjectsAround() const override;

		void aquireDevices() noexcept;

	protected:
		std::shared_ptr<ManagerModule> m_manager;
	};
} // namespace MobileRobots

#endif /* !__COMMAND_CENTER_HPP_INCLUDED__ */