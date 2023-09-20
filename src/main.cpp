#include <array>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>
#include "metarray.hpp"
#include "metalgo.hpp"
// #include "demangle.hpp"

namespace test {
	using namespace metarray;

	constexpr void type_checks()
	{
		{// constexpr std_arrays
			constexpr int a1[1]{};
			static_assert(not is_std_array_v<decltype(a1)>);

			constexpr int a2[1][1]{};
			static_assert(not is_std_array_v<decltype(a2)>);

			constexpr int a3[1][1][1]{};
			static_assert(not is_std_array_v<decltype(a3)>);

			constexpr std::array<int, 1> a4{};
			static_assert(is_std_array_v<decltype(a4)>);

			constexpr std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_std_array_v<decltype(a5)>);

			constexpr std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_std_array_v<decltype(a6)>);
		}

		{// const std_arrays
			static_assert(not is_std_array_v<const int[1]>);
			static_assert(not is_std_array_v<const int[1][1]>);
			static_assert(not is_std_array_v<const int[1][1][1]>);
			static_assert(is_std_array_v<const std::array<int, 1>>);
			static_assert(is_std_array_v<const std::array<std::array<int, 1>, 1>>);
			static_assert(is_std_array_v<const std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}

		{// std_arrays
			static_assert(not is_std_array_v<int[1]>);
			static_assert(not is_std_array_v<int[1][1]>);
			static_assert(not is_std_array_v<int[1][1][1]>);
			static_assert(is_std_array_v<std::array<int, 1>>);
			static_assert(is_std_array_v<std::array<std::array<int, 1>, 1>>);
			static_assert(is_std_array_v<std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}

		{// volatile std_arrays
			static_assert(not is_std_array_v<volatile int[1]>);
			static_assert(not is_std_array_v<volatile int[1][1]>);
			static_assert(not is_std_array_v<volatile int[1][1][1]>);
			static_assert(is_std_array_v<volatile std::array<int, 1>>);
			static_assert(is_std_array_v<volatile std::array<std::array<int, 1>, 1>>);
			static_assert(is_std_array_v<volatile std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}

		{// const volatile std_arrays
			static_assert(not is_std_array_v<const volatile int[1]>);
			static_assert(not is_std_array_v<const volatile int[1][1]>);
			static_assert(not is_std_array_v<const volatile int [1][1][1]>);
			static_assert(is_std_array_v<const volatile std::array<int, 1>>);
			static_assert(is_std_array_v<const volatile std::array<std::array<int, 1>, 1>>);
			static_assert(is_std_array_v<const volatile std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}

		{// constexpr arrays
			constexpr int a1[1]{};
			static_assert(is_array_v<decltype(a1)>);

			constexpr int a2[1][1]{};
			static_assert(is_array_v<decltype(a2)>);

			constexpr int a3[1][1][1]{};
			static_assert(is_array_v<decltype(a3)>);

			constexpr std::array<int, 1> a4{};
			static_assert(is_array_v<decltype(a4)>);

			constexpr std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_array_v<decltype(a5)>);

			constexpr std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_array_v<decltype(a6)>);
		}

		{// const arrays
			static_assert(is_array_v<const int[1]>);
			static_assert(is_array_v<const int[1][1]>);
			static_assert(is_array_v<const int [1][1][1]>);
			static_assert(is_array_v<const std::array<int, 1>>);
			static_assert(is_array_v<const std::array<std::array<int, 1>, 1>>);
			static_assert(is_array_v<const std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}

		{// arrays
			static_assert(is_array_v<int[1]>);
			static_assert(is_array_v<int[1][1]>);
			static_assert(is_array_v<int[1][1][1]>);
			static_assert(is_array_v<std::array<int, 1>>);
			static_assert(is_array_v<std::array<std::array<int, 1>, 1>>);
			static_assert(is_array_v<std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}

		{// volatile arrays
			static_assert(is_array_v<volatile int[1]>);
			static_assert(is_array_v<volatile int[1][1]>);
			static_assert(is_array_v<volatile int[1][1][1]>);
			static_assert(is_array_v<volatile std::array<int, 1>>);
			static_assert(is_array_v<volatile std::array<std::array<int, 1>, 1>>);
			static_assert(is_array_v<volatile std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}

		{// const volatile arrays
			static_assert(is_array_v<const volatile int[1]>);
			static_assert(is_array_v<const volatile int[1][1]>);
			static_assert(is_array_v<const volatile int[1][1][1]>);
			static_assert(is_array_v<const volatile std::array<int, 1>>);
			static_assert(is_array_v<const volatile std::array<std::array<int, 1>, 1>>);
			static_assert(is_array_v<const volatile std::array<std::array<std::array<int, 1>, 1>, 1>>);
		}
	}

	constexpr void rank_checks()
	{
		static_assert(rank_v<int[1]> == 1);
		static_assert(rank_v<int[1][1]> == 2);
		static_assert(rank_v<int[1][1][1]> == 3);
		static_assert(rank_v<std::array<int, 1>> == 1);
		static_assert(rank_v<std::array<std::array<int, 1>, 1>> == 2);
		static_assert(rank_v<std::array<std::array<std::array<int, 1>, 1>, 1>> == 3);
	}

	constexpr void extent_checks()
	{
		static_assert(extent_v<int[1], 0> == 1);
		static_assert(std::is_same_v<remove_extent_t<int[1]>, int>);
		static_assert(std::is_same_v<remove_all_extents_t<int[1]>, int>);
		static_assert(std::is_same_v<extents_of_t<int[1]>, std::index_sequence<1>>);
		static_assert(total_items_v<int[1]> == 1);

		static_assert(extent_v<int[11], 0> == 11);
		static_assert(std::is_same_v<remove_extent_t<int[11]>, int>);
		static_assert(std::is_same_v<remove_all_extents_t<int[11]>, int>);
		static_assert(std::is_same_v<extents_of_t<int[11]>, std::index_sequence<11>>);
		static_assert(total_items_v<int[11]> == 11);

		static_assert(extent_v<int[2][4], 0> == 2);
		static_assert(extent_v<int[2][4], 1> == 4);
		static_assert(std::is_same_v<remove_extent_t<int[2][4]>, int[4]>);
		static_assert(std::is_same_v<remove_all_extents_t<int[2][4]>, int>);
		static_assert(std::is_same_v<extents_of_t<int[2][4]>, std::index_sequence<2, 4>>);
		static_assert(total_items_v<int[2][4]> == 2*4);

		static_assert(extent_v<int[9][3][4], 0> == 9);
		static_assert(extent_v<int[9][3][4], 1> == 3);
		static_assert(extent_v<int[9][3][4], 2> == 4);
		static_assert(std::is_same_v<remove_extent_t<int[9][3][4]>, int[3][4]>);
		static_assert(std::is_same_v<remove_all_extents_t<int[9][3][4]>, int>);
		static_assert(std::is_same_v<extents_of_t<int[9][3][4]>, std::index_sequence<9, 3, 4>>);
		static_assert(total_items_v<int[9][3][4]> == 9*3*4);

		static_assert(extent_v<std::array<int, 100>, 0> == 100);
		static_assert(std::is_same_v<remove_extent_t<std::array<int, 100>>, int>);
		static_assert(std::is_same_v<remove_all_extents_t<std::array<int, 100>>, int>);
		static_assert(std::is_same_v<extents_of_t<std::array<int, 100>>, std::index_sequence<100>>);
		static_assert(total_items_v<std::array<int, 100>> == 100);

		static_assert(extent_v<std::array<std::array<int, 22>, 11>, 0> == 11);
		static_assert(extent_v<std::array<std::array<int, 22>, 11>, 1> == 22);
		static_assert(std::is_same_v<remove_extent_t<std::array<std::array<int, 22>, 11>>, std::array<int, 22>>);
		static_assert(std::is_same_v<remove_all_extents_t<std::array<std::array<int, 22>, 11>>, int>);
		static_assert(std::is_same_v<extents_of_t<std::array<std::array<int, 22>, 11>>, std::index_sequence<11, 22>>);
		static_assert(total_items_v<std::array<std::array<int, 22>, 11>> == 11*22);

		static_assert(extent_v<std::array<std::array<std::array<int, 100>, 50>, 25>, 0> == 25);
		static_assert(extent_v<std::array<std::array<std::array<int, 100>, 50>, 25>, 1> == 50);
		static_assert(extent_v<std::array<std::array<std::array<int, 100>, 50>, 25>, 2> == 100);
		static_assert(std::is_same_v<remove_extent_t<std::array<std::array<std::array<int, 100>, 50>, 25>>, std::array<std::array<int, 100>, 50>>);
		static_assert(std::is_same_v<remove_all_extents_t<std::array<std::array<std::array<int, 100>, 50>, 25>>, int>);
		static_assert(std::is_same_v<extents_of_t<std::array<std::array<std::array<int, 100>, 50>, 25>>, std::index_sequence<25, 50, 100>>);
		static_assert(total_items_v<std::array<std::array<std::array<int, 100>, 50>, 25>> == 25*50*100);
	}

	constexpr void indexer_support_checks()
	{
		static_assert(std::is_same_v<indexer_of_t<int[1]>, std::pair<std::index_sequence<0>, std::index_sequence<1>>>);
		static_assert(std::is_same_v<indexer_of_t<int[10][20]>, std::pair<std::index_sequence<0, 0>, std::index_sequence<10, 20>>>);
		static_assert(std::is_same_v<indexer_of_t<int[5][6][3]>, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<5, 6, 3>>>);
		static_assert(std::is_same_v<indexer_of_t<std::array<int, 11>>, std::pair<std::index_sequence<0>, std::index_sequence<11>>>);
		static_assert(std::is_same_v<indexer_of_t<std::array<std::array<int, 5>, 12>>, std::pair<std::index_sequence<0, 0>, std::index_sequence<12, 5>>>);
		static_assert(std::is_same_v<indexer_of_t<std::array<std::array<std::array<int, 31>, 22>, 13>>, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<13, 22, 31>>>);

		using s1 = concat_sequence_t<std::index_sequence<0>>;
		static_assert(std::is_same_v<s1, std::index_sequence<0>>);
		using s2 = concat_sequence_t<std::index_sequence<0>, std::index_sequence<1>>;
		static_assert(std::is_same_v<s2, std::index_sequence<0, 1>>);
		using s3 = concat_sequence_t<std::index_sequence<0>, std::index_sequence<1, 2>>;
		static_assert(std::is_same_v<s3, std::index_sequence<0, 1, 2>>);
		using s4 = concat_sequence_t<std::index_sequence<0>, std::index_sequence<1, 2, 3, 4, 5>>;
		static_assert(std::is_same_v<s4, std::index_sequence<0, 1, 2, 3, 4, 5>>);
		using s5 = concat_sequence_t<std::index_sequence<2, 4>, std::index_sequence<6, 8>>;
		static_assert(std::is_same_v<s5, std::index_sequence<2, 4, 6, 8>>);

		static_assert(index_sequence_tail<std::index_sequence<0>>::value == 0);
		static_assert(index_sequence_tail<std::index_sequence<0, 1>>::value == 1);
		static_assert(index_sequence_tail<std::index_sequence<0, 1, 2>>::value == 2);
	}

	constexpr void indexer_usage_checks()
	{
		{// c-arrays
			using indexer0_a1 = indexer_of_t<int[3]>;
			static_assert(is_first_indexer_v<indexer0_a1>);
			static_assert(std::is_same_v<indexer0_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);
			using indexer1_a1 = next_indexer_t<indexer0_a1>;
			static_assert(std::is_same_v<indexer1_a1, std::pair<std::index_sequence<1>, std::index_sequence<3>>>);
			using indexer2_a1 = next_indexer_t<indexer1_a1>;
			static_assert(is_last_indexer_v<indexer2_a1>);
			static_assert(std::is_same_v<indexer2_a1, std::pair<std::index_sequence<2>, std::index_sequence<3>>>);
			using indexer3_a1 = next_indexer_t<indexer2_a1>;
			static_assert(std::is_same_v<indexer3_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);

			using indexer0_a2 = indexer_of_t<int[2][3]>;
			static_assert(is_first_indexer_v<indexer0_a2>);
			static_assert(std::is_same_v<indexer0_a2, std::pair<std::index_sequence<0, 0>, std::index_sequence<2, 3>>>);
			using indexer1_a2 = next_indexer_t<indexer0_a2>;
			static_assert(std::is_same_v<indexer1_a2, std::pair<std::index_sequence<1, 0>, std::index_sequence<2, 3>>>);
			using indexer2_a2 = next_indexer_t<indexer1_a2>;
			static_assert(std::is_same_v<indexer2_a2, std::pair<std::index_sequence<0, 1>, std::index_sequence<2, 3>>>);
			using indexer3_a2 = next_indexer_t<indexer2_a2>;
			static_assert(std::is_same_v<indexer3_a2, std::pair<std::index_sequence<1, 1>, std::index_sequence<2, 3>>>);
			using indexer4_a2 = next_indexer_t<indexer3_a2>;
			static_assert(std::is_same_v<indexer4_a2, std::pair<std::index_sequence<0, 2>, std::index_sequence<2, 3>>>);
			using indexer5_a2 = next_indexer_t<indexer4_a2>;
			static_assert(is_last_indexer_v<indexer5_a2>);
			static_assert(std::is_same_v<indexer5_a2, std::pair<std::index_sequence<1, 2>, std::index_sequence<2, 3>>>);
			using indexer6_a2 = next_indexer_t<indexer5_a2>;
			static_assert(std::is_same_v<indexer6_a2, std::pair<std::index_sequence<0, 0>, std::index_sequence<2, 3>>>);

			using indexer_0_a3 = indexer_of_t<int[2][3][2]>;
			static_assert(is_first_indexer_v<indexer_0_a3>);
			static_assert(std::is_same_v<indexer_0_a3, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_1_a3 = next_indexer_t<indexer_0_a3>;
			static_assert(std::is_same_v<indexer_1_a3, std::pair<std::index_sequence<1, 0, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_2_a3 = next_indexer_t<indexer_1_a3>;
			static_assert(std::is_same_v<indexer_2_a3, std::pair<std::index_sequence<0, 1, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_3_a3 = next_indexer_t<indexer_2_a3>;
			static_assert(std::is_same_v<indexer_3_a3, std::pair<std::index_sequence<1, 1, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_4_a3 = next_indexer_t<indexer_3_a3>;
			static_assert(std::is_same_v<indexer_4_a3, std::pair<std::index_sequence<0, 2, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_5_a3 = next_indexer_t<indexer_4_a3>;
			static_assert(std::is_same_v<indexer_5_a3, std::pair<std::index_sequence<1, 2, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_6_a3 = next_indexer_t<indexer_5_a3>;
			static_assert(std::is_same_v<indexer_6_a3, std::pair<std::index_sequence<0, 0, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer_7_a3 = next_indexer_t<indexer_6_a3>;
			static_assert(std::is_same_v<indexer_7_a3, std::pair<std::index_sequence<1, 0, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer_8_a3 = next_indexer_t<indexer_7_a3>;
			static_assert(std::is_same_v<indexer_8_a3, std::pair<std::index_sequence<0, 1, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer_9_a3 = next_indexer_t<indexer_8_a3>;
			static_assert(std::is_same_v<indexer_9_a3, std::pair<std::index_sequence<1, 1, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer10_a3 = next_indexer_t<indexer_9_a3>;
			static_assert(std::is_same_v<indexer10_a3, std::pair<std::index_sequence<0, 2, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer11_a3 = next_indexer_t<indexer10_a3>;
			static_assert(is_last_indexer_v<indexer11_a3>);
			static_assert(std::is_same_v<indexer11_a3, std::pair<std::index_sequence<1, 2, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer12_a3 = next_indexer_t<indexer11_a3>;
			static_assert(std::is_same_v<indexer12_a3, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<2, 3, 2>>>);
		}

		{// std arrays
			using indexer0_a1 = indexer_of_t<std::array<int, 3>>;
			static_assert(is_first_indexer_v<indexer0_a1>);
			static_assert(std::is_same_v<indexer0_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);
			using indexer1_a1 = next_indexer_t<indexer0_a1>;
			static_assert(std::is_same_v<indexer1_a1, std::pair<std::index_sequence<1>, std::index_sequence<3>>>);
			using indexer2_a1 = next_indexer_t<indexer1_a1>;
			static_assert(is_last_indexer_v<indexer2_a1>);
			static_assert(std::is_same_v<indexer2_a1, std::pair<std::index_sequence<2>, std::index_sequence<3>>>);
			using indexer3_a1 = next_indexer_t<indexer2_a1>;
			static_assert(std::is_same_v<indexer3_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);

			using indexer0_a2 = indexer_of_t<std::array<std::array<int, 3>, 2>>;
			static_assert(is_first_indexer_v<indexer0_a2>);
			static_assert(std::is_same_v<indexer0_a2, std::pair<std::index_sequence<0, 0>, std::index_sequence<2, 3>>>);
			using indexer1_a2 = next_indexer_t<indexer0_a2>;
			static_assert(std::is_same_v<indexer1_a2, std::pair<std::index_sequence<1, 0>, std::index_sequence<2, 3>>>);
			using indexer2_a2 = next_indexer_t<indexer1_a2>;
			static_assert(std::is_same_v<indexer2_a2, std::pair<std::index_sequence<0, 1>, std::index_sequence<2, 3>>>);
			using indexer3_a2 = next_indexer_t<indexer2_a2>;
			static_assert(std::is_same_v<indexer3_a2, std::pair<std::index_sequence<1, 1>, std::index_sequence<2, 3>>>);
			using indexer4_a2 = next_indexer_t<indexer3_a2>;
			static_assert(std::is_same_v<indexer4_a2, std::pair<std::index_sequence<0, 2>, std::index_sequence<2, 3>>>);
			using indexer5_a2 = next_indexer_t<indexer4_a2>;
			static_assert(is_last_indexer_v<indexer5_a2>);
			static_assert(std::is_same_v<indexer5_a2, std::pair<std::index_sequence<1, 2>, std::index_sequence<2, 3>>>);
			using indexer6_a2 = next_indexer_t<indexer5_a2>;
			static_assert(std::is_same_v<indexer6_a2, std::pair<std::index_sequence<0, 0>, std::index_sequence<2, 3>>>);

			using indexer_0_a3 = indexer_of_t<std::array<std::array<std::array<int, 2>, 3>, 2>>;
			static_assert(is_first_indexer_v<indexer_0_a3>);
			static_assert(std::is_same_v<indexer_0_a3, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_1_a3 = next_indexer_t<indexer_0_a3>;
			static_assert(std::is_same_v<indexer_1_a3, std::pair<std::index_sequence<1, 0, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_2_a3 = next_indexer_t<indexer_1_a3>;
			static_assert(std::is_same_v<indexer_2_a3, std::pair<std::index_sequence<0, 1, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_3_a3 = next_indexer_t<indexer_2_a3>;
			static_assert(std::is_same_v<indexer_3_a3, std::pair<std::index_sequence<1, 1, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_4_a3 = next_indexer_t<indexer_3_a3>;
			static_assert(std::is_same_v<indexer_4_a3, std::pair<std::index_sequence<0, 2, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_5_a3 = next_indexer_t<indexer_4_a3>;
			static_assert(std::is_same_v<indexer_5_a3, std::pair<std::index_sequence<1, 2, 0>, std::index_sequence<2, 3, 2>>>);
			using indexer_6_a3 = next_indexer_t<indexer_5_a3>;
			static_assert(std::is_same_v<indexer_6_a3, std::pair<std::index_sequence<0, 0, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer_7_a3 = next_indexer_t<indexer_6_a3>;
			static_assert(std::is_same_v<indexer_7_a3, std::pair<std::index_sequence<1, 0, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer_8_a3 = next_indexer_t<indexer_7_a3>;
			static_assert(std::is_same_v<indexer_8_a3, std::pair<std::index_sequence<0, 1, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer_9_a3 = next_indexer_t<indexer_8_a3>;
			static_assert(std::is_same_v<indexer_9_a3, std::pair<std::index_sequence<1, 1, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer10_a3 = next_indexer_t<indexer_9_a3>;
			static_assert(std::is_same_v<indexer10_a3, std::pair<std::index_sequence<0, 2, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer11_a3 = next_indexer_t<indexer10_a3>;
			static_assert(is_last_indexer_v<indexer11_a3>);
			static_assert(std::is_same_v<indexer11_a3, std::pair<std::index_sequence<1, 2, 1>, std::index_sequence<2, 3, 2>>>);
			using indexer12_a3 = next_indexer_t<indexer11_a3>;
			static_assert(std::is_same_v<indexer12_a3, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<2, 3, 2>>>);
		}
	}

	constexpr void indexer_access_checks()
	{
		{ // c-array, 1-dim
			constexpr int a[6]{1, 2, 3, 4, 5, 6};
			using idx0 = indexer_of_t<decltype(a)>;
			static_assert(get<idx0>(a) == 1);
			using idx1 = next_indexer_t<idx0>;
			static_assert(get<idx1>(a) == 2);
			using idx2 = next_indexer_t<idx1>;
			static_assert(get<idx2>(a) == 3);
			using idx3 = next_indexer_t<idx2>;
			static_assert(get<idx3>(a) == 4);
			using idx4 = next_indexer_t<idx3>;
			static_assert(get<idx4>(a) == 5);
			using idx5 = next_indexer_t<idx4>;
			static_assert(get<idx5>(a) == 6);
		}

		{ // c-array, 2-dim
			constexpr int a[3][4]{
				{10, 20, 30, 40},
				{11, 21, 31, 41},
				{12, 22, 32, 42},
			};

			using idx0 = indexer_of_t<decltype(a)>;
			static_assert(get<idx0>(a) == 10);
			using idx_last = last_indexer_of_t<decltype(a)>;
			static_assert(get<idx_last>(a) == 42);
		}

		{ // c-array 3-dim
			constexpr int a[2][3][5]{
				{//a[0]
					{111, 112, 113, 114, 115},
					{121, 122, 123, 124, 125},
					{131, 132, 133, 134, 135}
				},
				{//a[1]
					{211, 212, 213, 214, 215},
					{221, 222, 223, 224, 225},
					{231, 232, 233, 234, 235}
				}
			};

			using idx0 = indexer_of_t<decltype(a)>;
			static_assert(get<idx0>(a) == 111);
			using idx_last = last_indexer_of_t<decltype(a)>;
			static_assert(get<idx_last>(a) == 235);
		}

		{ // std array, 1-dim
			constexpr std::array<int, 6> a{1, 2, 3, 4, 5, 6};
			using idx0 = indexer_of_t<decltype(a)>;
			static_assert(get<idx0>(a) == 1);
			using idx1 = next_indexer_t<idx0>;
			static_assert(get<idx1>(a) == 2);
			using idx2 = next_indexer_t<idx1>;
			static_assert(get<idx2>(a) == 3);
			using idx3 = next_indexer_t<idx2>;
			static_assert(get<idx3>(a) == 4);
			using idx4 = next_indexer_t<idx3>;
			static_assert(get<idx4>(a) == 5);
			using idx5 = next_indexer_t<idx4>;
			static_assert(get<idx5>(a) == 6);
		}

		{ // std array, 2-dim
			constexpr std::array<std::array<int, 4>, 3> a{{
				{10, 20, 30, 40},
				{11, 21, 31, 41},
				{12, 22, 32, 42},
			}};

			using idx0 = indexer_of_t<decltype(a)>;
			static_assert(get<idx0>(a) == 10);
			using idx_last = last_indexer_of_t<decltype(a)>;
			static_assert(get<idx_last>(a) == 42);
		}

		{ // std array 3-dim
			constexpr std::array<std::array<std::array<int, 5>, 3>, 2> a{{
				{{//a[0]
					{111, 112, 113, 114, 115},
					{121, 122, 123, 124, 125},
					{131, 132, 133, 134, 135}
				}},
				{{//a[1]
					{211, 212, 213, 214, 215},
					{221, 222, 223, 224, 225},
					{231, 232, 233, 234, 235}
				}}
			}};

			using idx0 = indexer_of_t<decltype(a)>;
			static_assert(get<idx0>(a) == 111);
			using idx_last = last_indexer_of_t<decltype(a)>;
			static_assert(get<idx_last>(a) == 235);
		}
	}

	constexpr void offset_checks()
	{
		{ // c-array, 2-dim
			constexpr int a[3][4]{
				{10, 20, 30, 40},
				{11, 21, 31, 41},
				{12, 22, 32, 42},
			};

			using ext_a = extents_of_t<decltype(a)>;
			using idx0 = indexer_from_offset_t<0, ext_a>;
			static_assert(offset_from_indexer_v<idx0> == 0);

			static_assert(get<idx0>(a) == 10);
			using idx1 = indexer_from_offset_t<1, ext_a>;
			static_assert(offset_from_indexer_v<idx1> == 1);
			static_assert(get<idx1>(a) == 20);
			using idx2 = indexer_from_offset_t<2, ext_a>;
			static_assert(offset_from_indexer_v<idx2> == 2);
			static_assert(get<idx2>(a) == 30);
			using idx3 = indexer_from_offset_t<3, ext_a>;
			static_assert(offset_from_indexer_v<idx3> == 3);
			static_assert(get<idx3>(a) == 40);
			using idx4 = indexer_from_offset_t<4, ext_a>;
			static_assert(offset_from_indexer_v<idx4> == 4);
			static_assert(get<idx4>(a) == 11);
			using idx5 = indexer_from_offset_t<5, ext_a>;
			static_assert(offset_from_indexer_v<idx5> == 5);
			static_assert(get<idx5>(a) == 21);
			using idx6 = indexer_from_offset_t<6, ext_a>;
			static_assert(offset_from_indexer_v<idx6> == 6);
			static_assert(get<idx6>(a) == 31);
			using idx7 = indexer_from_offset_t<7, ext_a>;
			static_assert(offset_from_indexer_v<idx7> == 7);
			static_assert(get<idx7>(a) == 41);
			using idx8 = indexer_from_offset_t<8, ext_a>;
			static_assert(offset_from_indexer_v<idx8> == 8);
			static_assert(get<idx8>(a) == 12);
			using idx9 = indexer_from_offset_t<9, ext_a>;
			static_assert(offset_from_indexer_v<idx9> == 9);
			static_assert(get<idx9>(a) == 22);
			using idx10 = indexer_from_offset_t<10, ext_a>;
			static_assert(offset_from_indexer_v<idx10> == 10);
			static_assert(get<idx10>(a) == 32);
			using idx11 = indexer_from_offset_t<11, ext_a>;
			static_assert(offset_from_indexer_v<idx11> == 11);
			static_assert(get<idx11>(a) == 42);
		}
	}

#pragma region "variant experiments"
	// constexpr void iterator_checks()
	// {
	// 	{
	// 		using a = std::array<int, 4>;
	// 		using idx0 = indexer_of_t<a>;
	// 		using idx1 = next_indexer_t<idx0>;
	// 		using idx2 = next_indexer_t<idx1>;
	// 		using idx3 = next_indexer_t<idx2>;
	// 		using it = index_iterator_of_t<a>;
	// 		static_assert(std::is_same_v<it, std::variant<idx0, idx1, idx2, idx3>>);
	// 	}

	// 	{
	// 		using a = int[3][2][3];
	// 		using idx_0 = indexer_of_t<a>;
	// 		using idx_1 = next_indexer_t<idx_0>;
	// 		using idx_2 = next_indexer_t<idx_1>;
	// 		using idx_3 = next_indexer_t<idx_2>;
	// 		using idx_4 = next_indexer_t<idx_3>;
	// 		using idx_5 = next_indexer_t<idx_4>;
	// 		using idx_6 = next_indexer_t<idx_5>;
	// 		using idx_7 = next_indexer_t<idx_6>;
	// 		using idx_8 = next_indexer_t<idx_7>;
	// 		using idx_9 = next_indexer_t<idx_8>;
	// 		using idx10 = next_indexer_t<idx_9>;
	// 		using idx11 = next_indexer_t<idx10>;
	// 		using idx12 = next_indexer_t<idx11>;
	// 		using idx13 = next_indexer_t<idx12>;
	// 		using idx14 = next_indexer_t<idx13>;
	// 		using idx15 = next_indexer_t<idx14>;
	// 		using idx16 = next_indexer_t<idx15>;
	// 		using idx17 = next_indexer_t<idx16>;
	// 		using it = index_iterator_of_t<a>;
	// 		static_assert(std::is_same_v<it, std::variant<
	// 			idx_0, idx_1, idx_2, idx_3, idx_4, idx_5,
	// 			idx_6, idx_7, idx_8, idx_9, idx10, idx11,
	// 			idx12, idx13, idx14, idx15, idx16, idx17>>);
	// 	}
	// }

	// template <array A, typename Iter, typename Idx>
	// constexpr auto sum(const A& a, Iter iter)
	// {
	// 	using idx_t = decltype(std::visit([](auto&& idx) { return idx; }, iter));

	// 	if constexpr (is_last_indexer_v<idx_t>) {
	// 		return get<idx_t>(a);
	// 	}
	// 	else {
	// 		iter = next_indexer_t<idx_t>{};
	// 		return get<idx_t>(a) + sum(a, iter);
	// 	}
	// }

		// std::cout << sum(a) << '\n';
		// using It = index_iterator_of_t<decltype(a)>;

		// It it = indexer_of_t<decltype(a)>{};

		// int sigma{0};
		// for (std::size_t i{0}; i < total_items_v<decltype(a)>; ++i) {
		// 	std::cout << get(a, it) << '\n';
		// 	sigma += get(a, it);
		// 	// using idx_t = decltype(std::visit([](auto&& idx) { return idx; }, it));
		// 	// it = next_indexer_t<idx_t>{};
		// 	// std::cout << diagnostic::demangle<decltype(std::visit([](auto&& idx) { return idx; }, it))>() << '\n';
		// }
		// std::cout << '\n' << sigma << '\n';
#pragma endregion

#pragma region "namespace scoped test arrays"
	// 3-dim std::array
	constexpr std::array<std::array<std::array<int, 5>, 3>, 2> test_array_1{{
		{{//a[0]
			{1, 2, 3, 4, 5},
			{2, 3, 4, 5, 6},
			{3, 4, 5, 6, 7}
		}},
		{{//a[1]
			{3, 4, 5, 6, 7},
			{4, 5, 6, 7, 8},
			{5, 6, 7, 8, 9}
		}}
	}};//

	struct static_test_array_1 { constexpr static auto& value{test_array_1}; };

	// 2-dim c-array
	constexpr int test_array_2[3][4]{
		{10, 20, 30, 40},
		{11, 21, 31, 41},
		{12, 22, 32, 42},
	};

	struct static_test_array_2 { constexpr static auto& value{test_array_2}; };
#pragma endregion

	constexpr void find_checks()
	{
		using found_9 = decltype(static_find_if<static_test_array_1>([](const auto& item) constexpr { return item == 9; } ));
		static_assert(std::is_same_v<found_9, std::tuple<last_indexer_of_t<decltype(test_array_1)>>>);
		using rem_test1 = remove_indexer_t<last_indexer_of_t<decltype(test_array_1)>, found_9>;
		static_assert(std::is_same_v<rem_test1, std::tuple<>>);

		using ext_a = extents_of_t<decltype(test_array_1)>;
		using found_8 = decltype(static_find_if<static_test_array_1>([](const auto& item) constexpr { return item == 8; } ));
		using found_8_expected = std::tuple<indexer_from_offset_t<28, ext_a>, indexer_from_offset_t<24, ext_a>>;
		static_assert(std::is_same_v<found_8, found_8_expected>);
		using rem_test2 = remove_indexer_t<indexer_from_offset_t<28, ext_a>, found_8>;
		static_assert(std::is_same_v<rem_test2, std::tuple<indexer_from_offset_t<24, ext_a>>>);
		using rem_test3 = remove_indexer_t<indexer_from_offset_t<24, ext_a>, found_8>;
		static_assert(std::is_same_v<rem_test3, std::tuple<indexer_from_offset_t<28, ext_a>>>);
		using rem_test4 = remove_indexer_t<indexer_from_offset_t<24, ext_a>, rem_test2>;
		static_assert(std::is_same_v<rem_test4, std::tuple<>>);

		using found_less_5 = decltype(static_find_if<static_test_array_1>([](const auto& item) constexpr { return item < 5; } ));
		constexpr auto less_5{static_transform_to_array<static_test_array_1>(found_less_5{})};
		static_assert(less_5 == std::array<int, 12>{1, 3, 2, 4, 3, 2, 4, 3, 4, 3, 4, 4});//TODO: unintuitive index order.

		using min_a = decltype(static_find_min<static_test_array_1>());
		static_assert(get<min_a>(static_test_array_1::value) == 1);
	}

	constexpr void find_k_checks()
	{
		//NOTE: static_find_k_min<> should work regardless of array "topology", i.e.: std::array/c-array, ranks, and extents.
		//      unfortunately, for compile-time functionality, the arrays need to be wrapped in a type type (see: static_test_array_*).

		{//3-dim std::array tests
			constexpr auto min_a1_k1_sequence{static_find_k_min<1, static_test_array_1>()};
			constexpr auto result_min_a1_k1{static_transform_to_array<static_test_array_1>(min_a1_k1_sequence)};
			static_assert(result_min_a1_k1 == std::array<int, 1>{1});

			constexpr auto min_a1_k4_sequence{static_find_k_min<4, static_test_array_1>()};
			constexpr auto result_min_a1_k4{static_transform_to_array<static_test_array_1>(min_a1_k4_sequence)};
			static_assert(result_min_a1_k4 == std::array<int, 4>{1, 2, 2, 3});

			constexpr auto min_a1_k9_sequence{static_find_k_min<9, static_test_array_1>()};
			constexpr auto result_min_a1_k9{static_transform_to_array<static_test_array_1>(min_a1_k9_sequence)};
			static_assert(result_min_a1_k9 == std::array<int, 9>{1, 2, 2, 3, 3, 3, 3, 4, 4});
		}

		{//2-dim c-array tests
			constexpr auto min_a2_k1_sequence{static_find_k_min<1, static_test_array_2>()};
			constexpr auto result_min_a2_k1{static_transform_to_array<static_test_array_2>(min_a2_k1_sequence)};
			static_assert(result_min_a2_k1 == std::array<int, 1>{10});

			constexpr auto min_a2_k4_sequence{static_find_k_min<4, static_test_array_2>()};
			constexpr auto result_min_a2_k4{static_transform_to_array<static_test_array_2>(min_a2_k4_sequence)};
			static_assert(result_min_a2_k4 == std::array<int, 4>{10, 11, 12, 20});

			constexpr auto min_a2_k9_sequence{static_find_k_min<9, static_test_array_2>()};
			constexpr auto result_min_a2_k9{static_transform_to_array<static_test_array_2>(min_a2_k9_sequence)};
			static_assert(result_min_a2_k9 == std::array<int, 9>{10, 11, 12, 20, 21, 22, 30, 31, 32});
		}
	}

	constexpr void algo_checks()
	{
		constexpr std::array<int, 5> a1{2, 4, 6, 8, 10};
		static_assert(sum(a1) == 30);
		static_assert(accumulate(a1, 0, std::plus<int>{}) == 30);

		static_assert(product(a1) == 3840);
		static_assert(accumulate(a1, 1, std::multiplies<int>{}) == 3840);

		constexpr std::array<std::array<std::array<int, 5>, 3>, 2> a2{{
			{{//a[0]
				{111, 112, 113, 114, 115},
				{121, 122, 123, 124, 125},
				{131, 132, 133, 134, 135}
			}},
			{{//a[1]
				{211, 212, 213, 214, 215},
				{221, 222, 223, 224, 225},
				{231, 232, 233, 234, 235}
			}}
		}};
		static_assert(sum(a2) == 5190);

		constexpr int a3[2][3][5]{
			{//a[0]
				{111, 112, 113, 114, 115},
				{121, 122, 123, 124, 125},
				{131, 132, 133, 134, 135}
			},
			{//a[1]
				{211, 212, 213, 214, 215},
				{221, 222, 223, 224, 225},
				{231, 232, 233, 234, 235}
			}
		};
		static_assert(sum(a3) == 5190);

		constexpr std::array<std::array<std::array<unsigned long long, 5>, 3>, 2> a4{{
			{{//a[0]
				{1, 2, 3, 4, 5},
				{2, 3, 4, 5, 6},
				{3, 4, 5, 6, 7}
			}},
			{{//a[1]
				{3, 4, 5, 6, 7},
				{4, 5, 6, 7, 8},
				{5, 6, 7, 8, 9}
			}}
		}};
		static_assert(sum(a4) == 150);
		static_assert(product(a4) == 408614592055345152ull);
	}
}

int main()
{
	test::type_checks();
	test::rank_checks();
	test::extent_checks();
	test::indexer_support_checks();
	test::indexer_usage_checks();
	test::indexer_access_checks();
	test::offset_checks();
	test::find_checks();
	test::find_k_checks();
	test::algo_checks();

	std::cout << "###\n";

	return 0;
}
