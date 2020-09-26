#ifndef SOA_TO_TUPLE_H
#define SOA_TO_TUPLE_H

#include <cstddef>
#include <tuple>
#include <utility>

#include <boost/preprocessor.hpp>

#include "bind.h"

namespace soa
{
namespace impl
{
template <std::size_t N, typename T>
struct to_tuple_impl;

template <typename T>
struct to_tuple_impl<0U, T>
{
	static constexpr auto make = [](auto &&) noexcept
	{
		return std::make_tuple();
	};

	using type = decltype(make(std::declval<T>()));
};

#define SOA_IDENTIFIER(z, i, text) BOOST_PP_COMMA_IF(i) BOOST_PP_CAT(text, i)
#define SOA_IDENTIFIER_LIST(n) BOOST_PP_REPEAT(n, SOA_IDENTIFIER, _)
#define SOA_FORWARD(z, data, text) std::forward<decltype(text)>(text)
#define SOA_FORWARD_IDENTIFIER_LIST(...)      \
  BOOST_PP_LIST_ENUM(BOOST_PP_LIST_TRANSFORM( \
      SOA_FORWARD, 0, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)))
#define SOA_TO_TUPLE_IMPL(i, ...)                                       \
  template <typename T>                                                 \
  struct to_tuple_impl<i, T> {                                          \
    static constexpr auto make = [](auto &&x) noexcept {                \
      auto &&[__VA_ARGS__] = std::forward<decltype(x)>(x);              \
      return std::make_tuple(SOA_FORWARD_IDENTIFIER_LIST(__VA_ARGS__)); \
    };                                                                  \
    using type = decltype(make(std::declval<T>()));                     \
  };

#define SOA_TO_TUPLE(z, i, data) SOA_TO_TUPLE_IMPL(i, SOA_IDENTIFIER_LIST(i))

BOOST_PP_REPEAT_FROM_TO(1, SOA_MAX_BINDINGS, SOA_TO_TUPLE, 0)

#undef SOA_TO_TUPLE
#undef SOA_TO_TUPLE_IMPL
#undef SOA_FORWARD_IDENTIFIER_LIST
#undef SOA_IDENTIFIER_LIST
#undef SOA_IDENTIFIER
} // namespace impl

template <typename T>
using to_tuple = impl::to_tuple_impl<max_bind_v<T>, T>;

template <typename T>
using to_tuple_t = typename to_tuple<T>::type;

template <typename T>
inline static constexpr auto make_to_tuple = to_tuple<T>::make;
} // namespace soa

namespace soa
{
namespace impl
{
template <typename T, typename Tuple, std::size_t... I>
constexpr T make_from_tuple_impl(Tuple &&t, std::index_sequence<I...>)
{
	return T{std::get<I>(std::forward<Tuple>(t))...};
}
} // namespace impl

template <typename T, typename Tuple>
constexpr T make_from_tuple(Tuple &&t)
{
	return impl::make_from_tuple_impl<T>(
		std::forward<Tuple>(t),
		std::make_index_sequence<
			std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}
} // namespace soa

#endif // SOA_TO_TUPLE_H
