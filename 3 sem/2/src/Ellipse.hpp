#ifndef __ELLIPSE_HPP_INCLUDED__
#define __ELLIPSE_HPP_INCLUDED__

#include <numeric>
#include <algorithm>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif /* !M_PI */

namespace detail
{
	static constexpr double __vectorcall sqrtNewtonRaphson(double x, double cur, double prev) noexcept
	{
		return cur == prev ? 
			cur : 
			sqrtNewtonRaphson(x, .5 * (cur + x / cur), cur);
	}

	inline static double constexpr __vectorcall sqrt(const double x) noexcept
	{
		return (x >= 0. && x < std::numeric_limits<double>::infinity() ? 
			sqrtNewtonRaphson(x, x, 0.l) : 
			std::numeric_limits<double>::quiet_NaN()); 
	}

	inline static double constexpr __vectorcall sqr(const double x) noexcept { return (x * x); }
} // anonymous namespace

class Ellipse
{
public:
	Ellipse() = delete;

	inline constexpr Ellipse(const double a, const double b) noexcept :
		m_a(a),
		m_b(b)
	{ }

	[[nodiscard]]
	inline constexpr double getA() const noexcept { return m_a; }

	[[nodiscard]]
	inline constexpr double getB() const noexcept { return m_b; }

	[[nodiscard]]
	inline constexpr double getC() const noexcept { return detail::sqrt(detail::sqr(std::max(m_a, m_b)) - detail::sqr(std::min(m_a, m_b))); }

	[[nodiscard]]
	inline constexpr double getEccentricity() const noexcept { return getC() / std::max(m_a, m_b); }

	[[nodiscard]]
	inline constexpr double getLength() const noexcept
	{
		auto t = 3 * detail::sqr((m_a - m_b) / (m_a + m_b));

		return M_PI * (m_a + m_b) * (1 + t / (10 + detail::sqrt(4 - t)));
	}

	[[nodiscard]]
	inline constexpr double getArea() const noexcept { return M_PI * m_a * m_b; }

	[[nodiscard]]
	inline constexpr auto getMinMaxDistToFoci(const double x, const double y) const noexcept 
	{
		if (auto c = getC(); m_a > m_b)
			return std::minmax(detail::sqrt(detail::sqr(x - c) + detail::sqr(y)), detail::sqrt(detail::sqr(x + c) + detail::sqr(y)));
		else
			return std::minmax(detail::sqrt(detail::sqr(x) + detail::sqr(y - c)), detail::sqrt(detail::sqr(x) + detail::sqr(y + c)));
	}

	[[nodiscard]]
	inline constexpr std::pair<double, double> getCartesianY(const double x) const noexcept 
	{ 
		auto sqrtY{ detail::sqrt(detail::sqr(m_b) * detail::sqrt((1 - detail::sqr(x) / detail::sqr(m_a)))) };
		
		return { -sqrtY, +sqrtY };
	}

	inline void setA(const double a) noexcept { m_a = a; }

	inline void setB(const double b) noexcept { m_b = b; }

	__declspec(property(get = getA, put = setA)) double a;
	__declspec(property(get = getB, put = setB)) double b;
	__declspec(property(get = getC))             double c;
	__declspec(property(get = getEccentricity))  double e;
	__declspec(property(get = getLength))        double length;
	__declspec(property(get = getArea))          double area;

private:
	double m_a;
	double m_b;
};

#endif /* !__ELLIPSE_HPP_INCLUDED__ */