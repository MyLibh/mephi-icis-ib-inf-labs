#pragma once

#include "chain_hash_table.hpp"
#include "list_spec.hpp"

template<typename _Key, typename _Key1, typename _Key2, typename _T>
class chain_hash_table<_Key, item<_Key1, _Key2, _T>>
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

	using item_t = item<_Key1, _Key2, _T>;

private:
	[[nodiscard]]
	__forceinline std::size_t calc_hash(std::shared_ptr<_Key> key) const noexcept
	{
		return (std::hash<_Key>()(*key) % m_table.size());
	}

public:
	chain_hash_table(const std::size_t capacity, std::shared_ptr<std::size_t> item_t::* ptr_to_index);

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

	void add(std::shared_ptr<_Key> key, item_t item);

	bool remove(std::shared_ptr<_Key> key, const std::size_t index) noexcept;

	void remove(std::shared_ptr<_Key> key) noexcept;

	[[nodiscard]]
	inline auto get(std::shared_ptr<_Key> key, const std::size_t index) const noexcept;

	[[nodiscard]]
	inline auto get(std::shared_ptr<_Key> key) const;

	void print(std::ostream& ostr = std::cout) const;

private:
	std::size_t												 m_length;
	std::vector<linked_list<data_t<_Key, _Key1, _Key2, _T>>> m_table;
};

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline chain_hash_table<_Key, item<_Key1, _Key2, _T>>::chain_hash_table(const std::size_t capacity, std::shared_ptr<std::size_t> item_t::* ptr_to_index) :
	m_length{ 0U },
	m_table(capacity, ptr_to_index)
{ }

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline void chain_hash_table<_Key, item<_Key1, _Key2, _T>>::add(std::shared_ptr<_Key> key, item_t item)
{
	auto hash = calc_hash(key);

	m_table.at(hash).add(key, item);

	m_length++;
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline bool chain_hash_table<_Key, item<_Key1, _Key2, _T>>::remove(std::shared_ptr<_Key> key, const std::size_t index) noexcept
{
	auto hash = calc_hash(key);

	auto res = m_table.at(hash).remove(key, index);
	if (res)
		m_length--;

	return res;
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline void chain_hash_table<_Key, item<_Key1, _Key2, _T>>::remove(std::shared_ptr<_Key> key) noexcept
{
	auto hash = calc_hash(key);

	m_table.at(hash).remove(key);
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline auto chain_hash_table<_Key, item<_Key1, _Key2, _T>>::get(std::shared_ptr<_Key> key, const std::size_t index) const noexcept
{
	auto hash = calc_hash(key);

	return m_table.at(hash).get(key, index);
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline auto chain_hash_table<_Key, item<_Key1, _Key2, _T>>::get(std::shared_ptr<_Key> key) const 
{
	auto hash = calc_hash(key);

	return m_table.at(hash).get(key);
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline void chain_hash_table<_Key, item<_Key1, _Key2, _T>>::print(std::ostream& ostr /* = std::cout */) const
{
	for (auto&& list : m_table)
		if (list.length())
			list.print(ostr), ostr << std::endl;
}