#ifndef __COORD_HPP_INCLUDED__
#define __COORD_HPP_INCLUDED__

#include <functional>

namespace MobileRobots
{
	struct Coord
	{
		uint32_t x;
		uint32_t y;
	};

	inline constexpr bool operator<(const Coord& lhs, const Coord& rhs) noexcept
	{
		return (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y)) ? true : false;
	}

	inline constexpr bool operator==(const Coord& lhs, const Coord& rhs) noexcept
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	inline constexpr bool operator!=(const Coord& lhs, const Coord& rhs) noexcept
	{
		return !(lhs == rhs);
	}
} // MobileRobots

namespace std
{
	template<>
	struct hash<MobileRobots::Coord>
	{
		inline constexpr size_t operator()(const MobileRobots::Coord& coord) const
		{
			return (static_cast<size_t>(coord.x) << 32) | coord.y;
		}
	};
}

#endif /* !__COORD_HPP_INCLUDED__ */