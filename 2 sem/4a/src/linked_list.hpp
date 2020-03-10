#pragma once

#include "util.hpp"

#include <memory>
#include <iostream>

template<typename _T>
struct node
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

public:
	using node_t = node<_T>;

public:
	node() noexcept = default;
	node(const node&) noexcept = default;
	node(node&&) noexcept = default;
	node(_T data);

	node& operator=(const node&) noexcept = default;
	node& operator=(node&&) noexcept = default;

public:
	_T						data;
	std::shared_ptr<node_t>	next;
};

template<typename _T>
node<_T>::node(_T data) :
	data{ data },
	next{ nullptr }
{ }

template<typename _T>
class linked_list final
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

public:
	using node_t = typename node<_T>::node_t;

public:
	linked_list() noexcept;
	linked_list(const linked_list&) = delete;
	linked_list(linked_list&& list) noexcept;

	linked_list& operator=(const linked_list&) = delete;
	linked_list& operator=(linked_list&& list);

	[[nodiscard]]
	__forceinline auto length() const noexcept
	{
		return m_length;
	}

	void add(_T data);
	
	[[nodiscard]]
	__forceinline auto get() const noexcept 
	{
		return m_head;
	}

	void print(std::ostream& ostr = std::cout) const;

	friend std::ostream& operator<<(std::ostream& ostr, const linked_list<_T>& list);

private:
	std::shared_ptr<node_t> m_head;
	std::size_t				m_length;
};

template<typename _T>
linked_list<_T>::linked_list() noexcept :
	m_head{ nullptr },
	m_length{ 0U }
{ }

template<typename _T>
inline linked_list<_T>::linked_list(linked_list&& list) noexcept :
	m_head{ list.m_head },
	m_length{ list.m_length }
{ }

template<typename _T>
inline linked_list<_T>& linked_list<_T>::operator=(linked_list<_T>&& list)
{
	m_head = list.m_head;
	m_length = list.m_length;

	return *this;
}

template<typename _T>
inline void linked_list<_T>::add(_T data)
{
	auto ptr{ m_head };
	while (ptr)
	{
		if (!ptr->next)
		{
			ptr->next = std::make_shared<node_t>(data);

			break;
		}

		ptr = ptr->next;
	}

	if (!m_head)
		m_head = std::make_shared<node_t>(data);

	m_length++;
}

template<typename _T>
inline void linked_list<_T>::print(std::ostream& ostr /* = std::cout */) const
{
	auto ptr{ m_head };
	while (ptr)
	{
		ostr << ptr->data << " ";

		ptr = ptr->next;
	}
}

template<typename _T>
std::ostream& operator<<(std::ostream& ostr, const linked_list<_T>& list)
{
	list.print(ostr);

	return ostr;
}