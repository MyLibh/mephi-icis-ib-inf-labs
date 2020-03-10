#pragma once

#include <random> 

class generator final
{
public:
    using uid_t = std::uniform_int_distribution<>;

public:
    static constexpr std::string_view ASCII = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^abcdefghijklmnopqrstuvwxyz0123456789{|}~";

public:
    static void seed(const unsigned long seed = std::random_device()()) noexcept;

    [[nodiscard]]
    static int get_int(uid_t dis) noexcept;

    [[nodiscard]]
    static int get_int(const int left, const int right) noexcept;

    [[nodiscard]]
    static char get_char() noexcept;

    [[nodiscard]]
    static std::string get_str(const size_t length) noexcept;

private:
    static std::mt19937 gen;
};