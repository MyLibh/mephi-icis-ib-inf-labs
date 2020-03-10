#pragma once

#include "table_cli.hpp"

#include <string_view>

template<typename _Key1, typename _Key2, typename _T>
using func_t = void (table_cli<_Key1, _Key2, _T>::*)();

template<typename _Key1, typename _Key2, typename _T>
void communicate(const std::vector<std::pair<std::string_view, func_t<_Key1, _Key2, _T>>>& opts);

template<typename _Key1, typename _Key2, typename _T>
void communicate(const std::vector<std::pair<std::string_view, func_t<_Key1, _Key2, _T>>>& opts)
{
	auto tab{ std::make_shared<table<_Key1, _Key2, _T>>(5) };
	table_cli<_Key1, _Key2, _T> cli(tab);
	auto size{ opts.size() };

	auto cls = []() { system("cls"); };

#define ever (;;)
	for ever
	{
		std::cout << "[0] - Exit" << std::endl;
		for (size_t i{}; i < size; ++i)
			std::cout << "[" << i + 1 << "] - " << opts[i].first << std::endl;
			
		std::cout << "\nOpt\n> ";
		std::size_t i{};
		std::cin >> i;

		if (!i)
			break;
		else if (i > size)
		{
			system("cls");

			std::cout << "Wrong input" << std::endl << std::endl;

			continue;
		}
		else
		{
			system("cls");

			(cli.*opts[i - 1].second)();
		}

		std::cout << std::endl;
	}
#undef ever
}