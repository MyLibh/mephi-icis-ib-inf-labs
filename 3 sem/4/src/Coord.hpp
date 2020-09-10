/**
 * @file Coord.hpp
 *
 * @ingroup Utility
 *
 * @brief Provides an implementation of point
 *
 * @note Assumes that each coordinate is in range [0; 2^32)
 *
 * @author mylibh
*/

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

	/**
	 * @brief Compares tow points
	 * 
	 * @param lhs First instance
	 * @param rhs Second instance
	 * 
	 * @return True if 'lhs' is to the left or to the bottom, False overwise
	 * 
	 * @exceptsafe
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
	 * 
	 * @exceptsafe
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
	 * 
	 * @exceptsafe
	*/
	inline constexpr bool operator!=(const Coord& lhs, const Coord& rhs) noexcept
	{
		return !(lhs == rhs);
	}
} // MobileRobots

namespace std
{
	/**
	 * @brief Specialization of std::hash for MobileRobots::Coord
	 * 
	 * @exceptsafe
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