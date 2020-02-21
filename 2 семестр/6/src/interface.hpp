#pragma once

#include "graph_cli.hpp"

#include <string_view>

template<typename _T>
using func_t = void (graph_cli<_T>::*)();

template<typename _T>
void communicate(const std::vector<std::pair<std::string_view, func_t<_T>>>& opts);

template<typename _T>
void communicate(const std::vector<std::pair<std::string_view, func_t<_T>>>& opts)
{
	std::cout << "\nEnter filepath to graph\n> ";
	std::filesystem::path path;
	std::cin >> std::ws >> path;

	graph_cli<_T> cli(std::make_shared<graph<_T>>(), path);

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