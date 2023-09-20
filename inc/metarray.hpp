#pragma once

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
// #include <variant>

namespace metarray {
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
	inline static constexpr auto value{S * total_items<std::remove_cvref_t<remove_extent_t<T[S]>>>::value};
};

template <std_array T>
struct total_items<T> {
	inline static constexpr auto value{std::tuple_size_v<T> * total_items<std::remove_cvref_t<remove_extent_t<T>>>::value};
};

template <array T>
inline constexpr auto total_items_v{total_items<T>::value};

// --- support traits/concepts -------------------------------------------------------------------------------------------------------------
template <typename...>
struct is_index_sequence : std::false_type{};

template <std::size_t...S>
struct is_index_sequence<std::index_sequence<S...>> : std::true_type{};

template <typename T>
inline constexpr bool is_index_sequence_v{is_index_sequence<T>::value};

template <typename T>
concept index_sequence = is_index_sequence_v<T>;

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
	using type = std::pair<std::index_sequence<(extent_v<T, Is> - 1)...>, extents_of_t<T>>;
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
struct is_valid_indexer : std::false_type{};

template <std::size_t...Is, std::size_t...Es>
requires (sizeof...(Is) == sizeof...(Es) && ((Is < Es) && ...))
struct is_valid_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> : std::true_type{};

template <typename Idx>
inline constexpr auto is_valid_indexer_v{is_valid_indexer<std::remove_cvref_t<Idx>>::value};

template <typename Idx>
concept valid_indexer = is_valid_indexer_v<Idx>;

template <typename...>
struct is_valid_indexer_of : std::false_type{};

template <array A, std::size_t...Is, std::size_t...Es>
requires (rank_v<A> == sizeof...(Is) && sizeof...(Is) == sizeof...(Es) && ((Is < Es) && ...))
struct is_valid_indexer_of<A, std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> : std::true_type{};

template <array A, typename Idx>
inline constexpr auto is_valid_indexer_of_v{is_valid_indexer_of<A, std::remove_cvref_t<Idx>>::value};

template <typename A, typename Idx>
concept valid_indexer_of = is_valid_indexer_of_v<A, Idx>;

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

template <index_sequence...Seq>
using concat_sequence_t = concat_sequence<Seq...>::type;

template <typename...>
struct index_sequence_head;

template <std::size_t Head, std::size_t...Tail>
struct index_sequence_head<std::index_sequence<Head, Tail...>> {
	inline static constexpr auto value{Head};
	using type = std::index_sequence<Head>;
};

template <index_sequence Seq>
using index_sequence_head_t = index_sequence_head<Seq>::type;

template <index_sequence Seq>
inline constexpr auto index_sequence_head_v{index_sequence_head<Seq>::value};

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

template <index_sequence Seq>
using index_sequence_tail_t = index_sequence_tail<Seq>::type;

template <index_sequence Seq>
inline constexpr auto index_sequence_tail_v{index_sequence_tail<Seq>::value};

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
	using type = concat_sequence_t<
		std::index_sequence<Head>,
		typename index_sequence_leading<std::index_sequence<Tail...>>::type
	>;
};

template <typename Seq>
using index_sequence_leading_t = index_sequence_leading<Seq>::type;

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

template <typename Seq>
using index_sequence_trailing_t = index_sequence_trailing<Seq>::type;

template <typename...>
struct is_first_indexer;

template <std::size_t...Is, std::size_t...Es>
requires (valid_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>>)
struct is_first_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> {
	inline static constexpr bool value{std::is_same_v<std::index_sequence<Is...>, std::index_sequence<(Is * 0)...>>};
};

template <valid_indexer T>
inline constexpr bool is_first_indexer_v{is_first_indexer<T>::value};

template <typename...>
struct is_last_indexer;

template <std::size_t...Is, std::size_t...Es>
requires (valid_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>>)
struct is_last_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> {
	inline static constexpr bool value{std::is_same_v<std::index_sequence<Is...>, std::index_sequence<(Es - 1)...>>};
};

