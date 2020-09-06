#ifndef __MODULE_HPP_INCLUDED__
#define __MODULE_HPP_INCLUDED__

#include <QString>

namespace MobileRobots
{
	class Module
	{
	protected:
		inline virtual QString toStringHelper(const QString& type) const { return QString("<b>type:</b> %1<br/><b>isActive:</b> %2<br/><b>priority:</b> %3<br/>").arg(type).arg(m_isActive).arg(m_priority); };

	public:
		inline constexpr Module(const bool isActive, const unsigned priority) noexcept :
			m_isActive(isActive),
			m_priority(priority)
		{ }

		inline virtual ~Module() noexcept = default;

		[[nodiscard]]
		inline constexpr auto getPriority() const noexcept { return m_priority; }

		inline constexpr bool isActive() const noexcept { return m_isActive; }

		inline void turnOn() noexcept { m_isActive = true; }

		inline void turnOff() noexcept { m_isActive = false; }

		virtual QString toString() const = 0;

	protected:
		bool     m_isActive;
		unsigned m_priority;
	};
} // namespace MobileRobots

#endif /* !__MODULE_HPP_INCLUDED__ */