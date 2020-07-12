#ifndef SOA_STRUCT_ARRAY_H
#define SOA_STRUCT_ARRAY_H

#include <cstddef>
#include <functional>
#include <tuple>
#include <utility>

#include "tuple_wrapper.h"
#include "vectorize.h"

namespace soa
{
	namespace impl
	{
		template <typename T, typename>
		struct struct_array_impl;

		template <typename T, std::size_t... Is>
		struct struct_array_impl<T, std::index_sequence<Is...>>
		{
			using value_type = typename vectorize<T>::value_type;
			using size_type = typename vectorize<T>::size_type;
			using difference_type = typename vectorize<T>::difference_type;
			using reference = typename vectorize<T>::reference;
			using const_reference = typename vectorize<T>::const_reference;
			using pointer = typename vectorize<T>::pointer;
			using const_pointer = typename vectorize<T>::const_pointer;

			template <typename U>
			struct struct_array_iterator
			{
				using value_type = tuple_wrapper<typename vectorize<T>::value_type>;
				using difference_type = std::ptrdiff_t;
				using reference = tuple_wrapper<typename vectorize<T>::reference>;
				using pointer = typename vectorize<T>::pointer;
				using iterator_category = std::random_access_iterator_tag;

				U iterators;

				struct_array_iterator() = default;
				struct_array_iterator(const struct_array_iterator&) = default;
				struct_array_iterator(struct_array_iterator&&) = default;

				template <typename...Args>
					requires (sizeof...(Is) == sizeof...(Args)) && (... && std::is_constructible_v<std::tuple_element_t<Is, U>, Args>)
				struct_array_iterator(Args&&...args)
					: iterators{std::forward<Args>(args)...}
				{
				}

				template <typename V>
					requires (... && std::is_constructible_v<std::tuple_element_t<Is, U>, std::tuple_element_t<Is, V>>)
				struct_array_iterator(const struct_array_iterator<V>& that)
					: iterators{std::get<Is>(that.iterators)...}
				{
				}

				~struct_array_iterator() = default;

				auto operator=(const struct_array_iterator&) -> struct_array_iterator& = default;
				auto operator=(struct_array_iterator&&) -> struct_array_iterator& = default;

				template <typename V>
				requires (... && std::is_constructible_v<std::tuple_element_t<Is, U>, std::tuple_element_t<Is, V>>)
				auto operator=(const struct_array_iterator<V>& that) noexcept -> struct_array_iterator&
				{
					return *this = struct_array_iterator{that};
				}

				[[nodiscard]]
				auto operator-(const difference_type n) const noexcept -> struct_array_iterator
				{
					return {(std::get<Is>(iterators) - n) ...};
				}

				[[nodiscard]]
				auto operator-(const struct_array_iterator& that) const noexcept -> difference_type
				{
					return std::get<0>(iterators) - std::get<0>(that.iterators);
				}

				[[nodiscard]]
				auto operator+=(const difference_type n) noexcept -> struct_array_iterator&
				{
					return *this = (*this + n);
				}

				[[nodiscard]]
				auto operator-=(const difference_type n) noexcept -> struct_array_iterator&
				{
					return *this = (*this - n);
				}

				auto operator++() noexcept -> struct_array_iterator&
				{
					(..., ++std::get<Is>(iterators));
					return *this;
				}

				[[nodiscard]]
				auto operator++(int) const noexcept -> struct_array_iterator
				{
					return ++struct_array_iterator{*this};
				}

				auto operator--() noexcept -> struct_array_iterator&
				{
					(..., --std::get<Is>(iterators));
					return *this;
				}

				[[nodiscard]]
				auto operator--(int) -> struct_array_iterator
				{
					return --struct_array_iterator{*this};
				}

				auto operator*() noexcept -> reference
				{
					return {std::make_tuple(std::ref(*(std::get<Is>(iterators)))...)};
				}

				auto operator->() noexcept -> pointer
				{
					return {((std::get<Is>(iterators)).operator->())...};
				}

				auto operator[](const difference_type n) noexcept -> reference
				{
					return {*(std::get<Is>(iterators) + n)...};
				}

				/* TODO Fix noexcept specification */
				void swap(struct_array_iterator& that) noexcept 
				{
					(..., swap(std::get<Is>(iterators), std::get<Is>(that.iterators)));
				}