template <valid_indexer T>
inline constexpr bool is_last_indexer_v{is_last_indexer<T>::value};

template <typename...>
struct next_indexer;

template <std::size_t I, std::size_t E>
requires (I < E)
struct next_indexer<std::pair<std::index_sequence<I>, std::index_sequence<E>>> {
	inline static constexpr bool carries{I + 1 == E};
	using type = std::pair<std::index_sequence<(I + 1) < E ? I + 1 : 0>, std::index_sequence<E>>;
};

template <std::size_t...Is, std::size_t...Es>
requires (valid_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>>)
struct next_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> {
private:
	inline static constexpr bool carries{
		next_indexer<std::pair<
			index_sequence_head_t<std::index_sequence<Is...>>,
			index_sequence_head_t<std::index_sequence<Es...>>
		>>::carries
	};

public:
	using type = std::pair<std::conditional_t<carries,
		// if the head index carries, then type is 0, "next<trailing>"
		concat_sequence_t<
			std::index_sequence<0>,
			typename next_indexer<std::pair<
				index_sequence_trailing_t<std::index_sequence<Is...>>,
				index_sequence_trailing_t<std::index_sequence<Es...>>
			>>::type::first_type
		>,
		// if the head index does not carry, then type is head + 1, trailing
		concat_sequence_t<
			std::index_sequence<index_sequence_head_v<std::index_sequence<Is...>> + 1>,
			index_sequence_trailing_t<std::index_sequence<Is...>>
		>
	>, std::index_sequence<Es...>>;
};

template <valid_indexer I>
using next_indexer_t = next_indexer<I>::type;

// --- offsets -----------------------------------------------------------------------------------------------------------------------------
template <typename...>
struct indexer_from_offset;

template <std::size_t O, std::size_t E>
requires (E > 0)
struct indexer_from_offset<std::integral_constant<std::size_t, O>, std::index_sequence<E>> {
	using type = std::pair<std::index_sequence<O % E>, std::index_sequence<E>>;
};

template <std::size_t O, std::size_t...Es>
requires ((Es > 0),...)
struct indexer_from_offset<std::integral_constant<std::size_t, O>, std::index_sequence<Es...>> {
private:
	inline static constexpr auto tail_extent{index_sequence_tail_v<std::index_sequence<Es...>>};

public:
	using type = std::pair<
		concat_sequence_t<
			typename indexer_from_offset<
				std::integral_constant<std::size_t, O / tail_extent>,
				index_sequence_leading_t<std::index_sequence<Es...>>
			>::type::first_type,
			std::index_sequence<O % tail_extent>
		>,
		std::index_sequence<Es...>
	>;
};

template <std::size_t O, index_sequence E>
using indexer_from_offset_t = indexer_from_offset<std::integral_constant<std::size_t, O>, E>::type;

template <typename...>
struct offset_from_indexer;

template <std::size_t I, std::size_t E>
requires (I < E)
struct offset_from_indexer<std::pair<std::index_sequence<I>, std::index_sequence<E>>> {
	inline static constexpr auto value{I};
};

template <std::size_t...Is, std::size_t...Es>
requires (valid_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>>)
struct offset_from_indexer<std::pair<std::index_sequence<Is...>, std::index_sequence<Es...>>> {
private:
	inline static constexpr auto tail_idx{index_sequence_tail_v<std::index_sequence<Is...>>};
	inline static constexpr auto tail_ext{index_sequence_tail_v<std::index_sequence<Es...>>};

	using leading = std::pair<
		index_sequence_leading_t<std::index_sequence<Is...>>,
		index_sequence_leading_t<std::index_sequence<Es...>>>;

public:
	inline static constexpr auto value{tail_idx + (tail_ext * offset_from_indexer<leading>::value)};
};

template <valid_indexer Idx>
inline constexpr auto offset_from_indexer_v{offset_from_indexer<Idx>::value};

// --- iteration ---------------------------------------------------------------------------------------------------------------------------
// template <typename...>
// struct concat_variant;

