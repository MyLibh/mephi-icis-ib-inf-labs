#pragma once

#include "util.hpp"
#include "probe_spec.hpp"

template<typename _Key, typename _T>
class probe_hash_table
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

private:
	inline static constexpr std::size_t K = 1U;

private:
	[[nodiscard]]
	__forceinline std::size_t calc_hash(_Key key) const noexcept
	{
		return (std::hash<_Key>()(*key) % m_table.size());
	}

	[[nodiscard]]
	__forceinline std::size_t get_next(const std::size_t hash_idx) const noexcept
	{
		return ((hash_idx +  probe_hash_table::K) % m_table.size());
	}

public:
	probe_hash_table(const std::size_t capacity);

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

	bool add(_Key key, _T data);

	bool remove(_Key key) noexcept;

	[[nodiscard]]
	auto get(_Key key) const noexcept;

	void print(std::ostream& ostr = std::cout) const;

	friend std::ostream& operator<<(std::ostream& ostr, const probe_hash_table<_Key, _T>& table);

private:
	std::size_t						 m_length;
	std::vector<std::pair<_Key, _T>> m_table;
};

template<typename _Key, typename _T>
inline probe_hash_table<_Key, _T>::probe_hash_table(const std::size_t capacity) :
	m_length{ 0U },
	m_table(capacity)
{ }

template<typename _Key, typename _T>
inline bool probe_hash_table<_Key, _T>::add(_Key key, _T data)
{
	if (m_length == m_table.size())
		return false;

	auto hash_idx{ calc_hash(key) };
	auto def = _Key{};
	while (m_table.at(hash_idx) && m_table.at(hash_idx)->first != def)
		hash_idx = get_next(hash_idx);

	m_length++;
	m_table.at(hash_idx) = std::make_pair(_Key{}, _T{});

	return true;
}

template<typename _Key, typename _T>
inline bool probe_hash_table<_Key, _T>::remove(_Key key) noexcept
{
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx))
	{
		if (m_table.at(hash_idx).first == key)
		{
			m_length--;

			m_table.at(hash_idx) = std::make_pair(_Key{}, _T{});

			return true;
		}

		hash_idx = get_next(hash_idx);
	}

	return false;
}

template<typename _Key, typename _T>
[[nodiscard]]
inline auto probe_hash_table<_Key, _T>::get(_Key key) const noexcept
{
	auto hash_idx{ calc_hash(key) };
	while (m_table.at(hash_idx))
	{
		if (m_table.at(hash_idx)->first == key)
			return m_table.at(hash_idx)->second;

		hash_idx = get_next(hash_idx);
	}

	return _T{ };
}

template<typename _Key, typename _T>
inline void probe_hash_table<_Key, _T>::print(std::ostream& ostr) const
{
	for (auto&& cell : m_table)
		ostr << cell.data << std::endl;
}

template<typename _Key, typename _T>
std::ostream& operator<<(std::ostream& ostr, const probe_hash_table<_Key, _T>& table)
{
	table.print(ostr);

	return ostr;
}