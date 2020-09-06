#ifndef __BARRIER_HPP_INCLUDED__
#define __BARRIER_HPP_INCLUDED__

#include "MapObject.hpp"

namespace MobileRobots
{
	class Barrier final : public MapObject
	{
	public:
		Barrier() = delete;

		inline constexpr Barrier(const Coord& coord) noexcept :
			MapObject(coord)
		{ }

		inline ~Barrier() noexcept override = default;

		inline QString toString() const override { return MapObject::toStringHelper("Barrier"); }
	};
} // namespace MobileRobots

#endif /* !__BARRIER_HPP_INCLUDED__ */