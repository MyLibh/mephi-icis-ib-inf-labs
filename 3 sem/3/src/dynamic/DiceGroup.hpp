#ifndef __DICE_GROUP_HPP_INCLUDED__
#define __DICE_GROUP_HPP_INCLUDED__

#include <memory>
#include <stdexcept>

#include "../../include/generator.hpp"

using uchar_t = unsigned char;

class DiceGroup
{
private:
	void resize(const size_t newCapacity);

public:
	inline constexpr DiceGroup() noexcept :
		m_size{},
		m_capacity{},
		m_dices{}
	{ }

	inline DiceGroup(const DiceGroup& other) :
		m_size(other.m_size),
		m_capacity(m_size),
		m_dices(std::make_unique<uchar_t[]>(m_capacity))
	{ std::memcpy(m_dices.get(), other.m_dices.get(), other.m_size * sizeof(uchar_t)); }

	inline DiceGroup(DiceGroup&& other) noexcept : 
		DiceGroup()
	{ swap(other); }

	inline DiceGroup(const size_t size) :
		m_size(size),
		m_capacity(m_size),
		m_dices(std::make_unique<uchar_t[]>(m_capacity))
	{ roll(); }

	inline DiceGroup(const size_t size, std::unique_ptr<uchar_t[]> dices) :
		m_size(size),
		m_capacity(m_size),
		m_dices(std::move(dices))
	{ }

	inline DiceGroup& operator=(const DiceGroup& other)
	{
		if (this != &other)
		{

			m_size = other.m_size;
			m_capacity = m_size;

			m_dices.reset(new uchar_t[m_size]);

			std::memcpy(m_dices.get(), other.m_dices.get(), other.m_size * sizeof(uchar_t));
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
	size_t                     m_size;
	size_t                     m_capacity;
	std::unique_ptr<uchar_t[]> m_dices;
};

#endif /* !__DICE_GROUP_HPP_INCLUDED__ */