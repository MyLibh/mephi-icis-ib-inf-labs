#pragma once

#include "chain_hash_table.hpp"
#include "list_spec.hpp"

template<typename _Key, typename _Key1, typename _Key2>
class chain_hash_table<_Key, item<_Key1, _Key2>>
{
	using item_t = item<_Key1, _Key2>;
	using data_t = std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>;

private:
	[[nodiscard]]
	__forceinline std::size_t calc_hash(const _Key key) const noexcept
	{
		return (std::hash<_Key>()(key) % m_table.size());
	}

public:
	chain_hash_table(const std::size_t capacity, std::size_t item_t::* ptr_to_index);

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

	__forceinline void resize(const std::size_t capacity, std::size_t item_t::* ptr_to_index)
	{
		m_table.clear();

		m_table.resize(capacity, ptr_to_index);
	}

	void add(const _Key key, ptr_to_item_t<_Key1, _Key2> item, const bool to_index = true);

	bool remove(const _Key key, const std::size_t index) noexcept;

	std::size_t remove(const _Key key) noexcept;

	[[nodiscard]]
	inline auto get(const _Key key, const std::size_t index) const noexcept;

	[[nodiscard]]
	inline auto get(const _Key key) const;

	void print(std::ostream& ostr = std::cout) const;

private:
	std::size_t						 m_length;
	std::vector<linked_list<data_t>> m_table;
	std::size_t item_t::*			 m_ptr_to_index;
};

template<typename _Key, typename _Key1, typename _Key2>
inline chain_hash_table<_Key, item<_Key1, _Key2>>::chain_hash_table(const std::size_t capacity, std::size_t item_t::* ptr_to_index) :
	m_length{ 0U },
	m_table(capacity, ptr_to_index)
{ }

template<typename _Key, typename _Key1, typename _Key2>
inline void chain_hash_table<_Key, item<_Key1, _Key2>>::add(const _Key key, ptr_to_item_t<_Key1, _Key2> item, const bool to_index /* = true */)
{
	auto hash = calc_hash(key);

	m_table.at(hash).add(key, item, to_index);

	m_length++;
}

template<typename _Key, typename _Key1, typename _Key2>
inline bool chain_hash_table<_Key, item<_Key1, _Key2>>::remove(const _Key key, const std::size_t index) noexcept
{
	auto hash = calc_hash(key);

	auto res = m_table.at(hash).remove(key, index);
	if (res)
		m_length--;

	return res;
}

template<typename _Key, typename _Key1, typename _Key2>
inline std::size_t chain_hash_table<_Key, item<_Key1, _Key2>>::remove(const _Key key) noexcept
{
	auto num_del = m_table.at(calc_hash(key)).remove(key);

	m_length -= num_del;

	return num_del;
}

template<typename _Key, typename _Key1, typename _Key2>
inline auto chain_hash_table<_Key, item<_Key1, _Key2>>::get(const _Key key, const std::size_t index) const noexcept
{
	auto hash = calc_hash(key);

	return m_table.at(hash).get(key, index);
}

template<typename _Key, typename _Key1, typename _Key2>
inline auto chain_hash_table<_Key, item<_Key1, _Key2>>::get(const _Key key) const
{
	auto hash = calc_hash(key);

	return m_table.at(hash).get(key);
}

template<typename _Key, typename _Key1, typename _Key2>
inline void chain_hash_table<_Key, item<_Key1, _Key2>>::print(std::ostream& ostr /* = std::cout */) const
{
	for (auto&& list : m_table)
		if (list.length())
			ostr << list << std::endl;
}