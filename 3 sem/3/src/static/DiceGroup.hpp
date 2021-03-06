#ifndef __DICE_GROUP_HPP_INCLUDED__
#define __DICE_GROUP_HPP_INCLUDED__

#include <memory>
#include <array>
#include <stdexcept>

#include "../../include/generator.hpp"

using uchar_t = unsigned char;

class DiceGroup
{
private:
	inline static constexpr auto MAX_DICES_NUM{ 10U };

private:
	void resize(const size_t newCapacity);

public:
	inline constexpr DiceGroup() noexcept :
		m_size{},
		m_dices{}
	{ }

	inline DiceGroup(const DiceGroup& other) :
		m_size(other.m_size),
		m_dices(other.m_dices)
	{ }

	inline DiceGroup(DiceGroup&& other) noexcept : 
		DiceGroup()
	{ swap(other); }

	inline DiceGroup(const size_t size) :
		m_size(size),
		m_dices()
	{
		if (m_size > m_dices.size())
			throw std::invalid_argument("Wrong size"); 
		
		roll(); 
	}

	inline DiceGroup(const size_t size, std::array<uchar_t, DiceGroup::MAX_DICES_NUM> dices) :
		m_size(size),
		m_dices(std::move(dices))
	{
		if (m_size > m_dices.size())
			throw std::invalid_argument("Wrong size");
	}

	inline DiceGroup& operator=(const DiceGroup& other)
	{
		if (this != &other)
		{
			m_size  = other.m_size;
			m_dices = other.m_dices;
		}

		return *this;
	}
	
	inline DiceGroup& operator=(DiceGroup&& other) noexcept
	{ 
		if (this != &other)
			swap(other); // NOTE: No deallocation

		return *this; 
	} 

	friend std::ostream& operator<<(std::ostream&, const DiceGroup& diceGroup);

	DiceGroup& operator+=(const uchar_t value);

	DiceGroup& operator-=(const uchar_t value) noexcept;

	inline uchar_t operator[](const size_t idx) const 
	{ 
		if (idx > m_size)
			throw std::invalid_argument("Wrong index");

		return m_dices[idx];
	}

	[[nodiscard]]
	inline size_t getSum() const noexcept
	{
		size_t sum{};
		for (size_t i{}; i < m_size; ++i)
			sum += m_dices[i];

			return sum;
	}

	bool hasConcurrence() const noexcept;

	inline void roll() noexcept
	{
		for (size_t i{}; i < m_size; ++i)
			m_dices[i] = util::generator::get(0, 6);
	}

	void swap(DiceGroup& other) noexcept;

	bool areAllTheSame() const noexcept;

private:
	size_t                                        m_size;
	std::array<uchar_t, DiceGroup::MAX_DICES_NUM> m_dices;
};

#endif /* !__DICE_GROUP_HPP_INCLUDED__ */