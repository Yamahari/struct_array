#ifndef SOA_BIND_H
#define SOA_BIND_H

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

#include <boost/preprocessor.hpp>

#ifndef SOA_MAX_BINDINGS
#define SOA_MAX_BINDINGS 9
#endif

namespace soa
{
namespace impl
{
template <std::size_t N, typename T, typename = void>
struct bind_impl final : std::integral_constant<std::size_t, 0U>
{
};

#define SOA_BIND_INITIALIZER(z, n, text) BOOST_PP_COMMA_IF(n) text
#define SOA_BIND_INITIALIZER_LIST(n) \
  BOOST_PP_REPEAT(n, SOA_BIND_INITIALIZER, {})
#define SOA_BIND_IMPL(i, ...)                                    \
  template <typename T>                                          \
  struct bind_impl<i, T, std::void_t<decltype(T{__VA_ARGS__})>> final \
      : std::integral_constant<std::size_t, i> {};
#define SOA_BIND(z, i, data) SOA_BIND_IMPL(i, SOA_BIND_INITIALIZER_LIST(i))

BOOST_PP_REPEAT_FROM_TO(1, SOA_MAX_BINDINGS, SOA_BIND, 0)

#undef SOA_BIND
#undef SOA_BIND_IMPL
#undef SOA_BIND_INITIALIZER_LIST
#undef SOA_BIND_INITIALIZER

template <std::size_t N, typename T>
inline static constexpr auto bind_v = bind_impl<N, T>::value;

template <typename T, typename>
struct max_bind_impl;

template <typename T, std::size_t... Is>
struct max_bind_impl<T, std::index_sequence<Is...>>
	: std::integral_constant<std::size_t, std::max({bind_v<Is, T>...})>
{
};
} // namespace impl

template <typename T>
using max_bind =
impl::max_bind_impl<T, decltype(std::make_index_sequence<SOA_MAX_BINDINGS>{})>;

template <typename T>
inline static constexpr auto max_bind_v = max_bind<T>::value;
} // namespace soa

#endif // SOA_BIND_H
