#ifndef __UTILITY_HPP_INCLUDED__
#define __UTILITY_HPP_INCLUDED__

#include <string_view>

namespace MobileRobots::Utility
{
	template <typename _Ty>
	struct ReverseWrapper 
	{ 
		_Ty& iterable;
	};

	template <typename _Ty>
	inline auto begin(ReverseWrapper<_Ty> rev) 
	{ 
		return std::rbegin(rev.iterable); 
	}

	template <typename _Ty>
	auto end(ReverseWrapper<_Ty> rev)
	{ 
		return std::rend(rev.iterable); 
	}

	template <typename _Ty>
	inline ReverseWrapper<_Ty> reverse(_Ty&& iterable) 
	{ 
		return { iterable }; 
	}

	void showError(const std::string_view msg);
} // namespace MobileRobots::Utility

#endif /* !__UTILITY_HPP_INCLUDED__ */