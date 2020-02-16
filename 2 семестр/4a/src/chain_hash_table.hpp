#pragma once

#include "list_spec_for_hash_table.hpp"

#include <vector>

template<typename _Key, typename _T>
class chain_hash_table
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

private:
	[[nodiscard]]
	__forceinline std::size_t calc_hash(std::shared_ptr<_Key> key) const noexcept
	{
		return (std::hash<_Key>()(*key) % m_table.size());
	}

public:
	chain_hash_table(const std::size_t capacity) noexcept;
	chain_hash_table(const chain_hash_table&) = delete;
	chain_hash_table(chain_hash_table&&) = delete;

	chain_hash_table& operator=(const chain_hash_table&) = delete;
	chain_hash_table& operator=(chain_hash_table&&) = delete;

	[[nodiscard]]
	__forceinline auto capacity() const noexcept
	{
		return m_table.size();
	}
	
	[[nodiscard]]
	__forceinline auto length() const noexcept
	{
		return m_length;
	}

	[[nodiscard]]
	inline auto number_of_items() const noexcept;

	void add(std::shared_ptr<_Key> key, std::shared_ptr<_T> item);

	bool remove(std::shared_ptr<_Key> key) noexcept;

	[[nodiscard]]
	inline auto get(std::shared_ptr<_Key> key) noexcept;

	void print(std::ostream& ostr = std::cout);

	friend std::ostream& operator<<(std::ostream& ostr, const chain_hash_table<_Key, _T>& table);

private:
	std::size_t									  m_length;
	std::vector<linked_list<std::pair<_Key, _T>>> m_table;
};

template<typename _Key, typename _T>
chain_hash_table<_Key, _T>::chain_hash_table(const std::size_t capacity) noexcept :
	m_length{ },
	m_table(capacity)
{ }

template<typename _Key, typename _T>
[[nodiscard]]
inline auto chain_hash_table<_Key, _T>::number_of_items() const noexcept
{
	std::size_t res{};
	for (auto&& list : m_table)
		res += list.length();

	return res;
}

template<typename _Key, typename _T>
void chain_hash_table<_Key, _T>::add(std::shared_ptr<_Key> key, std::shared_ptr<_T> item)
{
	auto hash = calc_hash(key);

	m_table.at(hash).add(key, item);

	m_length++;
}

template<typename _Key, typename _T>
bool chain_hash_table<_Key, _T>::remove(std::shared_ptr<_Key> key) noexcept
{
	auto hash = calc_hash(key);

	auto res = m_table.at(hash).remove(key);
	if (res)
		m_length--;

	return res;
}

template<typename _Key, typename _T>
[[nodiscard]]
inline auto chain_hash_table<_Key, _T>::get(std::shared_ptr<_Key> key) noexcept
{
	auto hash = calc_hash(key);

	return m_table.at(hash).get(key);
}

template<typename _Key, typename _T>
void chain_hash_table<_Key, _T>::print(std::ostream& ostr /* = std::cout */)
{
	for (auto&& list : m_table)
		if (list.length())
			ostr << list << std::endl;
}

template<typename _Key, typename _T>
std::ostream& operator<<(std::ostream& ostr, const chain_hash_table<_Key, _T>& table)
{
	table.print(ostr);

	return ostr;
}