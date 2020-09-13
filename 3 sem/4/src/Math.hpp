#ifndef __MATH_HPP_INCLUDED__
#define __MATH_HPP_INCLUDED__

#include <type_traits>
#include <numeric>

namespace Math
{
	namespace Constants
	{
		template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
		constexpr _Ty pi = _Ty(3.1415926535897932385L);
	}

	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	constexpr auto rad2deg(const _Ty rad) noexcept { return rad / Constants::pi<_Ty> * 180; }

	constexpr double __vectorcall sqrtNewtonRaphson(const double x, const double cur, const double prev) noexcept
	{
		return cur == prev ?
			cur :
			sqrtNewtonRaphson(x, .5 * (cur + x / cur), cur);
	}

	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	inline _Ty constexpr sqrt(const _Ty x) noexcept
	{
		return static_cast<_Ty>(x >= 0 && x < std::numeric_limits<double>::infinity() ?
			sqrtNewtonRaphson(static_cast<double>(x), static_cast<double>(x), 0.) :
			std::numeric_limits<double>::quiet_NaN());
	}

	template<typename _Ty, typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
	inline _Ty constexpr sqr(const _Ty x) noexcept { return (x * x); }

	template<typename... _Args, typename = std::enable_if_t<(... && std::is_arithmetic_v<_Args>)>>
	inline constexpr auto hypot(_Args... args) noexcept { return sqrt((sqr(args) + ...)); }
} // namespace Math

#endif /* !__MATH_HPP_INCLUDED__ */