// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Ellipse.hpp"
#include "Utility.hpp"

signed main()
{
	std::ios_base::sync_with_stdio(false);

	auto a{ input<double>("a") },
		 b{ input<double>("b") };

	Ellipse ellipse(a, b);
	uint16_t op{};
	while (true)
	{
		std::cout <<
			"0. Exit\n"
			"1. Info\n"
			"2. Set a\n"
			"3. Set b\n"
			"4. Get cartesian x\n"
			"5. Get minmax distance to foci\n" <<
			std::endl;

		if (auto op{ input<uint16_t>("op") }; !op) // The best CLI ever
			break;
		else if (op == 1)
		{
			std::cout <<
				"a: " << ellipse.a << std::endl <<
				"b: " << ellipse.b << std::endl <<
				"c: " << ellipse.c << std::endl <<
				"e: " << ellipse.e << std::endl <<
				"length: " << ellipse.length << std::endl <<
				"area: " << ellipse.area << std::endl;
		}
		else if (op == 2)
			ellipse.a = input<double>("new a");
		else if (op == 3)
			ellipse.b = input<double>("new b");
		else if (op == 4)
		{
			const auto&& [y1, y2] = ellipse.getCartesianY(input<double>("x"));
			std::cout << "y1: " << y1 << ", y2: " << y2 << std::endl;
		}
		else if (op == 5)
		{
			const auto&& x{ input<double>("x") };
			const auto&& y{ input<double>("y") };
			const auto&& [min, max] = ellipse.getMinMaxDistToFoci(x, y);
			std::cout << "min: " << min << "max: " << max << std::endl;
		}
		else
			std::cerr << "Wrong opt" << std::endl;
	}

	return 0;
}

