#pragma once

#include "linked_list.hpp"
#include "item.hpp"

#include <algorithm>

template<typename _Key, typename _Key1, typename _Key2>
class linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>
{
	using item_t = item<_Key1, _Key2>;
	using data_t = std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>;
	using node_t = typename node<data_t>::node_t;

public:
	linked_list(std::size_t item_t::* ptr_to_index) noexcept;

	[[nodiscard]]
	__forceinline auto length() const noexcept
	{
		return m_length;
	}

	void add(const _Key key, ptr_to_item_t<_Key1, _Key2> data, const bool to_index = true);

	bool remove(const _Key key, const std::size_t index) noexcept;

	std::size_t remove(const _Key key) noexcept;

	[[nodiscard]]
	auto get(const _Key key, const std::size_t index) const noexcept;

	[[nodiscard]]
	auto get(const _Key key) const;

	void print(std::ostream& ostr = std::cout) const;

private:
	std::shared_ptr<node_t>	m_head;
	std::size_t				m_length;
	std::size_t item_t::*	m_ptr_to_index;
};

template<typename _Key, typename _Key1, typename _Key2>
inline linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>::linked_list(std::size_t item_t::* ptr_to_index) noexcept :
	m_head{ nullptr },
	m_length{ 0U },
	m_ptr_to_index{ ptr_to_index }
{ }

template<typename _Key, typename _Key1, typename _Key2>
inline void linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>::add(const _Key key, ptr_to_item_t<_Key1, _Key2> item, const bool to_index /* = true */)
{
	auto ptr{ m_head };
	std::size_t index{};
	while (ptr)
	{
		if (ptr->data.first == key)
			index = std::max(index, *ptr->data.second.*m_ptr_to_index);

		if (!ptr->next)
		{
			ptr->next = std::make_shared<node_t>(std::make_pair(key, item));

			if(to_index)
				*ptr->next->data.second.*m_ptr_to_index = ++index;

			break;
		}

		ptr = ptr->next;
	}

	if (!m_head)
	{
		m_head = std::make_shared<node_t>(std::make_pair(key, item));

		if (to_index)
			*m_head->data.second.*m_ptr_to_index = ++index;
	}

	m_length++;
}

template<typename _Key, typename _Key1, typename _Key2>
inline bool linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>::remove(const _Key key, const std::size_t index) noexcept
{
	auto ptr{ m_head };
	decltype(ptr) prev{ nullptr };
	while (ptr)
	{
		if (auto&& [k, item] = ptr->data; k == key && *item.*m_ptr_to_index == index)
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

template<typename _Key, typename _Key1, typename _Key2>
inline std::size_t linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>::remove(const _Key key) noexcept
{
	auto old_len{ m_length };
	auto ptr{ m_head };
	decltype(ptr) prev{ nullptr };
	while (ptr)
	{
		if (ptr->data.first == key)
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

	return (old_len - m_length);
}

template<typename _Key, typename _Key1, typename _Key2>
[[nodiscard]]
inline auto linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>::get(const _Key key, const std::size_t index) const noexcept
{
	auto ptr{ m_head };
	while (ptr)
	{
		if (auto&& [k, item] = ptr->data; k == key && *item.*m_ptr_to_index == index)
			return item;

		ptr = ptr->next;
	}

	return std::shared_ptr<item_t>();
}

template<typename _Key, typename _Key1, typename _Key2>
[[nodiscard]]
inline auto linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>::get(const _Key key) const
{
	linked_list<item_t> res;

	auto ptr{ m_head };
	while (ptr)
	{
		if (ptr->data.first == key)
			res.add(*ptr->data.second);

		ptr = ptr->next;
	}

	return std::move(res);
}

template<typename _Key, typename _Key1, typename _Key2>
inline void linked_list<std::pair<_Key, ptr_to_item_t<_Key1, _Key2>>>::print(std::ostream& ostr /* = std::cout */) const
{
	auto ptr{ m_head };
	while (ptr)
	{
		ostr << *ptr->data.second << " ";

		ptr = ptr->next;
	}
}