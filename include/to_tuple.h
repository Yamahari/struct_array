#ifndef SOA_TO_TUPLE_H
#define SOA_TO_TUPLE_H

#include <cstddef>
#include <tuple>
#include <utility>

#include "bind.h"

namespace soa 
{
    namespace impl {
        template <std::size_t N, typename T>
        struct to_tuple_impl;

        template <typename T>
        struct to_tuple_impl<0U, T>
        {
            static constexpr auto make = [](auto&& x) noexcept
            {
                return std::make_tuple();
            };

            using type = decltype(make(std::declval<T>()));
        };

        template <typename T>
        struct to_tuple_impl<1U, T>
        {
            static constexpr auto make = [](auto&& x) noexcept
            {
                auto&& [_1] = std::forward<decltype(x)>(x);
                return std::make_tuple(std::forward<decltype(_1)>(_1));
            };

            using type = decltype(make(std::declval<T>()));
        };

        template <typename T>
        struct to_tuple_impl<2U, T>
        {
            static constexpr auto make = [](auto&& x) noexcept
            {
                auto&& [_1, _2] = std::forward<decltype(x)>(x);
                return std::make_tuple(std::forward<decltype(_1)>(_1),
                                    std::forward<decltype(_2)>(_2));
            };

            using type = decltype(make(std::declval<T>()));
        };
    } // namespace impl

    template<typename T>
    using to_tuple = impl::to_tuple_impl<max_bind_v<T>, T>;

	template <typename T>
	using to_tuple_t = typename to_tuple<T>::type;

	template <typename T>
	inline static constexpr auto make_to_tuple = to_tuple<T>::make;
} // namespace soa

#endif // SOA_TO_TUPLE_H