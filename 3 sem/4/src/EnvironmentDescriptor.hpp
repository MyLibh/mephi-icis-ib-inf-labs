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
		std::shared_ptr<MapObject> getObject(const Coord& coord) const noexcept;

		inline constexpr void setWidth(const unsigned width) noexcept { m_width = width; }

		inline constexpr void setHeight(const unsigned height) noexcept { m_height = height; }

		inline constexpr bool isInField(const Coord& coord) const noexcept { return coord.x < m_width && coord.y < m_height; }

		template<typename _ObjectType, typename ..._Args, typename = std::enable_if_t<std::is_base_of_v<MapObject, _ObjectType>>>
		inline void addObject(_Args&&... args) { m_objects.push_back(std::make_shared<_ObjectType>(std::forward<_Args>(args)...)); }

	private:
		unsigned                                m_width;
		unsigned                                m_height;
		std::vector<std::shared_ptr<MapObject>> m_objects;
	};
} // namespace MobileRobots

#endif /* !__ENVIRONMENT_DESCRIPTOR_HPP_INCLUDED__ */