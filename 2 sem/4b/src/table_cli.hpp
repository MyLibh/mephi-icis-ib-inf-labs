#pragma once

#include "table.hpp"

#include <string>

template<typename _Key1, typename _Key2>
class table_cli
{
	static_assert(has_operator_in_v<_Key1>, "_Key1 must have operator std::istream>>");
	static_assert(has_operator_in_v<_Key2>, "_Key2 must have operator std::istream>>");

private:
	bool use_key1() const;

	_Key1 get_key1() const;
	_Key2 get_key2() const;
	std::string get_data() const;
	std::size_t get_index() const;

public:
	table_cli(std::shared_ptr<table<_Key1, _Key2>> table) noexcept;

	void add();
	void remove();
	void remove_all();
	void find();
	void print(); // Not const to match the pointer to member

private:
	std::shared_ptr<table<_Key1, _Key2>> m_table;
};

template<typename _Key1, typename _Key2>
inline bool table_cli<_Key1, _Key2>::use_key1() const
{
	std::cout << "What key should we use 1 or 2?\n> ";

	int k{};
	std::cin >> k;

	return !(k - 1);
}

template<typename _Key1, typename _Key2>
inline _Key1 table_cli<_Key1, _Key2>::get_key1() const
{
	std::cout << "Enter key1\n> ";

	_Key1 key1{};
	std::cin >> std::ws >> key1;

	return key1;
}

template<typename _Key1, typename _Key2>
inline _Key2 table_cli<_Key1, _Key2>::get_key2() const
{
	std::cout << "Enter key2\n> ";

	_Key2 key2{};
	std::cin >> std::ws >> key2;

	return key2;
}

template<typename _Key1, typename _Key2>
inline std::string table_cli<_Key1, _Key2>::get_data() const
{
	std::cout << "Enter data\n> ";
	
	std::string data{};
	std::getline(std::cin >> std::ws, data);

	return data;
}

template<typename _Key1, typename _Key2>
inline std::size_t table_cli<_Key1, _Key2>::get_index() const
{
	std::cout << "Enter index\n> ";

	std::size_t index{};
	std::cin >> index;

	return index;
}

template<typename _Key1, typename _Key2>
inline table_cli<_Key1, _Key2>::table_cli(std::shared_ptr<table<_Key1, _Key2>> table) noexcept :
	m_table{ table }
{ }

template<typename _Key1, typename _Key2>
inline void table_cli<_Key1, _Key2>::add()
{
	auto key1{ get_key1() };
	auto key2{ get_key2() };
	auto data{ get_data() };
	
	if(!m_table->add(std::make_shared<item<_Key1, _Key2>>(key1, key2, 0, 0), data))
		std::cerr << "[ERROR] Cannot add new element" << std::endl;
}

template<typename _Key1, typename _Key2>
inline void table_cli<_Key1, _Key2>::remove()
{ 
	if (auto res = use_key1() ? 
		m_table->remove_by_key1(get_key1(), get_index()) : 
		m_table->remove_by_key2(get_key2(), get_index()); 
	!res)
		std::cerr << "[ERROR] Cannot remove element" << std::endl;
}

template<typename _Key1, typename _Key2>
inline void table_cli<_Key1, _Key2>::remove_all()
{
	if (auto res = use_key1() ? 
		m_table->remove_all_by_key1(get_key1()) : 
		m_table->remove_all_by_key2(get_key2());
	!res)
		std::cerr << "[ERROR] Remove error occured" << std::endl;
}

template<typename _Key1, typename _Key2>
inline void table_cli<_Key1, _Key2>::find()
{
	auto list = std::move(use_key1() ? m_table->find_by_key1(get_key1()) : m_table->find_by_key2(get_key2()));

	std::cout << "Result\n" << list << std::endl;
}

template<typename _Key1, typename _Key2>
inline void table_cli<_Key1, _Key2>::print()
{
	m_table->print();
}