// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Utility.hpp"

#include <memory>
#include <map>
#include <algorithm>
#include <vector>

using type_t = int;

signed main()
{
	auto m{ input<size_t>("m") },
		 n{ input<size_t>("n") };
	
	auto matrix{ std::make_unique<std::unique_ptr<type_t[]>[]>(m) }; // CRS? -No.
	std::vector<type_t> vector(m);
	for (size_t i{}; i < m; ++i)
	{
		std::map<type_t, size_t> map;
		matrix[i] = std::make_unique<type_t[]>(n);
		for (size_t j{}; j < n; ++j)
		{
			matrix[i][j] = input<type_t>();

			if (matrix[i][j])
				if (auto&& [it, suc] = map.try_emplace(matrix[i][j], 1); !suc)
					it->second++;
		}

		using pair_t = decltype(map)::value_type;
		auto&& it = std::max_element(std::begin(map), std::end(map), [](const pair_t& p1, const pair_t& p2) { return p1.second < p2.second; });
		vector[i] = it == std::end(map) ? 0 : it->second	;
	}

	std::cout << "Matrix:\n";
	for (size_t i{}; i < m; ++i)
		for (size_t j{}; j < n; ++j)
			std::cout << matrix[i][j] << (j + 1 == n ? "\n" : " ");

	std::cout << "Vector:\n";
	std::copy(std::begin(vector), std::end(vector), std::ostream_iterator<type_t>(std::cout, "\n"));

	return 0;
}