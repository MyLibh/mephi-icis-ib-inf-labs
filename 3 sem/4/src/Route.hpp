#ifndef __ROUTE_HPP_INCLUDED__
#define __ROUTE_HPP_INCLUDED__

#include "Coord.hpp"

#include <queue>

namespace MobileRobots
{
	class Route final
	{
	public:
		Route() = delete;

		inline Route(std::queue<Coord> path) noexcept :
			m_path(std::move(path))
		{ }

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

		inline bool isFinished() const noexcept { return m_path.empty(); }

	private:
		std::queue<Coord> m_path;
	};
} // namespace MbileRobots

#endif /* !__ROUTE_HPP_INCLUDED__ */