#ifndef __UTILITY_HPP_INCLUDED__
#define __UTILITY_HPP_INCLUDED__

#include <iostream>

namespace std
{
	template<typename _Ty>
	concept numeric = std::is_arithmetic_v<_Ty>; // USER-DEFINED concept
} // namespace std

template<std::numeric _Ty>
_NODISCARD
_Ty input(const std::string_view name = "", std::istream& istr = std::cin)
{
	if (!name.empty())
		std::cout << "Input " << name << ":\n> ";

	_Ty value{};
	while (!(istr >> value))
	{
		if (!istr.good())
		{
			istr.clear();
			istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cerr << "Wrong input" << std::endl;
		}

		if (!name.empty())
			std::cout << "Input " << name << ":\n> ";
	}

	return value;
}

#endif /* !__UTILITY_HPP_INCLUDED__ */