// template <typename T>
// struct concat_variant<std::variant<T>> {
// 	using type = std::variant<T>;
// };

// template <typename...Heads, typename...Tails>
// struct concat_variant<std::variant<Heads...>, std::variant<Tails...>> {
// 	using type = std::variant<Heads..., Tails...>;
// };

// template <typename...T>
// using concat_variant_t = concat_variant<T...>::type;

// template <typename...>
// struct index_iterator_of;

// template <array A, valid_indexer Idx>
// requires is_last_indexer_v<Idx>
// struct index_iterator_of<A, Idx> {
// 	using type = std::variant<Idx>;
// };

// template <array A, valid_indexer Idx>
// struct index_iterator_of<A, Idx> {
// 	using type = concat_variant_t<std::variant<Idx>, typename index_iterator_of<A, next_indexer_t<Idx>>::type>;
// };

// template <array A>
// struct index_iterator_of<A> {
// 	using type = index_iterator_of<A, indexer_of_t<A>>::type;
// };

// template <array A>
// using index_iterator_of_t = index_iterator_of<A>::type;

template <typename...>
struct concat_tuple;

template <typename T>
struct concat_tuple<std::tuple<T>> {
	using type = std::tuple<T>;
};

template <typename...Heads, typename...Tails>
struct concat_tuple<std::tuple<Heads...>, std::tuple<Tails...>> {
	using type = std::tuple<Heads..., Tails...>;
};

template <typename...T>
using concat_tuple_t = concat_tuple<T...>::type;

template <typename...>
struct indexer_list_of;

template <array A, valid_indexer Idx>
requires is_last_indexer_v<Idx>
struct indexer_list_of<A, Idx> {
	using type = std::tuple<Idx>;
};

template <array A, valid_indexer Idx>
struct indexer_list_of<A, Idx> {
	using type = concat_tuple_t<std::tuple<Idx>, typename indexer_list_of<A, next_indexer_t<Idx>>::type>;
};

template <array A>
struct indexer_list_of<A> {
	using type = indexer_list_of<A, indexer_of_t<A>>::type;
};

template <array A>
using indexer_list_of_t = indexer_list_of<A>::type;

template <typename...>
struct remove_indexer;

template <valid_indexer Rem, valid_indexer Idx>
struct remove_indexer<Rem, std::tuple<Idx>> {
	using type = std::conditional_t<std::is_same_v<Rem, Idx>, std::tuple<>, std::tuple<Idx>>;
};

template <valid_indexer Rem, valid_indexer Head, valid_indexer...Idx>
struct remove_indexer<Rem, std::tuple<Head, Idx...>> {
	using type = std::conditional_t<
		std::is_same_v<Rem, Head>,
		typename remove_indexer<Rem, std::tuple<Idx...>>::type,
		concat_tuple_t<std::tuple<Head>, typename remove_indexer<Rem, std::tuple<Idx...>>::type>
	>;
};

template <valid_indexer Rem, typename IdxList>
using remove_indexer_t = typename remove_indexer<Rem, IdxList>::type;

template <typename...>
struct contains_indexer;

template <valid_indexer Find>
struct contains_indexer<Find, std::tuple<>> {
	inline static constexpr auto value{false};
};

template <valid_indexer Find, valid_indexer Idx>
struct contains_indexer<Find, std::tuple<Idx>> {
	inline static constexpr auto value{std::is_same_v<Find, Idx>};
};

template <valid_indexer Find, valid_indexer Head, valid_indexer...Idx>
struct contains_indexer<Find, std::tuple<Head, Idx...>> {
	inline static constexpr auto value{std::is_same_v<Find, Head> || contains_indexer<Find, std::tuple<Idx...>>::value};
};

template <valid_indexer Find, typename IdxList>
inline constexpr auto contains_indexer_v{contains_indexer<Find, IdxList>::value};

template <typename...>
struct set_diff;

