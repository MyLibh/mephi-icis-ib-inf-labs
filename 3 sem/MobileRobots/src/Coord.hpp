#ifndef __COORD_HPP_INCLUDED__
#define __COORD_HPP_INCLUDED__

namespace MobileRobots
{
	struct Coord
	{
		unsigned x;
		unsigned y;
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

#endif /* !__COORD_HPP_INCLUDED__ */