				[[nodiscard]]
				bool operator==(const struct_array_iterator& that) const noexcept
				{
					return *this - that == 0;
				}

				[[nodiscard]]
				bool operator!=(const struct_array_iterator& that) const noexcept
				{
					return !(*this == that);
				}

				[[nodiscard]]
				bool operator<(const struct_array_iterator& that) const noexcept
				{
					return *this - that > 0;
				}

				[[nodiscard]]
				bool operator>(const struct_array_iterator& that) const noexcept
				{
					return that < *this;
				}

				[[nodiscard]]
				bool operator<=(const struct_array_iterator& that) const noexcept
				{
					return !(*this > that);
				}

				[[nodiscard]]
				bool operator>=(const struct_array_iterator& that) const noexcept
				{
					return !(*this < that);
				}

				// friend hack to allow definition inside nested class
				friend auto operator+(const struct_array_iterator& it,
				                      const difference_type n) noexcept -> struct_array_iterator
				{
					return {(std::get<Is>(it.iterators) + n)...};
				}

				// friend hack to allow definition inside nested class
				friend auto operator+(const difference_type n,
				                      const struct_array_iterator& it) noexcept -> struct_array_iterator
				{
					return it + n;
				}
			};

			using iterator = struct_array_iterator<typename soa::vectorize<T>::iterator>;
			using const_iterator = struct_array_iterator<typename soa::vectorize<T>::const_iterator>;
			using reverse_iterator = struct_array_iterator<typename soa::vectorize<T>::reverse_iterator>;
			using const_reverse_iterator = struct_array_iterator<typename soa::vectorize<T>::const_reverse_iterator>;

			vectorize_t<T> components;

			auto operator[](const std::size_t pos) -> reference
			{
				return *(begin() + pos);
			}

			auto operator[](const std::size_t pos) const -> const_reference
			{
				return *(cbegin() + pos);
			}

			auto front() -> reference
			{
				return *begin();
			}

			[[nodiscard]]
			auto front() const -> const_reference
			{
				return *cbegin();
			}

			auto back() -> reference
			{
				return *(end() - 1);
			}

			[[nodiscard]]
			auto back() const -> const_reference
			{
				return *(cend() - 1);
			}