template <valid_indexer IdxA, valid_indexer...IdxB>
struct set_diff<std::tuple<IdxA>, std::tuple<IdxB...>> {
	using type = std::conditional_t<
		contains_indexer_v<IdxA, std::tuple<IdxB...>>,
		std::tuple<>,
		std::tuple<IdxA>
	>;
};

template <valid_indexer HeadA, valid_indexer...IdxA, valid_indexer...IdxB>
struct set_diff<std::tuple<HeadA, IdxA...>, std::tuple<IdxB...>> {
	using type = std::conditional_t<
		contains_indexer_v<HeadA, std::tuple<IdxB...>>,
		typename set_diff<std::tuple<IdxA...>, std::tuple<IdxB...>>::type,
		concat_tuple_t<std::tuple<HeadA>, typename set_diff<std::tuple<IdxA...>, std::tuple<IdxB...>>::type>
	>;
};

template <typename IdxListA, typename IdxListB>
using set_diff_t = typename set_diff<IdxListA, IdxListB>::type;

// --- access ------------------------------------------------------------------------------------------------------------------------------
template <valid_indexer Idx, array A>
requires (valid_indexer_of<A, Idx> && rank_v<A> > 0)
constexpr auto get(const A& a)
{
	if constexpr (rank_v<A> == 1) {
		return a[index_sequence_head_v<typename std::remove_cvref_t<Idx>::first_type>];
	}
	else {
		return get<
			std::pair<
				index_sequence_trailing_t<typename std::remove_cvref_t<Idx>::first_type>,
				index_sequence_trailing_t<typename std::remove_cvref_t<Idx>::second_type>
			>
		>(a[index_sequence_head_v<typename std::remove_cvref_t<Idx>::first_type>]);
	}
}

// template <valid_indexer Idx, array A>
// requires (valid_indexer_of<A, Idx> && rank_v<A> > 0)
// constexpr auto get(A& a)
// {
// 	if constexpr (rank_v<A> == 1) {
// 		return a[index_sequence_head_v<typename std::remove_cvref_t<Idx>::first_type>];
// 	}
// 	else {
// 		return get<
// 			std::pair<
// 				index_sequence_trailing_t<typename std::remove_cvref_t<Idx>::first_type>,
// 				index_sequence_trailing_t<typename std::remove_cvref_t<Idx>::second_type>
// 			>
// 		>(a[index_sequence_head_v<typename std::remove_cvref_t<Idx>::first_type>]);
// 	}
// }

// template <typename Iter, array A>
// requires (rank_v<A> > 0)//TODO: need to concept Iter somehow
// constexpr auto& get(const A& array, const Iter& iter)
// {
// 	using Idx = decltype(std::visit([](auto&& idx) { return idx; }, iter));

// 	if constexpr (rank_v<A> == 1) {
// 		return array[index_sequence_head_v<typename Idx::first_type>];
// 	}
// 	else {
// 		return get<
// 			std::pair<
// 				index_sequence_trailing_t<typename Idx::first_type>,
// 				index_sequence_trailing_t<typename Idx::second_type>
// 			>,
// 			remove_extent_t<A>
// 		>(array[index_sequence_head_v<typename Idx::first_type>]);
// 		// >(*const_cast<std::remove_cv_t<remove_extent_t<A>>*>(&array[index_sequence_head_v<typename Idx::first_type>]));

// 	}
// }

// --- transformation ----------------------------------------------------------------------------------------------------------------------
//TODO: shouldn't need std::remove_reference_t here. lower level types should work with or without it.
template <typename StaticArray>
using unwrap_static_array_t = std::remove_reference_t<decltype(StaticArray::value)>;

template <typename StaticArray, valid_indexer...Idx, std::size_t...Is>
requires ((sizeof...(Idx) <= total_items_v<unwrap_static_array_t<StaticArray>>)
	&& (is_valid_indexer_of_v<unwrap_static_array_t<StaticArray>, Idx> && ...)
	&& sizeof...(Is) == sizeof...(Idx))
