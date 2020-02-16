#pragma once

#include "table.hpp"

template<typename _Key1, typename _Key2, typename _T>
class table_cli
{
	static_assert(has_operator_in_v<_Key1>, "_Key1 must have operator std::istream>>");
	static_assert(has_operator_in_v<_Key2>, "_Key2 must have operator std::istream>>");
	static_assert(has_operator_in_v<_T>, "_T must have operator std::istream>>");

private:
	bool use_key1() const;

	_Key1 get_key1() const;
	_Key2 get_key2() const;
	_T get_data() const;
	std::size_t get_index() const;

public:
	table_cli(std::shared_ptr<table<_Key1, _Key2, _T>> table);

	void add();
	void remove();
	void remove_all();
	void find();
	void print(); // Not const to match the pointer to member

private:
	std::shared_ptr<table<_Key1, _Key2, _T>> m_table;
};

template<typename _Key1, typename _Key2, typename _T>
inline bool table_cli<_Key1, _Key2, _T>::use_key1() const
{
	std::cout << "What key should we use 1 or 2?\n> ";

	int k{};
	std::cin >> k;

	return !(k - 1);
}

template<typename _Key1, typename _Key2, typename _T>
inline _Key1 table_cli<_Key1, _Key2, _T>::get_key1() const
{
	std::cout << "Enter key1\n> ";

	_Key1 key1{};
	std::cin >> key1;

	return key1;
}

template<typename _Key1, typename _Key2, typename _T>
inline _Key2 table_cli<_Key1, _Key2, _T>::get_key2() const
{
	std::cout << "Enter key2\n> ";

	_Key2 key2{};
	std::cin >> key2;

	return key2;
}

template<typename _Key1, typename _Key2, typename _T>
inline _T table_cli<_Key1, _Key2, _T>::get_data() const
{
	std::cout << "Enter data\n> ";
	
	_T data{};
	std::cin >> data;

	return data;
}

template<typename _Key1, typename _Key2, typename _T>
inline std::size_t table_cli<_Key1, _Key2, _T>::get_index() const
{
	std::cout << "Enter index\n> ";

	std::size_t index{};
	std::cin >> index;

	return index;
}

template<typename _Key1, typename _Key2, typename _T>
inline table_cli<_Key1, _Key2, _T>::table_cli(std::shared_ptr<table<_Key1, _Key2, _T>> table) :
	m_table{ table }
{ }

template<typename _Key1, typename _Key2, typename _T>
inline void table_cli<_Key1, _Key2, _T>::add()
{
	auto key1 = get_key1();
	auto key2 = get_key2();
	auto data = get_data();
	
	item<_Key1, _Key2, _T> res(key1, key2, data);

	m_table->add(res.key1, res.key2, res);
}

template<typename _Key1, typename _Key2, typename _T>
inline void table_cli<_Key1, _Key2, _T>::remove()
{ 
	if (use_key1())
	{
		auto key1 = get_key1();
		auto index = get_index();

		m_table->remove_by_key1(std::make_shared<_Key1>(key1), index);
	}
	else
	{
		auto key2 = get_key2();
		auto index = get_index();

		m_table->remove_by_key2(std::make_shared<_Key2>(key2), index);
	}
}

template<typename _Key1, typename _Key2, typename _T>
inline void table_cli<_Key1, _Key2, _T>::remove_all()
{
	if (use_key1())
	{
		auto key1 = get_key1();

		m_table->remove_all_by_key1(std::make_shared<_Key1>(key1));
	}
	else
	{
		auto key2 = get_key2();

		m_table->remove_all_by_key2(std::make_shared<_Key2>(key2));
	}
}

template<typename _Key1, typename _Key2, typename _T>
inline void table_cli<_Key1, _Key2, _T>::find()
{
	linked_list<item<_Key1, _Key2, _T>> list;
	if (use_key1())
	{
		auto key1 = get_key1();

		list = std::move(m_table->find_by_key1(std::make_shared<_Key1>(key1)));
	}
	else
	{
		auto key2 = get_key2();

		list = std::move(m_table->find_by_key2(std::make_shared<_Key2>(key2)));
	}

	std::cout << "Result" << std::endl;
	list.print();
}

template<typename _Key1, typename _Key2, typename _T>
inline void table_cli<_Key1, _Key2, _T>::print()
{
	m_table->print();
}