			auto data() noexcept -> pointer
			{
				return {std::data(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto data() const noexcept -> pointer
			{
				return {std::data(std::get<Is>(components))...};
			}

			auto begin() noexcept -> iterator
			{
				return {std::begin(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto begin() const noexcept -> iterator
			{
				return {std::begin(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto cbegin() const noexcept -> const_iterator
			{
				return {std::cbegin(std::get<Is>(components))...};
			}

			auto end() noexcept -> iterator
			{
				return {std::end(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto end() const noexcept -> iterator
			{
				return {std::end(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto cend() const noexcept -> const_iterator
			{
				return {std::cend(std::get<Is>(components))...};
			}

			auto rbegin() noexcept -> reverse_iterator
			{
				return {std::rbegin(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto rbegin() const noexcept -> reverse_iterator
			{
				return {std::rbegin(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto crbegin() const noexcept -> const_reverse_iterator
			{
				return {std::crbegin(std::get<Is>(components))...};
			}

			auto rend() noexcept -> reverse_iterator
			{
				return {std::rend(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto rend() const noexcept -> reverse_iterator
			{
				return {std::rend(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto crend() const noexcept -> const_reverse_iterator
			{
				return {std::crend(std::get<Is>(components))...};
			}

			[[nodiscard]]
			bool empty() const noexcept
			{
				return (... && std::empty(std::get<Is>(components)));
			}

			[[nodiscard]]
			auto size() const noexcept -> size_type
			{
				return {std::size(std::get<Is>(components))...};
			}

			[[nodiscard]]
			auto max_size() const noexcept -> size_type
			{
				return {(std::get<Is>(components).max_size())...};
			}

			void reserve(const std::size_t new_cap)
			{
				(..., std::get<Is>(components).reserve(new_cap));
			}

			[[nodiscard]]
			auto capacity() const noexcept -> size_type
			{
				return {(std::get<Is>(components).capacity())...};
			}

			void shrink_to_fit()
			{
				(..., std::get<Is>(components).shrink_to_fit());
			}

			void clear() noexcept
			{
				(..., std::get<Is>(components).clear());
			}

			template <typename U>
				requires std::is_same_v<value_type, std::decay_t<U>>
			auto insert(const const_iterator pos, U&& value) -> iterator
			{
				return {
					std::get<Is>(components).insert(std::get<Is>(pos.iterators),
					                                std::forward<decltype(std::get<Is>(value))>(std::get<Is>(value)))...
				};
			}

			auto insert(const const_iterator pos, const size_type count, const value_type& value) -> iterator
			{
				return {std::get<Is>(components).insert(std::get<Is>(pos.iterators), count, std::get<Is>(value))...};
			}

			template <typename U>
				requires std::is_same_v<T, std::decay_t<U>>
			auto insert(const const_iterator pos, U&& value) -> iterator
			{
				return insert(pos, make_to_tuple<T>(std::forward<decltype(value)>(value)));
			}

		private:
			template <typename U>
				requires(std::tuple_size_v<std::decay_t<U>> == sizeof...(Is))
			auto emplace_impl(const const_iterator pos, U&& u) -> iterator
			{
				return {
					std::apply(
						[&](auto&&...xs)
						{
							return std::get<Is>(components).emplace(std::get<Is>(pos.iterators),
							                                        std::forward<decltype(xs)>(xs)...);
						}, std::forward<decltype(std::get<Is>(u))>(std::get<Is>(u)))
					...
				};
			}

		public:
			template <typename ...Args>
				requires (sizeof...(Is) == sizeof...(Args))
			auto emplace(const const_iterator pos, Args&&...args) -> iterator
			{
				return emplace_impl(pos, std::forward_as_tuple(std::forward<Args>(args)...));
			}

			auto erase(const const_iterator pos) -> iterator
			{
				return {std::get<Is>(components).erase(std::get<Is>(pos))...};
			}

			auto erase(const const_iterator first, const const_iterator last) -> iterator
			{
				return {std::get<Is>(components).erase(std::get<Is>(first), std::get<Is>(last))...};
			}

			void push_back(const value_type& value)
			{
				(..., std::get<Is>(components).push_back(std::get<Is>(value)));
			}

			void push_back(value_type&& value)
			{
				(..., std::get<Is>(components).push_back(std::move(std::get<Is>(value))));
			}

			template <typename U>
				requires std::is_same_v<T, std::decay_t<U>>
			void push_back(U&& value)
			{
				push_back(std::move(make_to_tuple<T>(std::forward<decltype(value)>(value))));
			}

		private:
			template <typename U>
				requires(std::tuple_size_v<std::decay_t<U>> == sizeof...(Is))
			auto emplace_back_impl(U&& u) -> reference
			{
				return {
					std::apply(
						[&](auto&&...xs)
						{
							return std::ref(std::get<Is>(components).emplace_back(std::forward<decltype(xs)>(xs)...));
						}, std::forward<decltype(std::get<Is>(u))>(std::get<Is>(u)))
					...
				};
			}

		public:
			template <typename...Args>
				requires (sizeof...(Is) == sizeof...(Args))
			auto emplace_back(Args&&... args) -> reference
			{
				return emplace_back_impl(std::forward_as_tuple(std::forward<Args>(args)...));
			}

			void pop_back()
			{
				(..., std::get<Is>(components).pop_back());
			}

			void resize(const std::size_t count)
			{
				(..., std::get<Is>(components).resize(count));
			}

			void resize(const std::size_t count, const value_type& value)
			{
				(..., std::get<Is>(components).resize(count, std::get<Is>(value)));
			}

			// TODO fix noexcept specifier
			void swap(struct_array_impl& other) noexcept
			{
				(..., std::get<Is>(components).swap(std::get<Is>(other.components)));
			}
		};
	} // namespace impl

	template<typename T>
	using struct_array = impl::struct_array_impl<T, decltype(std::make_index_sequence<std::tuple_size_v<to_tuple_t<T>>>{})>;

	template<typename T>
	void swap(struct_array<T>& lhs, struct_array<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}
} // namespace soa

#endif // SOA_STRUCT_ARRAY_H
