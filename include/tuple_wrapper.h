#ifndef SOA_TUPLE_WRAPPER_H
#define SOA_TUPLE_WRAPPER_H

// Helpful https://stackoverflow.com/questions/21588477/swap-temporary-tuples-of-references

#include <tuple>
#include <type_traits>

namespace soa 
{
    template<typename T>
    struct tuple_wrapper
    {
        T tuple;

        tuple_wrapper() = default;

        template<typename U>
            requires std::is_constructible_v<T, std::add_const_t<std::add_lvalue_reference_t<U>>>
        tuple_wrapper(const tuple_wrapper<U>& that)
            : tuple{that.tuple}
        {}

        template<typename U>
            requires std::is_constructible_v<T, std::add_rvalue_reference_t<U>>
        tuple_wrapper(tuple_wrapper<U>&& that)
            : tuple{std::move(that.tuple)}
        {}

        template<typename U>
            requires std::is_constructible_v<T, U>
        tuple_wrapper(U&& tuple)
            : tuple{std::forward<U>(tuple)}
        {}

        template<typename U>
            requires std::is_assignable_v<std::add_lvalue_reference_t<T>, std::add_const_t<std::add_lvalue_reference_t<U>>>
        auto operator=(const tuple_wrapper<U>& that) -> tuple_wrapper&
        {
            tuple = that.tuple;
            return *this;
        }

        template<typename U>
            requires std::is_assignable_v<std::add_lvalue_reference_t<T>, std::add_rvalue_reference_t<U>>
        auto operator=(tuple_wrapper<U>&& that) -> tuple_wrapper&
        {
            tuple = std::move(that.tuple);
            return *this;
        }

        template<typename U>
            requires std::is_assignable_v<std::add_lvalue_reference<T>, U>
        auto operator=(U&& tuple) -> tuple_wrapper&
        {
            this->tuple = std::forward<U>(tuple);
            return *this;
        }

        operator T() const noexcept
        {
            return tuple;
        }
    };
} // namespace soa

namespace std 
{
    template<typename T>
    struct tuple_size<soa::tuple_wrapper<T>>
        : std::tuple_size<T>
    {};
    
    template<std::size_t I, typename T>
    struct tuple_element<I, soa::tuple_wrapper<T>>
        : std::tuple_element<I, T>
    {};
} // namespace std

namespace soa
{
    template<std::size_t I, typename T>
    std::tuple_element_t<I, soa::tuple_wrapper<T>>& get(soa::tuple_wrapper<T>& wrapper) noexcept 
    {
        return std::get<I>(wrapper.tuple);
    }

    template<std::size_t I, typename T>
    std::tuple_element_t<I, soa::tuple_wrapper<T>>&& get(soa::tuple_wrapper<T>&& wrapper) noexcept 
    {
        return std::get<I>(wrapper.tuple);
    }

    template<std::size_t I, typename T>
    const std::tuple_element_t<I, soa::tuple_wrapper<T>>& get(const soa::tuple_wrapper<T>& wrapper) noexcept 
    {
        return std::get<I>(wrapper.tuple);
    }

    template<std::size_t I, typename T>
    const std::tuple_element_t<I, soa::tuple_wrapper<T>>&& get(const soa::tuple_wrapper<T>&& wrapper) noexcept 
    {
        return std::get<I>(wrapper.tuple);
    }

    template<typename T>
        requires std::is_swappable_v<T>
    void swap(soa::tuple_wrapper<T> lhs, soa::tuple_wrapper<T> rhs) noexcept(std::is_nothrow_swappable_v<T>)
    {
        using std::swap;
        swap(lhs.tuple, rhs.tuple);
    }
} // namespace soa

#endif // SOA_TUPLE_WRAPPER_H