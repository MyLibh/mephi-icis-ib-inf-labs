#ifndef __ENVIRONMENT_DESCRIPTOR_HPP_INCLUDED__
#define __ENVIRONMENT_DESCRIPTOR_HPP_INCLUDED__

#include "Barrier.hpp"
#include "InterestingObject.hpp"
#include "RobotCommander.hpp"

#include <vector>
#include <type_traits>

namespace MobileRobots
{
	class EnvironmentDescriptor final : public std::enable_shared_from_this<EnvironmentDescriptor>
	{
	public:
		inline EnvironmentDescriptor() noexcept = default;

		inline EnvironmentDescriptor(const unsigned width, const unsigned height) noexcept :
			m_width(width),
			m_height(height),
			m_objects()
		{ }

		inline ~EnvironmentDescriptor() noexcept = default;

		[[nodiscard]]
		inline constexpr auto getWidth() const noexcept { return m_width; }

		[[nodiscard]]
		inline constexpr auto getHeight() const noexcept { return m_height; }

		[[nodiscard]]
		inline const auto& getObjects() const noexcept { return m_objects; }

		[[nodiscard]]
		const std::shared_ptr<MapObject> getObject(const Coord& coord) const noexcept;

		inline constexpr void setWidth(const unsigned width) noexcept { m_width = width; }

		inline constexpr void setHeight(const unsigned height) noexcept { m_height = height; }

		template<typename _ObjectType>
		inline void addObject(const Coord& coord);

		template<typename _ObjectType>
		inline void addObject(const std::string_view description, const unsigned powerUsage, const unsigned maxSlots, const unsigned price, const std::vector<std::shared_ptr<Module>>& modules, const Coord& coord);

	private:
		unsigned                                m_width;
		unsigned                                m_height;
		std::vector<std::shared_ptr<MapObject>> m_objects;
	};

	template<typename _ObjectType>
	inline void EnvironmentDescriptor::addObject(const Coord& coord)
	{
		if constexpr (std::is_same_v<_ObjectType, Barrier>)
			m_objects.push_back(std::make_shared<Barrier>(coord));
		else if constexpr (std::is_same_v<_ObjectType, InterestingObject>)
			m_objects.push_back(std::make_shared<InterestingObject>(coord));
	}
	
	template<typename _ObjectType>
	inline void EnvironmentDescriptor::addObject(const std::string_view description, const unsigned powerUsage, const unsigned maxSlots, const unsigned price, const std::vector<std::shared_ptr<Module>>& modules, const Coord& coord)
	{
		if constexpr (std::is_same_v<_ObjectType, RobotScout>)
			m_objects.push_back(std::make_shared<RobotScout>(description, powerUsage, maxSlots, price, modules, coord));
		else if constexpr (std::is_same_v<_ObjectType, RobotCommander>)
			m_objects.push_back(std::make_shared<RobotCommander>(description, powerUsage, maxSlots, price, modules, coord));
		else if constexpr (std::is_same_v<_ObjectType, ObservationCenter>)
			m_objects.push_back(std::make_shared<ObservationCenter>(description, powerUsage, maxSlots, price, modules, coord));
		else if constexpr (std::is_same_v<_ObjectType, CommandCenter>)
			m_objects.push_back(std::make_shared<CommandCenter>(description, powerUsage, maxSlots, price, modules, coord));
	}
} // namespace MobileRobots

#endif /* !__ENVIRONMENT_DESCRIPTOR_HPP_INCLUDED__ */