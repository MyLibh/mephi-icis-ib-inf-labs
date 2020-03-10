// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "generator.hpp"

std::mt19937 generator::gen = std::mt19937(std::random_device()());

void generator::seed(const unsigned long seed /* = std::random_device()() */) noexcept
{
    generator::gen = std::mt19937(seed);
}

[[nodiscard]]
int generator::get_int(uid_t dis) noexcept
{
    return dis(generator::gen);
}

[[nodiscard]]
int generator::get_int(const int left, const int right) noexcept
{
    return generator::get_int(uid_t{ left, right });
}

[[nodiscard]]
char generator::get_char() noexcept
{
    return generator::ASCII[static_cast<size_t>(generator::get_int(uid_t{ 0, static_cast<int>(generator::ASCII.length() - 1) }))];
}

[[nodiscard]]
std::string generator::get_str(const size_t length) noexcept
{
    std::string res(length, '\0');
    std::for_each(std::begin(res), std::end(res), [](auto& c) { c = generator::get_char(); });

    return res;
}