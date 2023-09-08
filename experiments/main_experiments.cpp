#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <iostream>
#include <tuple>
#include <utility>
#include "std_array_meta.hpp"

template <array_meta::std_array T, std::size_t Stride = 0>
constexpr auto make_stride_indexer(const T& arr)
{
	const auto ext{array_meta::extents_of(arr)};
	const auto stride_size{ext[ext.size() - 1]};
	const auto stride_offset{stride_size * Stride};
	const auto stride_indexer{array_meta::next_index(array_meta::indexer<T>(), ext, stride_offset)};

	if constexpr (Stride == std::tuple_size_v<T> - 1) {
		return std::make_tuple(stride_indexer);
	}
	else {
		return std::tuple_cat(std::make_tuple(stride_indexer), make_stride_indexer<T, Stride + 1>(arr));
	}
}

template <array_meta::std_array T, std::size_t Stride = 0>
constexpr auto make_flag_indexer(const T& arr)
{
	constexpr std::bitset<array_meta::extent_v<T, array_meta::rank_v<T> - 1>> flag_indexer;

	if constexpr (Stride == std::tuple_size_v<T> - 1) {
		return std::make_tuple(flag_indexer);
	}
	else {
		return std::tuple_cat(std::make_tuple(flag_indexer), make_flag_indexer<T, Stride + 1>(arr));
	}
}

template <std::size_t Inc, array_meta::std_array T, std::size_t Stride = 0>
constexpr auto increment_stride_indexer(const T& arr, const auto& stride_indexer)
{
	const auto ext{array_meta::extents_of(arr)};
	const auto stride_idx{std::get<Stride>(stride_indexer)};

	if constexpr (Stride == std::tuple_size_v<T> - 1) {
		if constexpr (Inc == Stride) {
			return std::make_tuple(array_meta::next_index(stride_idx, ext));
		}
		else {
			return std::make_tuple(stride_idx);
		}
	}
	else {
		const auto tail{increment_stride_indexer<Inc, T, Stride + 1>(arr, stride_indexer)};

		if constexpr (Inc == Stride) {
			return std::tuple_cat(std::make_tuple(array_meta::next_index(stride_idx, ext)), tail);
		}
		else {
			return std::tuple_cat(std::make_tuple(stride_idx), tail);
		}
	}
}

template <array_meta::std_array T, std::size_t Min, std::size_t I>
constexpr std::size_t stride_index_of_min(const T& arr, const auto& stride_indexer, const auto& min)
{
	if constexpr (I == std::tuple_size_v<T> - 1) {
		return array_meta::at(arr, std::get<I>(stride_indexer)) < array_meta::at(arr, std::get<Min>(stride_indexer)) ? I : Min;
	}
	else {
		if (array_meta::at(arr, std::get<I>(stride_indexer)) < array_meta::at(arr, std::get<Min>(stride_indexer))) {
			return stride_index_of_min<T, I, I + 1>(arr, stride_indexer, array_meta::at(arr, std::get<I>(stride_indexer)));
		}
		else {
			return stride_index_of_min<T, Min, I + 1>(arr, stride_indexer, min);
		}
	}
}

template <array_meta::std_array T>
constexpr std::size_t stride_index_of_min(const T& arr, const auto& stride_indexer)
{
	return stride_index_of_min<T, 0, 1>(arr, stride_indexer, array_meta::at(arr, std::get<0>(stride_indexer)));
}

template <std::size_t I = 0, std::size_t Skip = 0, array_meta::std_array A, std::size_t M = array_meta::total_items_v<A>>
constexpr auto find_min(const A& arr, const auto& min_indexer, const auto& min)
{
	if constexpr ((1 << I) & Skip) {
		if constexpr (I == M - 1) {
			return min_indexer;
		}
		else {
			return find_min<I + 1, Skip>(arr, min_indexer, min);
		}
	}

	const auto ext{array_meta::extents_of(arr)};
	const auto next_indexer{array_meta::indexer_of(I, ext)};
	const auto next_value{array_meta::at(arr, next_indexer)};

	if (next_value < min) {
		if constexpr (I == M - 1) {
			return next_indexer;
		}
		else {
			return find_min<I + 1, Skip>(arr, next_indexer, next_value);
		}
	}
	else {
		if constexpr (I == M - 1) {
			return min_indexer;
		}
		else {
			return find_min<I + 1, Skip>(arr, min_indexer, min);
		}
	}
}

template <array_meta::std_array A>
constexpr auto find_min(const A& arr)
{
	return find_min<0>(arr, array_meta::indexer<A>(), array_meta::at(arr, 0));
}

