/**
 * @file Utility.hpp
 *
 * @defgroup Utility
 *
 * @brief Utility functions
 *
 * @author mylibh
*/
#ifndef __UTILITY_HPP_INCLUDED__
#define __UTILITY_HPP_INCLUDED__

#include <string_view>

namespace MobileRobots::Utility
{
	/**
	 * @brief Helper struct to use reversed range-based for loop
	*/
	template <typename _Ty>
	struct ReverseWrapper 
	{ 
		_Ty& iterable;
	};

	/**
	 * @brief Constructs reverse begin iterator
	*/
	template <typename _Ty>
	inline auto begin(ReverseWrapper<_Ty> rev) 
	{ 
		return std::rbegin(rev.iterable); 
	}

	/**
	 * @brief Constructs reverse end iterator
	*/
	template <typename _Ty>
	inline auto end(ReverseWrapper<_Ty> rev)
	{ 
		return std::rend(rev.iterable); 
	}

	/**
	 * @brief Constructs ReverseWrapper object
	*/
	template <typename _Ty>
	inline ReverseWrapper<_Ty> reverse(_Ty&& iterable) 
	{ 
		return { iterable }; 
	}

	/**
	 * @brief Shows message box with error
	 * 
	 * @param msg Error description
	*/
	void showError(const std::string_view msg);
} // namespace MobileRobots::Utility

#endif /* !__UTILITY_HPP_INCLUDED__ */