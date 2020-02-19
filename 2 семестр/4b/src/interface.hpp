#pragma once

#include "table_cli.hpp"

#include <string_view>

template<typename _Key1, typename _Key2>
using func_t = void (table_cli<_Key1, _Key2>::*)();

template<typename _Key1, typename _Key2>
void communicate(const std::vector<std::pair<std::string_view, func_t<_Key1, _Key2>>>& opts);

template<typename _Key1, typename _Key2>
void communicate(const std::vector<std::pair<std::string_view, func_t<_Key1, _Key2>>>& opts)
{
	std::cout << "Enter capacity\n> ";
	std::size_t capacity;
	std::cin >> capacity;

	std::cout << "\nEnter filepath to db\n> ";
	std::filesystem::path path;
	std::cin >> std::ws >> path;

	table_cli<_Key1, _Key2> cli(std::make_shared<table<_Key1, _Key2>>(capacity, path));

#define ever (;;)
	for ever
	{
		std::cout << "[0] - Exit" << std::endl;
		for (size_t i{}; i < opts.size(); ++i)
			std::cout << "[" << i + 1 << "] - " << opts[i].first << std::endl;
			
		std::cout << "\nOpt\n> ";
		std::size_t i{};
		std::cin >> i;

		if (!i)
			break;
		else if (i > opts.size())
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