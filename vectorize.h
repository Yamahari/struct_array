#ifndef SOA_VECTORIZE_H
#define SOA_VECTORIZE_H

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace soa
{
	namespace impl
	{
		template <std::size_t N, typename T, typename = void>
		struct bind
			: std::integral_constant<std::size_t, 0U>
		{
		};

		template <typename T>
		struct bind<1U, T, std::void_t<decltype(T{{}})>>
			: std::integral_constant<std::size_t, 1U>
		{
		};

		template <typename T>
		struct bind<2U, T, std::void_t<decltype(T{{}, {}})>>
			: std::integral_constant<std::size_t, 2U>
		{
		};

		template <std::size_t N, typename T>
		inline static constexpr auto bind_v = bind<N, T>::value;

		template <typename T, typename>
		struct max_bind;

		template <typename T, std::size_t ...Is>
		struct max_bind<T, std::index_sequence<Is...>>
			: std::integral_constant<std::size_t, std::max({bind_v<Is, T>...})>
		{
		};
	} // namespace impl

	static constexpr std::size_t max_bindings = 3U;

	template <typename T>
	struct max_bind
		: impl::max_bind<T, decltype(std::make_index_sequence<max_bindings>{})>
	{
	};

	template <typename T>
	inline static constexpr auto max_bind_v = max_bind<T>::value;

	template <std::size_t N, typename T>
	struct to_tuple;

	template <typename T>
	struct to_tuple<0U, T>
	{
		static constexpr auto make = [](auto&& x) noexcept
		{
			return std::make_tuple();
		};

		using type = decltype(make(std::declval<T>()));
	};

	template <typename T>
	struct to_tuple<1U, T>
	{
		static constexpr auto make = []<typename T0>(T0&& x) noexcept
		{
			auto&& [_1] = std::forward<T0>(x);
			return std::make_tuple(std::forward<decltype(_1)>(_1));
		};

		using type = decltype(make(std::declval<T>()));
	};

	template <typename T>
	struct to_tuple<2U, T>
	{
		static constexpr auto make = []<typename T0>(T0&& x) noexcept
		{
			auto&& [_1, _2] = std::forward<T0>(x);
			return std::make_tuple(std::forward<decltype(_1)>(_1),
			                       std::forward<decltype(_2)>(_2));
		};

		using type = decltype(make(std::declval<T>()));
	};

	template <typename T>
	using to_tuple_t = typename to_tuple<max_bind_v<T>, T>::type;

	template <typename T>
	inline static constexpr auto make_to_tuple = to_tuple<max_bind_v<T>, T>::make;

	namespace impl
	{
		template <typename T>
		struct vectorize;

		template <typename ...Ts>
		struct vectorize<std::tuple<Ts...>>
		{
			using type = std::tuple<std::vector<Ts>...>;
			using value_type = std::tuple<typename std::vector<Ts>::value_type...>;
			using allocator_type = std::tuple<typename std::vector<Ts>::allocator_type...>;
			using size_type = std::tuple<typename std::vector<Ts>::size_type...>;
			using difference_type = std::tuple<typename std::vector<Ts>::difference_type...>;
			using reference = std::tuple<typename std::vector<Ts>::reference...>;
			using const_reference = std::tuple<typename std::vector<Ts>::const_reference...>;
			using pointer = std::tuple<typename std::vector<Ts>::pointer...>;
			using const_pointer = std::tuple<typename std::vector<Ts>::const_pointer...>;
			using iterator = std::tuple<typename std::vector<Ts>::iterator...>;
			using const_iterator = std::tuple<typename std::vector<Ts>::const_iterator...>;
			using reverse_iterator = std::tuple<typename std::vector<Ts>::reverse_iterator...>;
			using const_reverse_iterator = std::tuple<typename std::vector<Ts>::const_reverse_iterator...>;
		};
	} // namespace impl

	template <typename T>
	struct vectorize final
		: impl::vectorize<to_tuple_t<T>>
	{
	};

	template <typename T>
	using vectorize_t = typename vectorize<T>::type;
}

#endif // SOA_VECTORIZE_H
