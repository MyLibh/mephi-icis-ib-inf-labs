#ifndef __GENERATOR_HPP_INCLUDED__
#define __GENERATOR_HPP_INCLUDED__

#include <random>
#include <algorithm>

namespace util
{
    class generator final
    {
    public:
        static constexpr std::string_view ASCII = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^abcdefghijklmnopqrstuvwxyz0123456789{|}~";

    public:
        inline static void seed(const unsigned long seed = std::random_device()()) { generator::gen.seed(seed); }

        template<typename T>
        [[nodiscard]]
        inline static constexpr T get(const T left, const T right) noexcept
        {
            static_assert(std::is_arithmetic_v<T>, "T must be arithmetic");

            if constexpr (std::is_integral_v<T>)
                return std::uniform_int_distribution<T>{ left, right }(generator::gen);
            else if constexpr (std::is_floating_point_v<T>)
                return std::uniform_real_distribution<T>{ left, right }(generator::gen);
        }

        [[nodiscard]]
        inline static constexpr char get_char() noexcept { return generator::ASCII[generator::get(std::size_t{ 0 }, std::size(generator::ASCII) - 1)]; }

        template<typename Pred>
        [[nodiscard]]
        static std::string get_str(const std::size_t length, Pred&& pred = [](auto& c) { c = generator::get_char(); }) noexcept
        {
            std::string res(length, '\0');
            std::transform(std::begin(res), std::end(res), pred);

            return res;
        }

    private:
        inline static std::mt19937 gen = std::mt19937(std::random_device()());
    };
} // namespace util

#endif /* !__GENERATOR_HPP_INCLUDED__ */