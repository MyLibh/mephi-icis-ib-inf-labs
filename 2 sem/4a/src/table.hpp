#pragma once

#include "chain_spec.hpp"
#include "probe_spec.hpp"

template<typename _Key1, typename _Key2, typename _T>
class table_cli;

template<typename _Key1, typename _Key2, typename _T>
class table
{
	using item_t = item<_Key1, _Key2, _T>;

public:
	table(const std::size_t capacity);

	bool add(std::shared_ptr<_Key1> key1, std::shared_ptr<_Key2> key2, item_t data);

	bool remove_by_key1(std::shared_ptr<_Key1> key, const std::size_t index);
	void remove_all_by_key1(std::shared_ptr<_Key1> key);
	bool remove_by_key2(std::shared_ptr<_Key2> key, const std::size_t index);
	void remove_all_by_key2(std::shared_ptr<_Key2> key);

	linked_list<item_t> find_by_key1(std::shared_ptr<_Key1> key);
	linked_list<item_t> find_by_key2(std::shared_ptr<_Key2> key);

	void print(std::ostream& ostr = std::cout);

private:
	chain_hash_table<_Key1, item_t> m_chain;
	probe_hash_table<_Key2, item_t> m_probe;
};

template<typename _Key1, typename _Key2, typename _T>
inline table<_Key1, _Key2, _T>::table(const std::size_t capacity) :
	m_chain(capacity, &item_t::index1),
	m_probe(capacity, &item_t::index2)
{ }

template<typename _Key1, typename _Key2, typename _T>
inline bool table<_Key1, _Key2, _T>::add(std::shared_ptr<_Key1> key1, std::shared_ptr<_Key2> key2, item_t data)
{
	if (!m_probe.add(key2, data))
		return false;


	m_chain.add(key1, data);

	return true;
}

template<typename _Key1, typename _Key2, typename _T>
inline bool table<_Key1, _Key2, _T>::remove_by_key1(std::shared_ptr<_Key1> key, const std::size_t index)
{
	auto item = m_chain.get(key, index);

	return (m_probe.remove(item.key2, *item.index2) == m_chain.remove(key, index));
}

template<typename _Key1, typename _Key2, typename _T>
inline void table<_Key1, _Key2, _T>::remove_all_by_key1(std::shared_ptr<_Key1> key)
{
	auto list = m_chain.get(key);
	for (auto ptr = list.get(); ptr; ptr = ptr->next)
		m_probe.remove(ptr->data.key2, *ptr->data.index2);

	m_chain.remove(key);
}

template<typename _Key1, typename _Key2, typename _T>
inline bool table<_Key1, _Key2, _T>::remove_by_key2(std::shared_ptr<_Key2> key, const std::size_t index)
{
	auto item = m_probe.get(key, index);

	return (m_chain.remove(item.key1, *item.index1) == m_probe.remove(key, index));
}

template<typename _Key1, typename _Key2, typename _T>
inline void table<_Key1, _Key2, _T>::remove_all_by_key2(std::shared_ptr<_Key2> key)
{
	auto list = m_probe.get(key);
	for (auto ptr = list.get(); ptr; ptr = ptr->next)
		m_chain.remove(ptr->data.key1, *ptr->data.index1);

	m_probe.remove(key);
}

template<typename _Key1, typename _Key2, typename _T>
inline linked_list<typename table<_Key1, _Key2, _T>::item_t> table<_Key1, _Key2, _T>::find_by_key1(std::shared_ptr<_Key1> key)
{
	return m_chain.get(key);
}

template<typename _Key1, typename _Key2, typename _T>
inline linked_list<typename table<_Key1, _Key2, _T>::item_t> table<_Key1, _Key2, _T>::find_by_key2(std::shared_ptr<_Key2> key)
{
	return m_probe.get(key);
}

template<typename _Key1, typename _Key2, typename _T>
inline void table<_Key1, _Key2, _T>::print(std::ostream& ostr /* = std::cout */)
{
	ostr << "Separate Chaining(" << m_chain.length() << ")\n" << m_chain << std::endl << std::endl;

	ostr << "Linear Probing(" << m_probe.length() << "/" << m_probe.capacity() << ")\n" << m_probe << std::endl;
}