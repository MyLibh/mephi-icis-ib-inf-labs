#ifndef __INTERESTING_OBJECT_HPP_INCLUDED__
#define __INTERESTING_OBJECT_HPP_INCLUDED__

#include "MapObject.hpp"

namespace MobileRobots
{
	class InterestingObject final : public MapObject
	{
	public:
		InterestingObject() = delete;

		inline constexpr InterestingObject(const Coord& coord) noexcept :
			MapObject(coord)
		{ }

		inline ~InterestingObject() noexcept override = default;

		inline QString toString() const override { return MapObject::toStringHelper("InterestingObject"); }
	};
} // namespace MobileRobots

#endif /* !__INTERESTING_OBJECT_HPP_INCLUDED__ */