#pragma once

#include <array>
#include <concepts>
#include <numeric>
#include <tuple>
#include <type_traits>

namespace array_meta {
// --- std::array traits/concepts ----------------------------------------------------------------------------------------------------------
template <typename>
struct is_std_array : std::false_type{};

template <typename T, std::size_t S>
struct is_std_array<std::array<T, S>> : std::true_type{};

template <typename T>
inline constexpr auto is_std_array_v{is_std_array<T>::value};

template <typename T>
concept std_array = is_std_array_v<std::remove_cv_t<T>>;


// --- std::array ranks --------------------------------------------------------------------------------------------------------------------
template <typename T>
struct rank : std::integral_constant<std::size_t, 0>{};

template <std_array T>
struct rank<T> : std::integral_constant<std::size_t, rank<typename T::value_type>::value + 1>{};

template <typename T>
inline constexpr auto rank_v{rank<T>::value};


// --- std::array extents ------------------------------------------------------------------------------------------------------------------
template <typename T, std::size_t I = 0>
struct extent : std::integral_constant<std::size_t, 0>{};

template <std_array T>
struct extent<T, 0> : std::integral_constant<std::size_t, std::tuple_size_v<T>>{};

template <std_array T, std::size_t I>
struct extent<T, I> : extent<typename T::value_type, I - 1>{};

template <typename T, std::size_t I>
inline constexpr auto extent_v{extent<T, I>::value};

template <typename T>
struct remove_all_extents {
	using type = T;
};
 
template <std_array T>
struct remove_all_extents<T> {
	using type = typename remove_all_extents<typename T::value_type>::type;
};
 
template <std_array T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

template <typename...>
struct total_items;

template <typename T>
struct total_items<T> {
	inline static constexpr std::size_t value = 1;
};

template <typename T>
requires std_array<T>
struct total_items<T> {
	inline static constexpr auto value = std::tuple_size_v<T> * total_items<typename T::value_type>::value;
};

// --- type aliases ------------------------------------------------------------------------------------------------------------------------
template <std::size_t S>
using indexer_t = std::array<std::size_t, S>;


// --- internal use ------------------------------------------------------------------------------------------------------------------------
namespace detail {
template <std::size_t I, std_array T>
constexpr void fill_extents(const T& arr, indexer_t<rank_v<T>>& ranks)
{
	ranks[I] = extent_v<T, I>;

	if constexpr (I > 0) {
		fill_extents<I - 1>(arr, ranks);
	}
}

template <std::size_t S, std::size_t E>
constexpr std::size_t extent_offset_of(const indexer_t<S>& indexer, const indexer_t<S>& extents)
{
	if constexpr (E == 0) {
		return indexer[0];
	}
	else {
		return indexer[E] + (extents[E] * extent_offset_of<S, E - 1>(indexer, extents));
	}
}

template <std::size_t S, std::size_t E = 0>
constexpr std::size_t extent_index_of(std::size_t offset, const indexer_t<S>& extents)
{
	static_assert(E < S);

	for (std::size_t ext_idx{E + 1}; ext_idx < S; ++ext_idx) {
		offset /= extents[ext_idx];
	}

	return offset % extents[E];
}

template <std::size_t S, std::size_t I = S - 1>
constexpr void fill_indexer(std::size_t offset, indexer_t<S>& indexer, const indexer_t<S>& extents)
{
	indexer[I] = extent_index_of<S, I>(offset, extents);

	if constexpr (I > 0) {
		fill_indexer<S, I - 1>(offset, indexer, extents);
	}
}

template <std_array P, std::size_t I, std_array T>
constexpr auto at(T& arr, const indexer_t<rank_v<P>>& indexer)
{
	if constexpr (rank_v<T> == 1) {
		return arr.at(indexer[I]);
	}
	else {
		return at<P, I + 1>(arr.at(indexer[I]), indexer);
	}
}
}//detail


// --- "public" interface ------------------------------------------------------------------------------------------------------------------
template <std_array T>
constexpr auto extents_of(const T& arr)
{
	indexer_t<rank_v<T>> ranks;
	detail::fill_extents<ranks.size() - 1>(arr, ranks);
	return ranks;
}

template <typename T>
requires std_array<T>
constexpr auto indexer()
{
	return indexer_t<rank_v<T>>{0};
}

template <std::size_t S>
constexpr std::size_t offset_of(const indexer_t<S>& indexer, const indexer_t<S>& extents)
{
	return detail::extent_offset_of<S, S - 1>(indexer, extents);
}

template <std::size_t S>
constexpr auto indexer_of(std::size_t offset, const indexer_t<S>& extents)
{
	indexer_t<S> indexer;
	detail::fill_indexer(offset, indexer, extents);
	return indexer;
}

template <std::size_t S>
constexpr auto next_index(const indexer_t<S>& indexer, const indexer_t<S>& extents, std::size_t offset = 1)
{// works like incrementing a value by offset, but each index ("digit") is a different base (as specified by extents).
//TODO: doesn't handle wrapping yet.
	bool carry;
	auto next{indexer};

	do {
		std::size_t extent{extents.size() - 1};
		do {
			carry = false;

			if (++next[extent] == extents[extent]) {
				next[extent] = 0;

				if (extent > 0) {
					--extent;
				}
				else {
					return next;
				}

				carry = true;
			}
		} while(carry);
	} while (--offset > 0);

	return next;
}

template <std::size_t S>
constexpr auto next_stride(const indexer_t<S>& indexer, const indexer_t<S>& extents)
{
	return next_index(indexer, extents, std::get<S - 1>(extents));
}

template <std_array T>
inline constexpr auto total_items_v = total_items<T>::value;

template <std_array T>
constexpr auto at(const T& arr, std::size_t offset)
{
	const auto indexer{indexer_of(offset, extents_of(arr))};
	return detail::at<T, 0>(arr, indexer);
}

template <std_array T>
constexpr auto at(T& arr, std::size_t offset)
{
	const auto indexer{indexer_of(offset, extents_of(arr))};
	return detail::at<T, 0>(arr, indexer);
}

template <std_array T>
constexpr auto at(const T& arr, const indexer_t<rank_v<T>>& indexer)
{
	return detail::at<T, 0>(arr, indexer);
}

template <std_array T>
constexpr auto at(T& arr, const indexer_t<rank_v<T>>& indexer)
{
	return detail::at<T, 0>(arr, indexer);
}

}
