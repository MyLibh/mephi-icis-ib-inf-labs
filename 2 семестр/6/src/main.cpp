// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "interface.hpp"

#include <iostream>

void init_io();

signed main()
{
	try
	{
		init_io();

		using graph_cli_t = graph_cli<double>;
		std::vector<std::pair<std::string_view, func_t<double>>> opts
		{
			{ "Add vertex", &graph_cli_t::add_vertex },
			{ "Add edge", &graph_cli_t::add_edge },
			{ "Remove vertex", &graph_cli_t::remove_vertex },
			{ "Find way", &graph_cli_t::find_way },
			{ "Print", &graph_cli_t::print },
			{ "Generate graph", &graph_cli_t::gen },
			{ "Profile", &graph_cli_t::profile }
		};

		communicate(opts);
	}
	catch (...)
	{
		std::cerr << "Unhandled exception" << std::endl;
	}

	return 0;
}

void init_io()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	std::cerr.tie(nullptr);
}