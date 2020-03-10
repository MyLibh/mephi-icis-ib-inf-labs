#pragma once

#include "probe_hash_table.hpp"
#include "item.hpp"

#include <algorithm>
#include <fstream>

template<typename _Key, typename _Key1, typename _Key2>
class probe_hash_table<_Key, item<_Key1, _Key2>>
{
	using item_t = item<_Key1, _Key2>;
	using data_t = std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>;

private:
	inline static constexpr std::size_t K = 1U;

private:
	[[nodiscard]]
	__forceinline std::size_t calc_hash(const _Key key) const noexcept
	{
		return (std::hash<_Key>()(key) % m_table.size());
	}

	[[nodiscard]]
	__forceinline std::size_t get_next(const std::size_t hash_idx) const noexcept
	{
		return ((hash_idx + probe_hash_table<_Key, item_t>::K) % m_table.size());
	}

public:
	probe_hash_table(const std::size_t capacity, std::size_t item_t::* ptr_to_index);

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

	__forceinline void resize(const std::size_t capacity)
	{
		m_table.clear();

		m_table.resize(capacity);
	}

	bool add(const _Key key, ptr_to_item_t<_Key1, _Key2> item, const bool to_index = true);

	bool remove(const _Key key, const std::size_t index) noexcept;

	std::size_t remove(const _Key key) noexcept;

	[[nodiscard]]
	auto get(const _Key key, const std::size_t index) const noexcept;

	[[nodiscard]]
	auto get(const _Key key) const;

	void print(std::ostream& ostr = std::cout) const;

	void save(std::fstream& ofstr) const;

private:
	std::size_t			  m_length;
	std::vector<data_t>   m_table;
	std::size_t item_t::* m_ptr_to_index;
};

template<typename _Key, typename _Key1, typename _Key2>
inline probe_hash_table<_Key, item<_Key1, _Key2>>::probe_hash_table(const std::size_t capacity, std::size_t item_t::* ptr_to_index) :
	m_length{ 0U }, 
	m_table(capacity),
	m_ptr_to_index{ ptr_to_index }
{ }

template<typename _Key, typename _Key1, typename _Key2>
inline bool probe_hash_table<_Key, item<_Key1, _Key2>>::add(const _Key key, ptr_to_item_t<_Key1, _Key2> item, const bool to_index /* = true */)
{
	if (m_length == m_table.size())
		return false;

	auto hash_idx{ calc_hash(key) };
	std::size_t index{};
	auto def = _Key{};
	while (m_table.at(hash_idx).second && m_table.at(hash_idx).first != def)
	{
		if (m_table.at(hash_idx).first == key)
			index = std::max(index, *m_table.at(hash_idx).second.*m_ptr_to_index);

		hash_idx = get_next(hash_idx);
	}

	m_length++;
	m_table.at(hash_idx) = std::make_pair(key, item);

	if(to_index)
		*m_table.at(hash_idx).second.*m_ptr_to_index = ++index;

	 return true;
}

template<typename _Key, typename _Key1, typename _Key2>
inline bool probe_hash_table<_Key, item<_Key1, _Key2>>::remove(const _Key key, const std::size_t index) noexcept
{
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx).second)
	{
		if (m_table.at(hash_idx).first == key && *m_table.at(hash_idx).second.*m_ptr_to_index == index)
		{
			m_length--;

			m_table.at(hash_idx) = std::make_pair(_Key{}, std::shared_ptr<item_t>());

			return true;
		}

		hash_idx = get_next(hash_idx);
	}

	return false;
}

template<typename _Key, typename _Key1, typename _Key2>
inline std::size_t probe_hash_table<_Key, item<_Key1, _Key2>>::remove(const _Key key) noexcept
{
	auto old_len{ m_length };
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx).second)
	{
		if (m_table.at(hash_idx).first == key)
		{
			m_length--;

			m_table.at(hash_idx) = std::make_pair(_Key{}, std::shared_ptr<item_t>());
		}

		hash_idx = get_next(hash_idx);
	}

	return (old_len - m_length);
}

template<typename _Key, typename _Key1, typename _Key2>
[[nodiscard]]
inline auto probe_hash_table<_Key, item<_Key1, _Key2>>::get(const _Key key, const std::size_t index) const noexcept
{
	auto hash_idx{ calc_hash(key) };
	auto def = _Key{};
	while (m_table.at(hash_idx).second)
	{
		if (m_table.at(hash_idx).first == key && *m_table.at(hash_idx).second.*m_ptr_to_index == index)
			return m_table.at(hash_idx).second;

		hash_idx = get_next(hash_idx);
	}

	return std::shared_ptr<item_t>();
}

template<typename _Key, typename _Key1, typename _Key2>
[[nodiscard]]
inline auto probe_hash_table<_Key, item<_Key1, _Key2>>::get(const _Key key) const
{
	linked_list<item_t> res;
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx).second)
	{
		if (m_table.at(hash_idx).first == key)
			res.add(*m_table.at(hash_idx).second);

		hash_idx = get_next(hash_idx);
	}

	return std::move(res);
}

template<typename _Key, typename _Key1, typename _Key2>
inline void probe_hash_table<_Key, item<_Key1, _Key2>>::print(std::ostream& ostr /* = std::cout */) const
{
	for (auto&& cell : m_table)
		if (auto def = _Key{}; cell.first != def && cell.second)
			ostr << *cell.second << std::endl;
}

template<typename _Key, typename _Key1, typename _Key2>
inline void probe_hash_table<_Key, item<_Key1, _Key2>>::save(std::fstream& fstr) const
{
	auto size{ m_table.size() };
	fstr.write(reinterpret_cast<const char*>(&size), sizeof(size));
	fstr.write(reinterpret_cast<const char*>(&m_length), sizeof(m_length));

	for (auto&& [key, ptr] : m_table)
		if (auto def = _Key{}; ptr && key != def)
			fstr.write(reinterpret_cast<const char*>(&*ptr), sizeof(*ptr));

	auto free_sells{ m_table.size() - m_length };
	item_t empty;
	for (size_t i{}; i < free_sells; ++i)
		fstr.write(reinterpret_cast<const char*>(&empty), sizeof(empty));
}