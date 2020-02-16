#pragma once

#include <utility>
#include <type_traits>
#include <iosfwd>

template<typename _T, typename = std::void_t<>>
struct has_operator_out : std::false_type {};

template<typename _T>
struct has_operator_out<_T, std::void_t<decltype(std::declval<std::ostream>() << std::declval<_T>())>> : std::true_type {};

template <typename _T>
constexpr static bool has_operator_out_v = has_operator_out<_T>::value;

template<typename _T, typename = std::void_t<>>
struct has_operator_in : std::false_type {};

template<typename _T>
struct has_operator_in<_T, std::void_t<decltype(std::declval<std::istream>() >> std::declval<_T>())>> : std::true_type {};

template <typename _T>
constexpr static bool has_operator_in_v = has_operator_out<_T>::value;