template <std::size_t K, std::size_t I = 0, std::size_t Skip = 0, array_meta::std_array A, typename T = array_meta::remove_all_extents_t<A>, typename Idx = array_meta::indexer_t<array_meta::rank_v<A>>, std::size_t M = array_meta::total_items_v<A>, typename...Mins>
constexpr std::tuple<Mins...> find_k_min(const A& arr, const Idx& ext, const Idx& min_indexer, const T min, const std::tuple<Mins...>& k_mins)
{
	if constexpr ((1 << I) & Skip) {// skip this index (already used)
		if constexpr (I == M - 1) { // end of the array
			if constexpr (static_cast<std::size_t>(std::popcount(Skip)) < K && static_cast<std::size_t>(std::popcount(Skip)) < M) {
				return find_k_min<K, 0, Skip>(arr, ext, array_meta::indexer<A>(), array_meta::at(arr, array_meta::indexer<A>()), k_mins);
			}
			else {
				return k_mins; // should be a tuple of K min items
			}
		}
		else { // skip this one, move on to next
			return find_k_min<K, I + 1, Skip>(arr, ext, min_indexer, min, k_mins);
		}
	}

	// const Idx ext{array_meta::extents_of(arr)};
	const Idx next_indexer{array_meta::indexer_of(I, ext)};
	const T next_value{array_meta::at(arr, next_indexer)};

	if (next_value < min) { // current index is a new min
		if constexpr (I == M - 1) { // end of the array
			if constexpr (static_cast<std::size_t>(std::popcount(Skip)) < K && static_cast<std::size_t>(std::popcount(Skip)) < M) {
				return find_k_min<K, 0, Skip | (1 << I)>(arr, ext, array_meta::indexer<A>(), array_meta::at(arr, array_meta::indexer<A>()), k_mins);
			}
			else {
				return std::tuple_cat(k_mins, std::make_tuple(array_meta::at(arr, next_indexer))); // append current index to the tuple
			}
		}
		else { // pass the current min on to the next index check
			return find_k_min<K, I + 1, Skip | (1 << I)>(arr, ext, next_indexer, next_value, k_mins);
		}
	}
	else {
		if constexpr (I == M - 1) { // end of the array, min passed in is still the min
			if constexpr (static_cast<std::size_t>(std::popcount(Skip)) < K && static_cast<std::size_t>(std::popcount(Skip)) < M) {
				return find_k_min<K, 0, Skip>(arr, ext, array_meta::indexer<A>(), array_meta::at(arr, array_meta::indexer<A>()), k_mins);
			}
			else {
				return std::tuple_cat(k_mins, std::make_tuple(array_meta::at(arr, min_indexer)));
			}
		}
		else { // pass the current min on to the next index check
			return find_k_min<K, I + 1, Skip>(arr, ext, min_indexer, min, k_mins);
		}
	}
}

template <std::size_t K, array_meta::std_array A>
constexpr auto find_k_min(const A& arr)
{
	// const auto k_tuple{find_k_min<K, 0>(arr, array_meta::indexer<A>(), array_meta::at(arr, array_meta::indexer<A>()), std::make_tuple())};
	// return std::tuple_size_v<decltype(k_tuple)>;
	// return std::make_from_tuple<std::array<array_meta::remove_all_extents_t<A>, std::tuple_size_v<decltype(k_tuple)>>>(k_tuple);
	// return std::make_from_tuple<std::array<array_meta::remove_all_extents_t<A>, K>>(k_tuple);

	return find_k_min<K>(arr, array_meta::extents_of(arr), array_meta::indexer<A>(), array_meta::at(arr, array_meta::indexer<A>()), std::make_tuple());
}


// template <std::size_t K, array_meta::std_array A, typename T = array_meta::remove_all_extents_t<A>, std::size_t Used = 0, std::size_t I = 0>
// constexpr auto find_k_min(const A& arr)
// {
// 	if constexpr (std::popcount(Used) == K) {
// 		return std::make_tuple();
// 	}

// 	const auto ext{array_meta::extents_of(arr)};
// 	const auto indexer{array_meta::indexer_of(I, ext)};


// 	const auto next_min{find_min<Used>(arr)};
// 	const auto min_offset{array_meta::offset_of(next_min, ext)};


// 	// return std::make_from_tuple<std::array<array_meta::remove_all_extents_t<T>, K>>(array_meta::at(arr, next_min), find_k_min<Used | min_offset>(arr));
// }

// namespace aux{
// template<std::size_t...> struct seq{};

// template<std::size_t N, std::size_t... Is>
// struct gen_seq : gen_seq<N-1, N-1, Is...>{};

// template<std::size_t... Is>
// struct gen_seq<0, Is...> : seq<Is...>{};

// template<class Ch, class Tr, class Tuple, std::size_t... Is>
// void print_tuple(std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){
//   using swallow = int[];
//   (void)swallow{0, (void(os << (Is == 0? "" : ", ") << std::get<Is>(t)), 0)...};
// }
// } // aux::

