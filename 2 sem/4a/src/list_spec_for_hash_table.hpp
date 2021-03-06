#pragma once

#include "linked_list.hpp"

template<typename _Key, typename _T>
class linked_list<std::pair<_Key, _T>>
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

	using node_t = typename node<_T>::node_t;

public:
	[[nodiscard]]
	__forceinline auto length() const noexcept
	{
		return m_length;
	}

	void add(std::shared_ptr<_Key> key, std::shared_ptr<_T> data);

	bool remove(std::shared_ptr<_Key> key) noexcept;

	[[nodiscard]]
	std::shared_ptr<_T> get(std::shared_ptr<_Key> key) const noexcept;

	void print(std::ostream& ostr = std::cout) const;

private:
	std::shared_ptr<node_t> m_head;
	std::size_t				m_length;
};

template<typename _Key, typename _T>
inline void linked_list<std::pair<_Key, _T>>::add(std::shared_ptr<_Key> key, std::shared_ptr<_T> data)
{
	auto ptr{ m_head };
	while (ptr)
	{
		if (!ptr->next)
		{
			ptr->next = std::make_shared<node_t>(std::make_pair(key, data));

			break;
		}

		ptr = ptr->next;
	}

	if (!m_head)
		m_head = std::make_shared<node_t>(std::make_pair(key, data));

	m_length++;
}

template<typename _Key, typename _T>
inline bool linked_list<std::pair<_Key, _T>>::remove(std::shared_ptr<_Key> key) noexcept
{
	auto ptr{ m_head };
	decltype(ptr) prev{ nullptr };
	while (ptr)
	{
		if (ptr->data.first == *key)
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

template<typename _Key, typename _T>
inline std::shared_ptr<_T> linked_list<std::pair<_Key, _T>>::get(std::shared_ptr<_Key> key) const noexcept
{
	auto ptr{ m_head };
	while (ptr)
	{
		if (ptr->data.first == *key)
			return ptr->data.second;

		ptr = ptr->next;
	}

	return nullptr;
}

template<typename _Key, typename _T>
inline void linked_list<std::pair<_Key, _T>>::print(std::ostream& ostr /* = std::cout */) const
{
	auto ptr{ m_head };
	while (ptr)
	{
		ostr << ptr->data.second << " ";

		ptr = ptr->next;
	}
}