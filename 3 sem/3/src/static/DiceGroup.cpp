#include "DiceGroup.hpp"

#include <iostream>

DiceGroup& DiceGroup::operator+=(const uchar_t value)
{
	if (m_size == m_dices.size())
		throw std::length_error("DiceGroup oversize");

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
