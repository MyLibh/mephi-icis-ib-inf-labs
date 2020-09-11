#ifndef __OBSERVATION_CENTER_HPP_INCLUDED__
#define __OBSERVATION_CENTER_HPP_INCLUDED__

#include "MapObject.hpp"
#include "Module.hpp"

#include <vector>
#include <map>
#include <string>

namespace MobileRobots
{
	class ObservationCenter : public MapObject
	{
	private:
		void initModules();

	protected:
		virtual QString toStringHelper(const QString& type) const override;

	public:
		ObservationCenter() = delete;
		
		inline ObservationCenter(const std::string_view description, const unsigned powerUsage, const unsigned maxSlots, const unsigned price, const std::vector<std::shared_ptr<Module>>& modules, const Coord& coord) :
			MapObject(coord),
			m_descr(description),
			m_powerUsage(powerUsage),
			m_price(price),
			m_maxSlots(maxSlots),
			m_modules(modules),
			m_owner()
		{ initModules(); }

		inline virtual ~ObservationCenter() noexcept override = default;

		inline virtual QString toString() const override { return ObservationCenter::toStringHelper("ObservationCenter"); }

		[[nodiscard]]
		inline auto getDescription() const noexcept { return m_descr; }

		[[nodiscard]]
		inline constexpr auto getPowerUsage() const noexcept { return m_powerUsage; }

		[[nodiscard]]
		inline constexpr auto getPrice() const noexcept { return m_price; }

		[[nodiscard]]
		inline constexpr auto getMaxSlots() const noexcept { return m_maxSlots; }

		[[nodiscard]]
		inline constexpr auto& getModules() const noexcept { return m_modules; }

		[[nodiscard]]
		inline auto getOwner() const noexcept { return m_owner; }

		[[nodiscard]]
		virtual std::map<Coord, std::shared_ptr<MapObject>> getObjectsAround() const;

		void setOwner(std::shared_ptr<MapObject> owner) noexcept { m_owner = std::dynamic_pointer_cast<ObservationCenter>(owner); }

	protected:
		std::string                          m_descr;
		unsigned                             m_powerUsage;
		unsigned		                     m_maxSlots;
		unsigned                             m_price;
		std::vector<std::shared_ptr<Module>> m_modules;
		std::shared_ptr<ObservationCenter>   m_owner;
	};
} // namespace MobileRobots

#endif /* !__OBSERVATION_CENTER_HPP_INCLUDED__ */