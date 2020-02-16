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
	__forceinline std::size_t get_index(_Key key, const std::size_t i) const noexcept
	{
		return (calc_hash(key) + i * probe_hash_table::K);
	}

public:
	probe_hash_table(const std::size_t capacity);

	void add(_Key key, _T data);
	void remove(_Key key) noexcept;
	_T get(_Key key) const noexcept;

	void print(std::ostream& ostr = std::cout) const;

	friend std::ostream& operator<<(std::ostream& ostr, const probe_hash_table<_Key, _T>& table);

private:
	std::vector<std::pair<_Key, _T>> m_table;
	std::size_t						 m_length;
};

template<typename _Key, typename _T>
inline probe_hash_table<_Key, _T>::probe_hash_table(const std::size_t capacity) :
	m_table(capacity)
{ }

template<typename _Key, typename _T>
inline void probe_hash_table<_Key, _T>::add(_Key key, _T data)
{
}

template<typename _Key, typename _T>
inline void probe_hash_table<_Key, _T>::remove(_Key key) noexcept
{
}

template<typename _Key, typename _T>
inline _T probe_hash_table<_Key, _T>::get(_Key key) const noexcept
{
}

template<typename _Key, typename _T>
inline void probe_hash_table<_Key, _T>::print(std::ostream& ostr) const
{
	for (auto&& cell : m_table)
		ostr << cell.data << std::endl;
}

template<typename _Key, typename _T>
inline std::ostream& operator<<(std::ostream& ostr, const probe_hash_table<_Key, _T>& table)
{
	table.print(ostr);

	return ostr;
}