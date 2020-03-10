// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "interface.hpp"

void init_io();

signed main()
{
	try
	{
		init_io();

		using table_cli_t = table_cli<int, int, std::string>;
		std::vector<std::pair<std::string_view, void (table_cli_t::*)()>> opts
		{
			{ "Add", &table_cli_t::add },
			{ "Remove", &table_cli_t::remove },
			{ "Remove all", &table_cli_t::remove_all },
			{ "Find", &table_cli_t::find },
			{ "Print", &table_cli_t::print}
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
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	std::cerr.tie(nullptr);
}