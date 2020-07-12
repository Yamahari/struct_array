#ifndef SOA_VECTORIZE_H
#define SOA_VECTORIZE_H

#include <tuple>
#include <vector>

#include "to_tuple.h"

namespace soa
{
	namespace impl
	{
		template <typename T>
		struct vectorize_impl;

		template <typename... Ts>
		struct vectorize_impl<std::tuple<Ts...>>
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
	using vectorize = impl::vectorize_impl<to_tuple_t<T>>;

	template <typename T>
	using vectorize_t = typename vectorize<T>::type;
} // namespace soa

#endif // SOA_VECTORIZE_H
