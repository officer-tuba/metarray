#pragma once

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include "metarray.hpp"

namespace metarray {

// --- transformation ----------------------------------------------------------------------------------------------------------------------
//TODO: shouldn't need std::remove_cvref_t here. lower level types should work with or without it.
template <typename StaticArray>
using unwrap_static_array_t = std::remove_cvref_t<decltype(StaticArray::value)>;

template <typename StaticArray, valid_indexer...Idx, std::size_t...Is>
requires ((sizeof...(Idx) <= total_items_v<unwrap_static_array_t<StaticArray>>)
	&& (is_valid_indexer_of_v<unwrap_static_array_t<StaticArray>, Idx> && ...)
	&& sizeof...(Is) == sizeof...(Idx))
constexpr std::array<remove_all_extents_t<unwrap_static_array_t<StaticArray>>, sizeof...(Idx)>
static_transform_to_array(const std::tuple<Idx...>&, const std::index_sequence<Is...>&)
{
	return {{get<Idx>(StaticArray::value)...}};
}

template <typename StaticArray, valid_indexer...Idx>
requires ((sizeof...(Idx) <= total_items_v<unwrap_static_array_t<StaticArray>>)
	&& (is_valid_indexer_of_v<unwrap_static_array_t<StaticArray>, Idx> && ...))
constexpr std::array<remove_all_extents_t<unwrap_static_array_t<StaticArray>>, sizeof...(Idx)>
static_transform_to_array(const std::tuple<Idx...>& idx)
{
	return static_transform_to_array<StaticArray, Idx...>(idx, std::make_index_sequence<sizeof...(Idx)>{});
}

// --- algorithms/numerics -----------------------------------------------------------------------------------------------------------------
template <typename StaticArray, typename Pred, std::size_t I, valid_indexer...Idx>
requires ((I == sizeof...(Idx) - 1) && (is_valid_indexer_of_v<unwrap_static_array_t<StaticArray>, Idx> && ...))
constexpr auto static_find_if(const std::tuple<Idx...>& idx, Pred pred)
{
	using IdxN = std::remove_reference_t<decltype(std::get<I>(idx))>;
	constexpr auto item{get<IdxN>(StaticArray::value)};

	if constexpr (pred(item)) {
		return std::make_tuple(IdxN{});
	}
	else {
		return std::make_tuple();
	}
}

template <typename StaticArray, typename Pred, std::size_t I, valid_indexer...Idx>
requires ((I < sizeof...(Idx) - 1) && (is_valid_indexer_of_v<unwrap_static_array_t<StaticArray>, Idx> && ...))
constexpr auto static_find_if(const std::tuple<Idx...>& idx, Pred pred)
{
	using IdxN = std::remove_reference_t<decltype(std::get<I>(idx))>;
	constexpr auto item{get<IdxN>(StaticArray::value)};

	if constexpr (pred(item)) {
		return std::tuple_cat(std::make_tuple(IdxN{}), static_find_if<StaticArray, Pred, I + 1>(idx, pred));
	}
	else {
		return static_find_if<StaticArray, Pred, I + 1>(idx, pred);
	}
}

template <typename StaticArray, typename Pred>
constexpr auto static_find_if(Pred pred)
{
	return static_find_if<StaticArray, Pred, 0>(indexer_list_of_t<unwrap_static_array_t<StaticArray>>{}, pred);
}

template <typename StaticArray, std::size_t I, std::size_t M, valid_indexer...Idx>
constexpr auto static_find_min(const std::tuple<Idx...>& idx)
{
	using IdxMin = std::remove_reference_t<decltype(std::get<M>(idx))>;
	using IdxN = std::remove_reference_t<decltype(std::get<I>(idx))>;
	constexpr auto item{get<IdxN>(StaticArray::value)};
	constexpr auto min{get<IdxMin>(StaticArray::value)};

	if constexpr (I == sizeof...(Idx) - 1) {
		if constexpr (item < min)
		{
			return IdxN{};
		}
		else {
			return IdxMin{};
		}
	}
	else {
		if constexpr (item < min) {
			return static_find_min<StaticArray, I + 1, I>(idx);
		}
		else {
			return static_find_min<StaticArray, I + 1, M>(idx);
		}
	}
}

template <typename StaticArray>
constexpr auto static_find_min()
{
	return static_find_min<StaticArray, 0, 0>(indexer_list_of_t<unwrap_static_array_t<StaticArray>>{});
}

template <std::size_t K, typename StaticArray, valid_indexer...Idx, valid_indexer...Found>
constexpr auto static_find_k_min(const std::tuple<Idx...>& idx, const std::tuple<Found...>& found)
{
	if constexpr (K == sizeof...(Found) || K == total_items_v<unwrap_static_array_t<StaticArray>>) {
		return found;
	}
	else {
		constexpr auto next_min{static_find_min<StaticArray, 0, 0>(set_diff_t<std::tuple<Idx...>, std::tuple<Found...>>{})};
		return static_find_k_min<K, StaticArray>(idx, std::tuple_cat(found, std::make_tuple(next_min)));
	}
}

template <std::size_t K, typename StaticArray>
constexpr auto static_find_k_min()
{
	return static_find_k_min<K, StaticArray>(indexer_list_of_t<unwrap_static_array_t<StaticArray>>{}, std::make_tuple());
}

template <array A, typename T, typename BinOp, std::size_t I, valid_indexer...Idx>
requires (is_valid_indexer_of_v<A, Idx> && ...)
constexpr T accumulate(const A& a, const std::tuple<Idx...>& idx, T init, BinOp op)
{
	using IdxN = decltype(std::get<I>(idx));
//TODO: is there a way to fold this instead of tail recursion?
	if constexpr (I == sizeof...(Idx) - 1) {
		return op(get<IdxN>(a), init);
	}
	else {
		return op(get<IdxN>(a), accumulate<A, T, BinOp, I + 1>(a, idx, init, op));
	}
}

template <array A, typename T, typename BinOp>
constexpr T accumulate(const A& a, T init, BinOp op)
{
	return accumulate<A, T, BinOp, 0>(a, indexer_list_of_t<A>{}, init, op);
}

template <array A, valid_indexer...Idx>
requires (is_valid_indexer_of_v<A, Idx> && ...)
constexpr auto sum(const A& a, const std::tuple<Idx...>&)
{
	return (get<Idx>(a) + ...);
}

template <array A>
constexpr auto sum(const A& a)
{
	return sum(a, indexer_list_of_t<A>{});
}

template <array A, valid_indexer...Idx>
requires (is_valid_indexer_of_v<A, Idx> && ...)
constexpr auto product(const A& a, const std::tuple<Idx...>&)
{
	return (get<Idx>(a) * ...);
}

template <array A>
constexpr auto product(const A& a)
{
	return product(a, indexer_list_of_t<A>{});
}

}//metarray
