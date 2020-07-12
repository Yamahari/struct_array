#ifndef SOA_BIND_H
#define SOA_BIND_H

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace soa
{
    namespace impl
	{
		template <std::size_t N, typename T, typename = void>
		struct bind_impl
			: std::integral_constant<std::size_t, 0U>
		{
		};

		template <typename T>
		struct bind_impl<1U, T, std::void_t<decltype(T{{}})>>
			: std::integral_constant<std::size_t, 1U>
		{
		};

		template <typename T>
		struct bind_impl<2U, T, std::void_t<decltype(T{{}, {}})>>
			: std::integral_constant<std::size_t, 2U>
		{
		};

        template <typename T>
		struct bind_impl<3U, T, std::void_t<decltype(T{{}, {}, {}})>>
			: std::integral_constant<std::size_t, 3U>
		{
		};

        template <typename T>
		struct bind_impl<4U, T, std::void_t<decltype(T{{}, {}, {}, {}})>>
			: std::integral_constant<std::size_t, 4U>
		{
		};

        template <typename T>
		struct bind_impl<5U, T, std::void_t<decltype(T{{}, {}, {}, {}, {}})>>
			: std::integral_constant<std::size_t, 5U>
		{
		};

        template <typename T>
		struct bind_impl<6U, T, std::void_t<decltype(T{{}, {}, {}, {}, {}, {}})>>
			: std::integral_constant<std::size_t, 6U>
		{
		};

        template <typename T>
		struct bind_impl<7U, T, std::void_t<decltype(T{{}, {}, {}, {}, {}, {}, {}})>>
			: std::integral_constant<std::size_t, 7U>
		{
		};

        template <typename T>
		struct bind_impl<8U, T, std::void_t<decltype(T{{}, {}, {}, {}, {}, {}, {}, {}, {}})>>
			: std::integral_constant<std::size_t, 8U>
		{
		};

		template <std::size_t N, typename T>
		inline static constexpr auto bind_v = bind_impl<N, T>::value;

		template <typename T, typename>
		struct max_bind_impl;

		template <typename T, std::size_t ...Is>
		struct max_bind_impl<T, std::index_sequence<Is...>>
			: std::integral_constant<std::size_t, std::max({bind_v<Is, T>...})>
		{
		};
	} // namespace impl

	static constexpr std::size_t max_bindings = 9U;

	template<typename T>
	using max_bind = impl::max_bind_impl<T, decltype(std::make_index_sequence<max_bindings>{})>;

	template <typename T>
	inline static constexpr auto max_bind_v = max_bind<T>::value;
} // namespace soa

#endif // SOA_BIND_H