#pragma once

#include "probe_hash_table.hpp"
#include "item.hpp"

#include <algorithm>

template<typename _Key, typename _Key1, typename _Key2, typename _T>
class probe_hash_table<_Key, item<_Key1, _Key2, _T>>
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

	using item_t = item<_Key1, _Key2, _T>;

private:
	inline static constexpr std::size_t K = 1U;

private:
	[[nodiscard]]
	__forceinline std::size_t calc_hash(std::shared_ptr<_Key> key) const noexcept
	{
		return (std::hash<_Key>()(*key) % m_table.size());
	}

	[[nodiscard]]
	__forceinline std::size_t get_next(const std::size_t hash_idx) const noexcept
	{
		return ((hash_idx + probe_hash_table<_Key, item_t>::K) % m_table.size());
	}

public:
	probe_hash_table(const std::size_t capacity, std::shared_ptr<std::size_t> item_t::*);

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

	bool add(std::shared_ptr<_Key> key, item_t data);

	bool remove(std::shared_ptr<_Key> key, const std::size_t index) noexcept;

	void remove(std::shared_ptr<_Key> key) noexcept;

	[[nodiscard]]
	auto get(std::shared_ptr<_Key> key, const std::size_t index) const noexcept;

	[[nodiscard]]
	auto get(std::shared_ptr<_Key> key) const;

	void print(std::ostream& ostr = std::cout) const;

private:
	std::size_t											  m_length;
	std::vector<std::shared_ptr<std::pair<_Key, item_t>>> m_table;
	std::shared_ptr<std::size_t> item_t::*				  m_ptr_to_index;
};

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline probe_hash_table<_Key, item<_Key1, _Key2, _T>>::probe_hash_table(const std::size_t capacity, std::shared_ptr<std::size_t> item_t::* ptr_to_index) :
	m_length{ 0U }, 
	m_table(capacity),
	m_ptr_to_index{ ptr_to_index }
{ }

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline bool probe_hash_table<_Key, item<_Key1, _Key2, _T>>::add(std::shared_ptr<_Key> key, item_t data)
{
	if (m_length == m_table.size())
		return false;

	auto hash_idx{ calc_hash(key) };
	std::size_t index{};
	auto def = _Key{};
	while (m_table.at(hash_idx) && m_table.at(hash_idx)->first != def)
	{
		if (m_table.at(hash_idx)->first == *key)
			index = std::max(index, *(m_table.at(hash_idx)->second.*m_ptr_to_index));

		hash_idx = get_next(hash_idx);
	}

	m_length++;
	m_table.at(hash_idx) = std::make_shared<std::pair<_Key, item_t>>(*key, data);

	*(m_table.at(hash_idx)->second.*m_ptr_to_index) = ++index;

	 return true;
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline bool probe_hash_table<_Key, item<_Key1, _Key2, _T>>::remove(std::shared_ptr<_Key> key, const std::size_t index) noexcept
{
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx))
	{
		if (m_table.at(hash_idx)->first == *key && *(m_table.at(hash_idx)->second.*m_ptr_to_index) == index)
		{
			m_length--;

			*m_table.at(hash_idx) = std::make_pair(_Key{}, item_t{});

			return true;
		}

		hash_idx = get_next(hash_idx);
	}

	return false;
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline void probe_hash_table<_Key, item<_Key1, _Key2, _T>>::remove(std::shared_ptr<_Key> key) noexcept
{
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx))
	{
		if (m_table.at(hash_idx)->first == *key)
		{
			m_length--;

			*m_table.at(hash_idx) = std::make_pair(_Key{}, item_t{});
		}

		hash_idx = get_next(hash_idx);
	}
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
[[nodiscard]]
inline auto probe_hash_table<_Key, item<_Key1, _Key2, _T>>::get(std::shared_ptr<_Key> key, const std::size_t index) const noexcept
{
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx))
	{
		if (m_table.at(hash_idx)->first == *key && *(m_table.at(hash_idx)->second.*m_ptr_to_index) == index)
			return m_table.at(hash_idx)->second;

		hash_idx = get_next(hash_idx);
	}

	return item_t{ };
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
[[nodiscard]]
inline auto probe_hash_table<_Key, item<_Key1, _Key2, _T>>::get(std::shared_ptr<_Key> key) const
{
	linked_list<item_t> res;
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx))
	{
		if (m_table.at(hash_idx)->first == *key)
			res.add(m_table.at(hash_idx)->second);

		hash_idx = get_next(hash_idx);
	}

	return std::move(res);
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline void probe_hash_table<_Key, item<_Key1, _Key2, _T>>::print(std::ostream& ostr /* = std::cout */) const
{
	for (auto&& cell : m_table)
		if (auto def = _Key{}; cell && cell->first != def)
			ostr << cell->second << std::endl;
}