// template<class Ch, class Tr, class... Args>
// auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
//     -> std::basic_ostream<Ch, Tr>&
// {
//   os << "(";
//   aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
//   return os << ")";
// }

int main()
{
	{
		static_assert(typeid(array_meta::remove_all_extents_t<std::array<int, 0>>) == typeid(int));
		static_assert(typeid(array_meta::remove_all_extents_t<std::array<std::array<int, 0>, 0>>) == typeid(int));
		static_assert(typeid(array_meta::remove_all_extents_t<std::array<std::array<std::array<int, 0>, 0>, 0>>) == typeid(int));

		static_assert(array_meta::rank_v<int> == 0);
		static_assert(array_meta::rank_v<std::array<int, 0>> == 1);
		static_assert(array_meta::rank_v<std::array<std::array<int, 0>, 0>> == 2);
		static_assert(array_meta::rank_v<std::array<std::array<std::array<int, 0>, 0>, 0>> == 3);

		static_assert(array_meta::extent_v<int, 0> == 0);
		static_assert(array_meta::extent_v<std::array<int, 5>, 0> == 5);
		static_assert(array_meta::extent_v<std::array<std::array<int, 11>, 22>, 0> == 22);
		static_assert(array_meta::extent_v<std::array<std::array<int, 11>, 22>, 1> == 11);
		static_assert(array_meta::extent_v<std::array<std::array<std::array<int, 33>, 44>, 55>, 0> == 55);
		static_assert(array_meta::extent_v<std::array<std::array<std::array<int, 33>, 44>, 55>, 1> == 44);
		static_assert(array_meta::extent_v<std::array<std::array<std::array<int, 33>, 44>, 55>, 2> == 33);

		constexpr std::array<std::array<std::array<int, 7>, 8>, 9> a{};
		static_assert(array_meta::total_items_v<decltype(a)> == 7*8*9);

		constexpr auto ext{array_meta::extents_of(a)};
		static_assert(ext.size() == 3);
		static_assert(ext[0] == 9);
		static_assert(ext[1] == 8);
		static_assert(ext[2] == 7);

		constexpr auto idx{array_meta::indexer<decltype(a)>()};
		static_assert(idx.size() == 3);
		static_assert(idx[0] == 0);
		static_assert(idx[1] == 0);
		static_assert(idx[2] == 0);

		static_assert(array_meta::offset_of(idx, ext) == 0);
		static_assert(array_meta::offset_of(array_meta::next_index(idx, ext), ext) == 1);
	}

	{
		// test values in the array mimic a base-1 location value in the array for testing clarity.
		constexpr std::array<std::array<std::array<int, 2>, 3>, 4> a{{
			// all these extra braces are required (gcc is slightly more forgiving than clang)...
			// or use a "flat" initializer and compiler flag: -Wno-missing-braces
			{{{111, 112},
			  {121, 122},
			  {131, 132}}},

			{{{211, 212},
			  {221, 222},
			  {231, 232}}},

			{{{311, 312},
			  {321, 322},
			  {331, 332}}},

			{{{411, 412},
			  {421, 422},
			  {431, 432}}},
		}};

		constexpr auto ext{array_meta::extents_of(a)};

		constexpr auto idx0{array_meta::indexer<decltype(a)>()};
		static_assert(array_meta::offset_of(idx0, ext) == 0);

		constexpr auto idx1{array_meta::next_index(idx0, ext)};
		static_assert(array_meta::offset_of(idx1, ext) == 1);

		constexpr auto idx2{array_meta::next_index(idx1, ext)};
		static_assert(array_meta::offset_of(idx2, ext) == 2);

		constexpr auto idx3{array_meta::next_index(idx2, ext)};
		static_assert(array_meta::offset_of(idx3, ext) == 3);

		constexpr auto idx4{array_meta::next_index(idx3, ext)};
		static_assert(array_meta::offset_of(idx4, ext) == 4);

		constexpr auto idx5{array_meta::next_index(idx4, ext)};
		static_assert(array_meta::offset_of(idx5, ext) == 5);

		constexpr auto idx6{array_meta::next_index(idx5, ext)};
		static_assert(array_meta::offset_of(idx6, ext) == 6);

		constexpr auto idx7{array_meta::next_index(idx6, ext)};
		static_assert(array_meta::offset_of(idx7, ext) == 7);

		constexpr auto idx8{array_meta::next_index(idx7, ext)};
		static_assert(array_meta::offset_of(idx8, ext) == 8);

		constexpr auto idx9{array_meta::next_index(idx8, ext)};
		static_assert(array_meta::offset_of(idx9, ext) == 9);

		// advance by 15
		constexpr auto idx15{array_meta::next_index(idx0, ext, 15)};
		static_assert(array_meta::at(a, idx15) == 322);

		// test roundtrip: offset -> indexer -> offset
		constexpr auto offset9{array_meta::offset_of(idx9, ext)};
		constexpr auto rt_idx9{array_meta::indexer_of(offset9, ext)};
		static_assert(rt_idx9 == idx9);

		static_assert(array_meta::at(a, 0) == 111);
		static_assert(array_meta::at(a, 1) == 112);
		static_assert(array_meta::at(a, offset9) == 222);
		static_assert(array_meta::at(a, rt_idx9) == 222);
		static_assert(array_meta::at(a, idx6) == 211);

		// explicit indexer
		constexpr array_meta::indexer_t<3> ex_idx{3, 2, 1}; // base 0 index...
		static_assert(array_meta::at(a, ex_idx) == 432);    // but test value is base 1, so index 3,2,1 -> 432

		// stride tests
		constexpr auto idx_stride1{array_meta::next_stride(idx0, ext)};
		static_assert(array_meta::at(a, idx_stride1) == 121);
		constexpr auto idx_stride2{array_meta::next_stride(idx_stride1, ext)};
		static_assert(array_meta::at(a, idx_stride2) == 131);
		constexpr auto idx_stride3{array_meta::next_stride(idx_stride2, ext)};
		static_assert(array_meta::at(a, idx_stride3) == 211);
	}

	{
		constexpr std::array<std::array<int, 5>, 3> a{{
			{95, 32, 47, 80, 28},
			{15, 51, 60, 24, 33},
			{51, 25, 76, 84, 83}
		}};

		constexpr auto ext{array_meta::extents_of(a)};
		constexpr auto idx0{array_meta::indexer<decltype(a)>()};
		static_assert(array_meta::at(a, idx0) == 95);
		constexpr auto idx1{array_meta::next_stride(idx0, ext)};
		static_assert(array_meta::at(a, idx1) == 15);
		constexpr auto idx2{array_meta::next_stride(idx1, ext)};
		static_assert(array_meta::at(a, idx2) == 51);

		constexpr auto stride_idx{make_stride_indexer(a)};
		static_assert(typeid(stride_idx) == typeid(std::tuple<array_meta::indexer_t<2>, array_meta::indexer_t<2>, array_meta::indexer_t<2>>));
		static_assert(array_meta::at(a, std::get<0>(stride_idx)) == 95);
		static_assert(array_meta::at(a, std::get<1>(stride_idx)) == 15);
		static_assert(array_meta::at(a, std::get<2>(stride_idx)) == 51);

		constexpr auto stride_idx_next0_1{increment_stride_indexer<0>(a, stride_idx)};
		static_assert(array_meta::at(a, std::get<0>(stride_idx_next0_1)) == 32);
		static_assert(array_meta::at(a, std::get<1>(stride_idx_next0_1)) == 15);
		static_assert(array_meta::at(a, std::get<2>(stride_idx_next0_1)) == 51);

		constexpr auto stride_idx_next0_2{increment_stride_indexer<0>(a, stride_idx_next0_1)};
		static_assert(array_meta::at(a, std::get<0>(stride_idx_next0_2)) == 47);
		static_assert(array_meta::at(a, std::get<1>(stride_idx_next0_2)) == 15);
		static_assert(array_meta::at(a, std::get<2>(stride_idx_next0_2)) == 51);

		constexpr auto stride_idx_next1_1{increment_stride_indexer<1>(a, stride_idx)};
		static_assert(array_meta::at(a, std::get<0>(stride_idx_next1_1)) == 95);
		static_assert(array_meta::at(a, std::get<1>(stride_idx_next1_1)) == 51);
		static_assert(array_meta::at(a, std::get<2>(stride_idx_next1_1)) == 51);

		constexpr auto stride_idx_next2_1{increment_stride_indexer<2>(a, stride_idx)};
		static_assert(array_meta::at(a, std::get<0>(stride_idx_next2_1)) == 95);
		static_assert(array_meta::at(a, std::get<1>(stride_idx_next2_1)) == 15);
		static_assert(array_meta::at(a, std::get<2>(stride_idx_next2_1)) == 25);

		constexpr auto least{stride_index_of_min(a, stride_idx)};
		static_assert(array_meta::at(a, std::get<least>(stride_idx)) == 15);

		constexpr auto flag_indexer{make_flag_indexer(a)};
		static_assert(typeid(flag_indexer) == typeid(std::tuple<std::bitset<5>, std::bitset<5>, std::bitset<5>>));

		constexpr auto min{find_min(a)};
		static_assert(array_meta::at(a, min) == 15);

		// constexpr auto min_3{find_k_min<1>(a)};
		// // static_assert(min_3 == 3);
		// std::cout << min_3 << '\n';
	}

	return 0;
}
// reset && make && build/array-picker && printf '\n###\n' || printf '\nfailed\n'
