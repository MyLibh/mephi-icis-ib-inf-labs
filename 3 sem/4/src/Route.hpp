#ifndef __ROUTE_HPP_INCLUDED__
#define __ROUTE_HPP_INCLUDED__

#include "Coord.hpp"

#include <queue>

namespace MobileRobots
{
	class Route final
	{
	public:
		inline Route() noexcept :
			m_to{ -1U, -1U }
		{ }

		inline Route(std::queue<Coord> path) noexcept : // CHECK: contract to non-empty queue
			m_path(std::move(path)),
			m_to{ -1U, -1U }
		{ 
			if (!m_path.empty())
				m_to = m_path.back();
		}

		[[nodiscard]]
		inline Coord getNext() noexcept
		{
			if (!m_path.empty())
			{
				auto&& coord = m_path.front();
				m_path.pop();

				return coord;
			}

			return { -1U, -1U }; // Fly away, bitch
		}

		[[nodiscard]]
		inline auto to() const noexcept { return m_to; }

		inline bool isFinished() const noexcept { return m_path.empty(); }

		inline explicit operator bool() const noexcept { return !isFinished(); }

	private:
		std::queue<Coord> m_path;
		Coord             m_to;
	};
} // namespace MbileRobots

#endif /* !__ROUTE_HPP_INCLUDED__ */