constexpr std::array<remove_all_extents_t<unwrap_static_array_t<StaticArray>>, sizeof...(Idx)>
transform_to_array(const std::tuple<Idx...>&, const std::index_sequence<Is...>&)
{
	return {{get<Idx>(StaticArray::value)...}};
}

template <typename StaticArray, valid_indexer...Idx>
requires ((sizeof...(Idx) <= total_items_v<unwrap_static_array_t<StaticArray>>)
	&& (is_valid_indexer_of_v<unwrap_static_array_t<StaticArray>, Idx> && ...))
constexpr std::array<remove_all_extents_t<unwrap_static_array_t<StaticArray>>, sizeof...(Idx)>
transform_to_array(const std::tuple<Idx...>& idx)
{
	return transform_to_array<StaticArray, Idx...>(idx, std::make_index_sequence<sizeof...(Idx)>{});
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

// template <array A, typename Pred, std::size_t I, valid_indexer...Idx>
// requires ((I == sizeof...(Idx) - 1) && (is_valid_indexer_of_v<A, Idx> && ...))
// constexpr auto find_if(const A& a, std::tuple<Idx...> idx, Pred pred)
// {
// 	using IdxN = std::remove_reference_t<decltype(std::get<I>(idx))>;
// 	constexpr auto item{get<IdxN>(a)};

// 	if constexpr (pred(item)) {
// 		return std::make_tuple(IdxN{});
// 	}
// 	else {
// 		return std::make_tuple();
// 	}
// }

// template <array A, typename Pred, std::size_t I, valid_indexer...Idx>
// requires ((I < sizeof...(Idx) - 1) && (is_valid_indexer_of_v<A, Idx> && ...))
// constexpr auto find_if(const A& a, std::tuple<Idx...> idx, Pred pred)
// {
// 	using IdxN = std::remove_reference_t<decltype(std::get<I>(idx))>;
// 	constexpr auto item{get<IdxN>(a)};

// 	if constexpr (pred(item)) {
// 		return std::tuple_cat(std::make_tuple(IdxN{}), find_if<A, Pred, I + 1>(a, idx, pred));
// 	}
// 	else {
// 		return find_if<A, Pred, I + 1>(a, idx, pred);
// 	}
// }

template <typename StaticArray, std::size_t I, std::size_t M, valid_indexer...Idx>
constexpr auto find_min(const std::tuple<Idx...>& idx)
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
			return find_min<StaticArray, I + 1, I>(idx);
		}
		else {
			return find_min<StaticArray, I + 1, M>(idx);
		}
	}
}

template <typename StaticArray>
constexpr auto find_min()
{
	return find_min<StaticArray, 0, 0>(indexer_list_of_t<unwrap_static_array_t<StaticArray>>{});
}

template <std::size_t K, typename StaticArray, valid_indexer...Idx, valid_indexer...Found>
constexpr auto find_k_min(const std::tuple<Idx...>& idx, const std::tuple<Found...>& found)
{
	if constexpr (K == sizeof...(Found) || K == total_items_v<unwrap_static_array_t<StaticArray>>) {
		return found;
	}
	else {
		constexpr auto next_min{find_min<StaticArray, 0, 0>(set_diff_t<std::tuple<Idx...>, std::tuple<Found...>>{})};
		return find_k_min<K, StaticArray>(idx, std::tuple_cat(found, std::make_tuple(next_min)));
	}
}

template <std::size_t K, typename StaticArray>
constexpr auto find_k_min()
{
	return find_k_min<K, StaticArray>(indexer_list_of_t<unwrap_static_array_t<StaticArray>>{}, std::make_tuple());
}

template <array A, typename T, typename BinOp, std::size_t I, valid_indexer...Idx>
requires (is_valid_indexer_of_v<A, Idx> && ...)
constexpr T accumulate(const A& a, const std::tuple<Idx...>& idx, T init, BinOp op)
{
	using IdxN = decltype(std::get<I>(idx));

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


//TODO: maybe?
// prev_indexer
// flatten_extent
// flatten_all_extents
// stride ops
// increment_indexer_by#
// decrement_indexer_by#
