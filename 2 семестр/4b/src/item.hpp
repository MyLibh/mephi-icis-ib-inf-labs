#pragma once

#include "util.hpp"

#include <memory>

template<typename _Key1, typename _Key2>
struct item
{
public:
	constexpr item() noexcept;
	item(const item&)		  = default;
	item(item&&)	 noexcept = default;
	item(const _Key1 key1, const  _Key2 key2, const std::streampos offset, const std::size_t length, 
		const std::size_t index1 = 0U, const std::size_t index2 = 0U) noexcept;

	item& operator=(const item&) = default;
	item& operator=(item&&)		 = default;

public:
	_Key1		   key1;
	_Key2		   key2;
	std::streampos offset;
	std::size_t	   length;
	std::size_t	   index1;
	std::size_t	   index2;
};

template<typename _Key1, typename _Key2>
using ptr_to_item_t = std::shared_ptr<item<_Key1, _Key2>>;

template<typename _Key1, typename _Key2>
std::ostream& operator<<(std::ostream& ostr, const item<_Key1, _Key2>& item);

template<typename _Key1, typename _Key2>
inline constexpr item<_Key1, _Key2>::item() noexcept :
	key1{ _Key1() },
	key2{ _Key2() },
	offset{ 0U },
	length{ 0U },
	index1{ 0U },
	index2{ 0U }
{ }

template<typename _Key1, typename _Key2>
inline item<_Key1, _Key2>::item(const _Key1 key1, 
								const _Key2 key2, 
								const std::streampos offset, 
								const std::size_t length, 
								const std::size_t index1 /* = 0U */,
								const std::size_t index2 /* = 0U */) noexcept :
	key1{ key1 },
	key2{ key2 },
	offset{ offset },
	length{ length },
	index1{ index1 },
	index2{ index2 }
{ }

template<typename _Key1, typename _Key2>
std::ostream& operator<<(std::ostream& ostr, const item<_Key1, _Key2>& item)
{
	auto&& [key1, key2, offset, length, index1, index2] = item;
	ostr << "[" << key1 << " " << key2 << " +" << offset << " " << length << " " << index1 << " " << index2 << "]";

	return ostr;
}
