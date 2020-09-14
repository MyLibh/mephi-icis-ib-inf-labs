#ifndef __MANAGER_MODULE_HPP_INCLUDED__
#define __MANAGER_MODULE_HPP_INCLUDED__

#include "EnergyConsumer.hpp"
#include "ObservationCenter.hpp"

#include <vector>

namespace MobileRobots
{
	class AI;

	class ManagerModule final : public EnergyConsumer
	{
	public:
		static void setAI(std::shared_ptr<AI> ai) noexcept;

	private:
		inline bool canAddDevice(std::shared_ptr<ObservationCenter> newDevice) const noexcept
		{ 
			return (static_cast<size_t>(m_maxDevices) > m_devices.size() && 
				std::find(std::begin(m_devices), std::end(m_devices), newDevice) == std::end(m_devices)); 
		}

	public:
		ManagerModule() = delete;

		inline ManagerModule(const unsigned maxDevices, const unsigned powerUsage, const unsigned radius, const unsigned priority) noexcept :
			EnergyConsumer(powerUsage, radius, priority),
			m_maxDevices(maxDevices),
			m_devices()
		{ }

		inline QString toString() const override { return (EnergyConsumer::toStringHelper("ManagerModule") + "<b>devices:</b> %1/%2<br/>").arg(m_devices.size()).arg(m_maxDevices); }

		[[nodiscard]]
		inline auto& getDevices() const noexcept { return m_devices; }

		inline bool addDevice(std::shared_ptr<ObservationCenter> newDevice) { return canAddDevice(newDevice) ? m_devices.emplace_back(newDevice), true : false; }

		bool isExplored(const Coord& coord) const noexcept;

	private:
		static std::shared_ptr<AI> sAI;

	private:
		unsigned                                        m_maxDevices;
		std::vector<std::shared_ptr<ObservationCenter>> m_devices;
	};
} // namespace MobileRobots

#endif /* !__MANAGER_MODULE_HPP_INCLUDED__ */