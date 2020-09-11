#ifndef __MAP_OBJECT_HPP_INCLUDED__ 
#define __MAP_OBJECT_HPP_INCLUDED__

#include "Coord.hpp"

#include <memory>

#include <QString>

namespace MobileRobots
{
	class EnvironmentDescriptor;

	class MapObject
	{
	public:
		static void setEnvironmentDescriptor(std::shared_ptr<EnvironmentDescriptor> envDesc) noexcept;

	protected:
		inline virtual QString toStringHelper(const QString& type) const { return std::move(QString("<b>x:</b> %1<br/><b>y:</b> %2<br/><b>type:</b> %3<br/>").arg(m_pos.x).arg(m_pos.y).arg(type)); }

	public:
		MapObject() = delete;

		inline constexpr explicit MapObject(const Coord& coord) noexcept :
			m_pos(coord)
		{ }

		inline virtual ~MapObject() noexcept = default;

		[[nodiscard]]
		inline constexpr auto getX() const noexcept { return m_pos.x; }

		[[nodiscard]]
		inline constexpr auto getY() const noexcept { return m_pos.y; }

		[[nodiscard]]
		inline constexpr auto getPos() const noexcept { return m_pos; }

		virtual QString toString() const = 0;

	protected:
		static std::shared_ptr<EnvironmentDescriptor> sEnvDesc;

	protected:
		Coord m_pos;
	};
} // namespace MobileRobots

#endif /* !__MAP_OBJECT_HPP_INCLUDED__ */
