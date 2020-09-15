/**
 * @file Math.hpp
 *
 * @ingroup Utility
 *
 * @brief Mathematical functions
 *
 * @author mylibh
*/

#ifndef __MATH_HPP_INCLUDED__
#define __MATH_HPP_INCLUDED__

#include <type_traits>
#include <numeric>

namespace Math
{
	namespace Constants
	{
		/**
		 * @brief Pi constant
		*/
		template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
		constexpr _Ty pi = _Ty(3.1415926535897932385L);
	}

	/**
	 * @brief Converts radians to degrees
	 * 
	 * @tparam _Ty Arithmetic type
	 * 
	 * @param angle Angle to convert
	 * 
	 * @return 'angle' in degrees
	*/
	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	inline constexpr auto rad2deg(const _Ty angle) noexcept
	{
		return angle / Constants::pi<_Ty> * 180;
	}

	/**
	 * @brief Converts degrees to radians
	 *
	 * @tparam _Ty Arithmetic type
	 *
	 * @param angle Angle to convert
	 *
	 * @return 'angle' in radians
	*/
	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	inline constexpr auto deg2rad(const _Ty angle) noexcept
	{
		return angle / 180 * Constants::pi<_Ty>;
	}

	/**
	 * @brief Calculates the absolete value
	 *
	 * @tparam _Ty Arithmetic signed type
	 *
	 * @param x 
	 *
	 * @return Absolete value
	*/
	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty> && std::is_signed_v<_Ty>>>
	inline constexpr auto abs(const _Ty x) noexcept
	{
		return x >= _Ty(0) ? x : -x;
	}

	/**
	 * @brief Helper function to calculate square root in compile-time
	 * 
	 * <a href="https://en.wikipedia.org/wiki/Newton%27s_method">Newton-Raphson method</a>
	*/
	constexpr double __vectorcall sqrtNewtonRaphson(const double x, const double cur, const double prev) noexcept
	{
		return abs(cur - prev) <= std::numeric_limits<double>::epsilon() ?
			cur :
			sqrtNewtonRaphson(x, .5 * (cur + x / cur), cur);
	}

	/**
	 * @brief Calculates the square root
	 * 
	 * @tparam _Ty Arithmetic type
	 * 
	 * @param x The number from which the square root is taken
	 * 
	 * @return Square root of 'x'
	*/
	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	inline _Ty constexpr sqrt(const _Ty x) noexcept
	{
		return static_cast<_Ty>(x >= 0 && x < std::numeric_limits<double>::infinity() ?
			sqrtNewtonRaphson(static_cast<double>(x), static_cast<double>(x), 0.) :
			std::numeric_limits<double>::quiet_NaN());
	}

	/**
	 * @brief Squares the number
	 * 
	 * @tparam _Ty Arithmetic type
	 * 
	 * @param x Number to square
	 * 
	 * @return Squared number
	*/
	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	inline _Ty constexpr sqr(const _Ty x) noexcept
	{
		return (x * x);
	}

	/**
	 * @brief Calculates the hypotenuse for n-dimensional space
	 * 
	 * @tparam ..._Args Arithmetic types
	 * 
	 * @param ...args 
	 * 
	 * @note Evaluates int sqrt(arg1^2 + arg2^2 + ... + argN^2)
	 * 
	 * @return Length of the hypotenuse
	*/
	template<typename... _Args, typename = std::enable_if_t<(... && std::is_arithmetic_v<_Args>)>>
	inline constexpr auto hypot(_Args... args) noexcept
	{
		return sqrt((sqr(args) + ...));
	}
} // namespace Math

#endif /* !__MATH_HPP_INCLUDED__ */