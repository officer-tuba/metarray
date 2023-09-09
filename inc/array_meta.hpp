#pragma once

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace array_meta {

// --- array traits/concepts ---------------------------------------------------------------------------------------------------------------
template <typename T>
concept c_array = std::is_array_v<T>;

template <typename>
struct is_std_array : std::false_type{};

template <typename T, std::size_t S>
struct is_std_array<std::array<T, S>> : std::true_type{};

template <typename T>
inline constexpr auto is_std_array_v{is_std_array<std::remove_cv_t<T>>::value};

template <typename T>
concept std_array = is_std_array_v<T>;

template <typename>
struct is_array : std::false_type{};

template <c_array T>
struct is_array<T> : std::true_type{};

template <typename T, std::size_t S>
struct is_array<std::array<T, S>> : std::true_type{};

template <typename T>
inline constexpr auto is_array_v{is_array<std::remove_cv_t<T>>::value};

template <typename T>
concept array = is_array_v<T>;

// --- array ranks -------------------------------------------------------------------------------------------------------------------------
template <typename T>
struct rank : std::integral_constant<std::size_t, 0>{};

template <c_array T>
struct rank<T> : std::rank<T>{};

template <std_array T>
struct rank<T> : std::integral_constant<std::size_t, rank<typename T::value_type>::value + 1>{};

template <typename T>
inline constexpr auto rank_v{rank<T>::value};

// --- array extents -----------------------------------------------------------------------------------------------------------------------
template <typename T, std::size_t I = 0>
struct extent : std::integral_constant<std::size_t, 0>{};

template <c_array T>
struct extent<T> : std::extent<T>{};

template <c_array T, std::size_t I>
struct extent<T, I> : std::extent<T, I>{};

template <std_array T>
struct extent<T, 0> : std::integral_constant<std::size_t, std::tuple_size_v<T>>{};

template <std_array T, std::size_t I>
struct extent<T, I> : extent<typename T::value_type, I - 1>{};

template <array T, std::size_t I>
inline constexpr auto extent_v{extent<T, I>::value};

template <typename...>
struct extents_of;

template <array T, std::size_t...Is>
struct extents_of<T, std::index_sequence<Is...>> {
    using type = std::index_sequence<extent_v<T, Is>...>;
};

template <array T>
struct extents_of<T> {
    using type = extents_of<T, std::make_index_sequence<rank_v<T>>>::type;
};

template <array T>
using extents_of_t = extents_of<T>::type;

template <typename T>
struct remove_extent {
    using type = T;
};

template <typename T, std::size_t S>
struct remove_extent<T[S]> : std::remove_extent<T[S]>{};

template <std_array T>
struct remove_extent<T> {
    using type = T::value_type;
};

template <array T>
using remove_extent_t = typename remove_extent<T>::type;

template <typename T>
struct remove_all_extents {
	using type = T;
};

template <c_array T>
struct remove_all_extents<T> : std::remove_all_extents<T>{};

template <std_array T>
struct remove_all_extents<T> {
	using type = typename remove_all_extents<typename T::value_type>::type;
};

template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

template <typename...>
struct total_items;

template <typename T>
struct total_items<T> {
	inline static constexpr std::size_t value{1};
};

template <typename T, std::size_t S>
struct total_items<T[S]> {
    inline static constexpr auto value{S * total_items<remove_extent_t<T[S]>>::value};
};

template <typename T>
requires std_array<T>
struct total_items<T> {
	inline static constexpr auto value{std::tuple_size_v<T> * total_items<remove_extent_t<T>>::value};
};

template <array T>
inline constexpr auto total_items_v{total_items<T>::value};

// --- indexer -----------------------------------------------------------------------------------------------------------------------------
template <std::size_t>
inline constexpr std::size_t zero_index{0};

template <typename...>
struct indexer_of;

template <array T, std::size_t...Is>
struct indexer_of<T, std::index_sequence<Is...>> {
    using type = std::pair<std::index_sequence<zero_index<Is>...>, extents_of_t<T>>;
};

template <typename...>
struct last_indexer_of;
template <array T, std::size_t...Is>
struct last_indexer_of<T, std::index_sequence<Is...>> {
    using type = std::index_sequence<(extent_v<T, Is> - 1)...>;
};

template <array T>
struct last_indexer_of<T> {
    using type = last_indexer_of<T, std::make_index_sequence<rank_v<T>>>::type;
};

template <array T>
using last_indexer_of_t = last_indexer_of<T>::type;

template <array T>
struct indexer_of<T> {
    using type = indexer_of<T, std::make_index_sequence<rank_v<T>>>::type;
};

template <array T>
using indexer_of_t = indexer_of<T>::type;

template <typename...>
struct concat_sequence;

template <std::size_t S>
struct concat_sequence<std::index_sequence<S>> {
    using type = std::index_sequence<S>;
};

template <std::size_t...Heads, std::size_t...Tails>
struct concat_sequence<std::index_sequence<Heads...>, std::index_sequence<Tails...>> {
    using type = std::index_sequence<Heads..., Tails...>;
};

template <typename...>
struct index_sequence_head;

template <std::size_t Head, std::size_t...Tail>
struct index_sequence_head<std::index_sequence<Head, Tail...>> {
    inline static constexpr auto value{Head};
    using type = std::index_sequence<Head>;
};

template <typename...>
struct index_sequence_tail;

template <std::size_t Tail>
struct index_sequence_tail<std::index_sequence<Tail>> {
    inline static constexpr auto value{Tail};
    using type = std::index_sequence<value>;
};

template <std::size_t Head, std::size_t...Tail>
struct index_sequence_tail<std::index_sequence<Head, Tail...>> {
    inline static constexpr auto value{index_sequence_tail<std::index_sequence<Tail...>>::value};
    using type = std::index_sequence<value>;
};

template <typename...>
struct index_sequence_leading;

template <std::size_t S>
struct index_sequence_leading<std::index_sequence<S>> {
    using type = std::index_sequence<>;
};

template <std::size_t Head, std::size_t Tail>
struct index_sequence_leading<std::index_sequence<Head, Tail>> {
    using type = std::index_sequence<Head>;
};

template <std::size_t Head, std::size_t...Tail>
struct index_sequence_leading<std::index_sequence<Head, Tail...>> {
    using type = typename concat_sequence<
        std::index_sequence<Head>,
        typename index_sequence_leading<std::index_sequence<Tail...>>::type>::type;
};

template <typename...>
struct index_sequence_trailing;

template <std::size_t S>
struct index_sequence_trailing<std::index_sequence<S>> {
    using type = std::index_sequence<S>;
};

template <std::size_t Head, std::size_t...Tail>
struct index_sequence_trailing<std::index_sequence<Head, Tail...>> {
    using type = std::index_sequence<Tail...>;
};

template <typename...>
struct is_first_indexer;

template <std::size_t...Is, std::size_t...Es>
requires (sizeof...(Is) == sizeof...(Es))
struct is_first_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> {
    inline static constexpr bool value{std::is_same_v<std::index_sequence<Is...>, std::index_sequence<(Is * 0)...>>};
};

template <typename T>
inline constexpr bool is_first_indexer_v{is_first_indexer<T>::value};

template <typename...>
struct is_last_indexer;

template <std::size_t...Is, std::size_t...Es>
requires (sizeof...(Is) == sizeof...(Es))
struct is_last_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> {
    inline static constexpr bool value{std::is_same_v<std::index_sequence<Is...>, std::index_sequence<(Es - 1)...>>};
};

template <typename T>
inline constexpr bool is_last_indexer_v{is_last_indexer<T>::value};

template <typename...>
struct next_indexer;

template <std::size_t I, std::size_t E>
struct next_indexer<std::pair<std::index_sequence<I>, std::index_sequence<E>>> {
    inline static constexpr bool carries{I + 1 == E};
    using type = std::pair<std::index_sequence<(I + 1) < E ? I + 1 : 0>, std::index_sequence<E>>;
};

template <std::size_t...Is, std::size_t...Es>
requires (sizeof...(Is) == sizeof...(Es))
struct next_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> {
private:
    inline static constexpr bool carries{
        next_indexer<std::pair<
            typename index_sequence_head<std::index_sequence<Is...>>::type,
            typename index_sequence_head<std::index_sequence<Es...>>::type
        >>::carries
    };

    // indices reversed
    // inline static constexpr bool carries{
    //     next_indexer<std::pair<
    //         typename index_sequence_tail<std::index_sequence<Is...>>::type,
    //         typename index_sequence_tail<std::index_sequence<Es...>>::type
    //     >>::carries
    // };

public:
    using type = std::pair<std::conditional_t<carries,
        // if the head index carries, then type is 0, "next<trailing>"
        typename concat_sequence<
            std::index_sequence<0>,
            typename next_indexer<std::pair<
                typename index_sequence_trailing<std::index_sequence<Is...>>::type,
                typename index_sequence_trailing<std::index_sequence<Es...>>::type
            >>::type::first_type
        >::type,
        // if the head index does not carry, then type is head + 1, trailing
        typename concat_sequence<
            std::index_sequence<index_sequence_head<std::index_sequence<Is...>>::value + 1>,
            typename index_sequence_trailing<std::index_sequence<Is...>>::type
        >::type
    >, std::index_sequence<Es...>>;

    // indices reversed
    // using type = std::pair<std::conditional_t<carries,
    //     // if the tail index carries, then type is "next<leading>", 0
    //     typename concat_sequence<
    //         typename next_indexer<std::pair<
    //             typename index_sequence_leading<std::index_sequence<Is...>>::type,
    //             typename index_sequence_leading<std::index_sequence<Es...>>::type
    //         >>::type::first_type,
    //         std::index_sequence<0>
    //     >::type,
    //     // if the tail index does not carry, then type is leading, tail + 1
    //     typename concat_sequence<
    //         typename index_sequence_leading<std::index_sequence<Is...>>::type,
    //         std::index_sequence<index_sequence_tail<std::index_sequence<Is...>>::value + 1>
    //     >::type
    // >, std::index_sequence<Es...>>;
};

}//array_meta