#ifndef __ENERGY_CONSUMER_HPP_INCLUDED__
#define __ENERGY_CONSUMER_HPP_INCLUDED__

#include "Module.hpp"

namespace MobileRobots
{
	class EnergyConsumer : public Module
	{
	protected:
		inline virtual QString toStringHelper(const QString& type) const override { return (Module::toStringHelper(type) + "<b>powerUsage:</b> %1<br/><b>r:</b> %2<br/>").arg(m_powerUsage).arg(m_r); }

	public:
		inline constexpr EnergyConsumer(const unsigned powerUsage, const unsigned radius, const unsigned priority, const bool isActive = true) noexcept :
			Module(isActive, priority),
			m_powerUsage(powerUsage),
			m_r(radius)
		{ }

		inline virtual ~EnergyConsumer() noexcept override = default;

		inline virtual QString toString() const override { return EnergyConsumer::toStringHelper("EnergyConsumer"); }

		[[nodiscard]]
		inline constexpr auto getPowerUsage() const noexcept { return m_powerUsage; }

		[[nodiscard]]
		inline constexpr auto getRadius() const noexcept { return m_r; }

	protected:
		unsigned m_powerUsage;
		unsigned m_r;
	};
} // namespace MobileRobots

#endif /* !__ENERGY_CONSUMER_HPP_INCLUDED__ */