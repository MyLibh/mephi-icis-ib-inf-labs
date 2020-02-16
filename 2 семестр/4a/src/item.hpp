#pragma once

#include "util.hpp"

#include <memory>

template<typename _Key1, typename _Key2, typename _T>
struct item
{
	static_assert(has_operator_out_v<_T>, "_T must have operator std::ostream<<");

public:
	constexpr item() noexcept = default;
	item(const item&)		  = default;
	item(item&&)	 noexcept = default;
	item(const _Key1 key1, const  _Key2 key2, const _T data);

	item& operator=(const item&) = default;
	item& operator=(item&&)		 = default;

public:
	std::shared_ptr<_Key1>		 key1;
	std::shared_ptr<_Key2>		 key2;
	std::shared_ptr<_T>			 data;
	std::shared_ptr<std::size_t> index1;
	std::shared_ptr<std::size_t> index2;
};

template<typename _Key1, typename _Key2, typename _T>
std::ostream& operator<<(std::ostream& ostr, const item<_Key1, _Key2, _T>& item);

template<typename _Key1, typename _Key2, typename _T>
inline item<_Key1, _Key2, _T>::item(const _Key1 key1, const _Key2 key2, const _T data) :
	key1{ std::make_shared<_Key1>(key1) },
	key2{ std::make_shared<_Key2>(key2) },
	data{ std::make_shared<_T>(data) },
	index1{ std::make_shared<std::size_t>() },
	index2{ std::make_shared<std::size_t>() }
{ }

template<typename _Key1, typename _Key2, typename _T>
std::ostream& operator<<(std::ostream& ostr, const item<_Key1, _Key2, _T>& item)
{
	ostr << "[" << *item.key1 << " " << *item.key2 << " " << *item.data << " " << *item.index1 << " " << *item.index2 << "]";

	return ostr;
}