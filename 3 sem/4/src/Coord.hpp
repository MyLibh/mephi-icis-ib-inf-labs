/**
 * @file Coord.hpp
 *
 * @ingroup Utility
 *
 * @brief Implementation of point
 *
 * @note Assumes that each coordinate is in range [0; 2^32)
 *
 * @author mylibh
*/

#ifndef __COORD_HPP_INCLUDED__
#define __COORD_HPP_INCLUDED__

#include "Math.hpp"

#include <functional>

namespace detail
{
	inline constexpr auto maxSubMin(const uint32_t a, const uint32_t b) noexcept { return std::max(a, b) - std::min(a, b); }
} // namespace detail

namespace MobileRobots
{
	struct Coord
	{
		uint32_t x;
		uint32_t y;

		/**
		 * @brief Default constructor
		*/
		inline constexpr Coord() noexcept :
			x{},
			y{}
		{ }

		/**
		 * @brief Constructs point with coords { x, y }
		 * 
		 * @param x 
		 * @param y 
		*/
		inline constexpr Coord(const uint32_t x, const uint32_t y) noexcept :
			x(x),
			y(y)
		{ }

		/**
		 * @brief Calculates distance to another point
		 * 
		 * @param other Point, distance to is calculated
		 * 
		 * @return distance to 'other' 
		*/
		inline constexpr auto distanceTo(const Coord& other) const noexcept { return Math::hypot(detail::maxSubMin(x, other.x), detail::maxSubMin(y, other.y)); }
	};

	/**
	 * @brief Compares tow points
	 * 
	 * @param lhs First instance
	 * @param rhs Second instance
	 * 
	 * @return True if 'lhs' is to the left or to the bottom, False overwise
	*/
	inline constexpr bool operator<(const Coord& lhs, const Coord& rhs) noexcept
	{
		return (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y)) ? true : false;
	}

	/**
	 * @brief Compares two points
	 * 
	 * @param lhs First instance
	 * @param rhs Second instance
	 * 
	 * @return True if they are the same, False overwise
	*/
	inline constexpr bool operator==(const Coord& lhs, const Coord& rhs) noexcept
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	/**
	 * @brief Compares two points
	 * 
	 * @param lhs First instance
	 * @param rhs Second instance
	 * 
	 * @return True if they are not the same, False overwise
	*/
	inline constexpr bool operator!=(const Coord& lhs, const Coord& rhs) noexcept
	{
		return !(lhs == rhs);
	}
} // namespace MobileRobots

namespace std
{
	/**
	 * @brief Specialization of std::hash for MobileRobots::Coord
	*/
	template<>
	struct hash<MobileRobots::Coord>
	{
		inline constexpr size_t operator()(const MobileRobots::Coord& coord) const noexcept
		{
			return (static_cast<size_t>(coord.x) << 32) | static_cast<size_t>(coord.y); //-V112
		}
	};
} // namespace std

#endif /* !__COORD_HPP_INCLUDED__ */