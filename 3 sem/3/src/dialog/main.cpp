// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "../dynamic/DiceGroup.hpp"
#include "../../include/Utility.hpp"

#include <iostream>

#pragma comment(lib, "../../lib/dynamic.lib")

signed main()
{
	std::ios_base::sync_with_stdio(false);

	try
	{
		DiceGroup g;
		while (true)
		{
			std::cout <<
				"WELCOME TO THE CLUB, BUDDY!\n"
				"+-------------------------+\n"
				"|0. Leave club            |\n"
				"|1. Try ur luck           |\n"
				"|2. Decrease chance       |\n"
				"|3. Increase chance       |\n"
				"+-------------------------+\n" <<
				std::endl;

			switch (input<int>("op"))
			{
			case 0: return 0;
			case 1:
			{
				g.roll();

				std::cout << g << "\n" << (g.areAllTheSame() ? "Congrats!!! U won dat" : "Unfortunately, not today("); 
				
				break;
			}
			case 2: g += uchar_t(); break;
			case 3: g -= uchar_t(); break;

			default: std::cerr << "Wrong opt" << std::endl;
			}

			std::cout << std::endl;
		}
	}
	catch (...)
	{
		std::cerr << "Unknown exception occured" << std::endl;

		return -1;
	}

	return 0;
}