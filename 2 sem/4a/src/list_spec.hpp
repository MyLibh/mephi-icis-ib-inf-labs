#pragma once

#include "linked_list.hpp"
#include "item.hpp"

#include <algorithm>

template<typename _Key, typename _Key1, typename _Key2, typename _T>
using data_t = std::pair<std::shared_ptr<_Key>, item<_Key1, _Key2, _T>>;

template<typename _Key, typename _Key1, typename _Key2, typename _T>
std::ostream& operator<<(std::ostream& ostr, const data_t<_Key, _Key1, _Key2, _T>& data)
{
	ostr << data.second;

	return ostr;
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
class linked_list<data_t<_Key, _Key1, _Key2, _T>>
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

	using item_t = item<_Key1, _Key2, _T>;
	using node_t = typename node<data_t<_Key, _Key1, _Key2, _T>>::node_t;

public:
	linked_list(std::shared_ptr<std::size_t> item_t::* ptr_to_index) noexcept;

	[[nodiscard]]
	__forceinline auto length() const noexcept
	{
		return m_length;
	}

	void add(std::shared_ptr<_Key> key, item_t data);

	bool remove(std::shared_ptr<_Key> key, const std::size_t index) noexcept;

	void remove(std::shared_ptr<_Key> key) noexcept;

	[[nodiscard]]
	auto get(std::shared_ptr<_Key> key, const std::size_t index) const noexcept;

	[[nodiscard]]
	auto get(std::shared_ptr<_Key> key) const;

	void print(std::ostream& ostr = std::cout) const;

private:
	std::shared_ptr<node_t>				   m_head;
	std::size_t							   m_length;
	std::shared_ptr<std::size_t> item_t::* m_ptr_to_index;
};

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline linked_list<data_t<_Key, _Key1, _Key2, _T>>::linked_list(std::shared_ptr<std::size_t> item_t::* ptr_to_index) noexcept :
	m_head{ nullptr },
	m_length{ 0U },
	m_ptr_to_index{ ptr_to_index }
{ }

template<typename _Key, typename _Key1, typename _Key2, typename _T>
void linked_list<data_t<_Key, _Key1, _Key2, _T>>::add(std::shared_ptr<_Key> key, item_t data)
{
	auto ptr{ m_head };
	std::size_t index{};
	while (ptr)
	{
		if (*ptr->data.first == *key)
			index = std::max(index, *(ptr->data.second.*m_ptr_to_index));

		if (!ptr->next)
		{
			ptr->next = std::make_shared<node_t>(std::make_pair(key, data));

			*(ptr->next->data.second.*m_ptr_to_index) = ++index;

			break;
		}

		ptr = ptr->next;
	}

	if (!m_head)
	{
		m_head = std::make_shared<node_t>(std::make_pair(key, data));

		*(m_head->data.second.*m_ptr_to_index) = ++index;
	}

	m_length++;
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
bool linked_list<data_t<_Key, _Key1, _Key2, _T>>::remove(std::shared_ptr<_Key> key, const std::size_t index) noexcept
{
	auto ptr{ m_head };
	decltype(ptr) prev{ nullptr };
	while (ptr)
	{
		if (auto&& [k, item] = ptr->data; *k == *key && *(item.*m_ptr_to_index) == index)
		{
			if (prev)
				prev->next = ptr->next;
			else
				m_head = ptr->next;

			m_length--;

			return true;
		}

		prev = ptr;
		ptr = ptr->next;
	}

	return false;
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
void linked_list<data_t<_Key, _Key1, _Key2, _T>>::remove(std::shared_ptr<_Key> key) noexcept
{
	auto ptr{ m_head };
	decltype(ptr) prev{ nullptr };
	while (ptr)
	{
		if (*ptr->data.first == *key)
		{
			if (prev)
				prev->next = ptr->next;
			else
				m_head = ptr->next;

			m_length--;
		}

		prev = ptr;
		ptr = ptr->next;
	}
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
[[nodiscard]]
inline auto linked_list<data_t<_Key, _Key1, _Key2, _T>>::get(std::shared_ptr<_Key> key, const std::size_t index) const noexcept
{
	auto ptr{ m_head };
	while (ptr)
	{
		if (auto&& [k, item] = ptr->data; *k == *key && *(item.*m_ptr_to_index) == index)
			return item;

		ptr = ptr->next;
	}

	return item_t{ };
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
[[nodiscard]]
inline auto linked_list<data_t<_Key, _Key1, _Key2, _T>>::get(std::shared_ptr<_Key> key) const
{
	linked_list<item<_Key1, _Key2, _T>> res;

	auto ptr{ m_head };
	while (ptr)
	{
		if (*ptr->data.first == *key)
			res.add(ptr->data.second);

		ptr = ptr->next;
	}

	return std::move(res);
}

template<typename _Key, typename _Key1, typename _Key2, typename _T>
inline void linked_list<data_t<_Key, _Key1, _Key2, _T>>::print(std::ostream& ostr /* = std::cout */) const
{
	auto ptr{ m_head };
	while (ptr)
	{
		ostr << ptr->data.second << " ";

		ptr = ptr->next;
	}
}