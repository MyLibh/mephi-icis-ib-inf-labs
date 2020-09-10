#include "DiceGroup.hpp"

#include <iostream>

void DiceGroup::resize(const size_t newCapacity)
{
	m_capacity = newCapacity;

	auto ptr = std::move(m_dices);

	m_dices = std::make_unique<uchar_t[]>(m_capacity);

	std::memcpy(m_dices.get(), ptr.get(), m_size * sizeof(uchar_t));
}

DiceGroup& DiceGroup::operator+=(const uchar_t value)
{
	if (!m_capacity)
		resize(1);
	else if (m_size == m_capacity)
		resize(2 * m_capacity);

	m_dices[m_size++] = value;

	return *this;
}

DiceGroup& DiceGroup::operator-=(const uchar_t value) noexcept
{
	for (size_t i{}; i < m_size; ++i)
		if (m_dices[i] == value)
		{
			std::swap(m_dices[i], m_dices[--m_size]);
			--i; // Yeah :)
		}

	return *this;
}

bool DiceGroup::hasConcurrence() const noexcept
{
	bool exists[6]{};
	for (size_t i{}; i < m_size; ++i)
		if (exists[m_dices[i]])
			return true;
		else
			exists[m_dices[i]] = true;

	return false;
}

void DiceGroup::swap(DiceGroup& other) noexcept
{
	using std::swap;

	swap(m_size, other.m_size);
	swap(m_capacity, other.m_capacity);
	swap(m_dices, other.m_dices);
}

bool DiceGroup::areAllTheSame() const noexcept
{
	if (m_size)
	{
		auto val = m_dices[0];
		for (size_t i{ 1 }; i < m_size; ++i)
			if (m_dices[i] != val)
				return false;

		return true;
	}

	return false;
}

std::ostream& operator<<(std::ostream& ostr, const DiceGroup& diceGroup)
{
	for (size_t i{}; i < diceGroup.m_size; ++i)
		ostr << static_cast<int>(diceGroup.m_dices[i]);

	return ostr;
}
