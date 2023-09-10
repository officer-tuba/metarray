#include <array>
#include <iostream>
#include <type_traits>
#include <utility>
#include "array_meta.hpp"
// #include "demangle.hpp"

namespace test {
	using namespace array_meta;

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
			const int a1[1]{};
			static_assert(not is_std_array_v<decltype(a1)>);

			const int a2[1][1]{};
			static_assert(not is_std_array_v<decltype(a2)>);

			const int a3[1][1][1]{};
			static_assert(not is_std_array_v<decltype(a3)>);

			const std::array<int, 1> a4{};
			static_assert(is_std_array_v<decltype(a4)>);

			const std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_std_array_v<decltype(a5)>);

			const std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_std_array_v<decltype(a6)>);
		}

		{// std_arrays
			int a1[1]{};
			static_assert(not is_std_array_v<decltype(a1)>);

			int a2[1][1]{};
			static_assert(not is_std_array_v<decltype(a2)>);

			int a3[1][1][1]{};
			static_assert(not is_std_array_v<decltype(a3)>);

			std::array<int, 1> a4{};
			static_assert(is_std_array_v<decltype(a4)>);

			std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_std_array_v<decltype(a5)>);

			std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_std_array_v<decltype(a6)>);
		}

		{// volatile std_arrays
			volatile int a1[1]{};
			static_assert(not is_std_array_v<decltype(a1)>);

			volatile int a2[1][1]{};
			static_assert(not is_std_array_v<decltype(a2)>);

			volatile int a3[1][1][1]{};
			static_assert(not is_std_array_v<decltype(a3)>);

			volatile std::array<int, 1> a4{};
			static_assert(is_std_array_v<decltype(a4)>);

			volatile std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_std_array_v<decltype(a5)>);

			volatile std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_std_array_v<decltype(a6)>);
		}

		{// const volatile std_arrays
			const volatile int a1[1]{};
			static_assert(not is_std_array_v<decltype(a1)>);

			const volatile int a2[1][1]{};
			static_assert(not is_std_array_v<decltype(a2)>);

			const volatile int a3[1][1][1]{};
			static_assert(not is_std_array_v<decltype(a3)>);

			const volatile std::array<int, 1> a4{};
			static_assert(is_std_array_v<decltype(a4)>);

			const volatile std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_std_array_v<decltype(a5)>);

			const volatile std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_std_array_v<decltype(a6)>);
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
			const int a1[1]{};
			static_assert(is_array_v<decltype(a1)>);

			const int a2[1][1]{};
			static_assert(is_array_v<decltype(a2)>);

			const int a3[1][1][1]{};
			static_assert(is_array_v<decltype(a3)>);

			const std::array<int, 1> a4{};
			static_assert(is_array_v<decltype(a4)>);

			const std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_array_v<decltype(a5)>);

			const std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_array_v<decltype(a6)>);
		}

		{// arrays
			int a1[1]{};
			static_assert(is_array_v<decltype(a1)>);

			int a2[1][1]{};
			static_assert(is_array_v<decltype(a2)>);

			int a3[1][1][1]{};
			static_assert(is_array_v<decltype(a3)>);

			std::array<int, 1> a4{};
			static_assert(is_array_v<decltype(a4)>);

			std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_array_v<decltype(a5)>);

			std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_array_v<decltype(a6)>);
		}

		{// volatile arrays
			volatile int a1[1]{};
			static_assert(is_array_v<decltype(a1)>);

			volatile int a2[1][1]{};
			static_assert(is_array_v<decltype(a2)>);

			volatile int a3[1][1][1]{};
			static_assert(is_array_v<decltype(a3)>);

			volatile std::array<int, 1> a4{};
			static_assert(is_array_v<decltype(a4)>);

			volatile std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_array_v<decltype(a5)>);

			volatile std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_array_v<decltype(a6)>);
		}

		{// const volatile arrays
			const volatile int a1[1]{};
			static_assert(is_array_v<decltype(a1)>);

			const volatile int a2[1][1]{};
			static_assert(is_array_v<decltype(a2)>);

			const volatile int a3[1][1][1]{};
			static_assert(is_array_v<decltype(a3)>);

			const volatile std::array<int, 1> a4{};
			static_assert(is_array_v<decltype(a4)>);

			const volatile std::array<std::array<int, 1>, 1> a5{};
			static_assert(is_array_v<decltype(a5)>);

			const volatile std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
			static_assert(is_array_v<decltype(a6)>);
		}
	}

	constexpr void rank_checks()
	{
		constexpr int a1[1]{};
		static_assert(rank_v<decltype(a1)> == 1);

		constexpr int a2[1][1]{};
		static_assert(rank_v<decltype(a2)> == 2);

		constexpr int a3[1][1][1]{};
		static_assert(rank_v<decltype(a3)> == 3);

		constexpr std::array<int, 1> a4{};
		static_assert(rank_v<decltype(a4)> == 1);

		constexpr std::array<std::array<int, 1>, 1> a5{};
		static_assert(rank_v<decltype(a5)> == 2);

		constexpr std::array<std::array<std::array<int, 1>, 1>, 1> a6{};
		static_assert(rank_v<decltype(a6)> == 3);
	}

	constexpr void extent_checks()
	{
		constexpr int a1[1]{};
		static_assert(extent_v<decltype(a1), 0> == 1);
		static_assert(std::is_same_v<remove_extent_t<decltype(a1)>, const int>);
		static_assert(std::is_same_v<remove_all_extents_t<decltype(a1)>, const int>);
		static_assert(std::is_same_v<extents_of_t<decltype(a1)>, std::index_sequence<1>>);
		static_assert(total_items_v<decltype(a1)> == 1);

		constexpr int a2[11]{};
		static_assert(extent_v<decltype(a2), 0> == 11);
		static_assert(std::is_same_v<remove_extent_t<decltype(a2)>, const int>);
		static_assert(std::is_same_v<remove_all_extents_t<decltype(a2)>, const int>);
		static_assert(std::is_same_v<extents_of_t<decltype(a2)>, std::index_sequence<11>>);
		static_assert(total_items_v<decltype(a2)> == 11);

		constexpr int a3[2][4]{};
		static_assert(extent_v<decltype(a3), 0> == 2);
		static_assert(extent_v<decltype(a3), 1> == 4);
		static_assert(std::is_same_v<remove_extent_t<decltype(a3)>, const int[4]>);
		static_assert(std::is_same_v<remove_all_extents_t<decltype(a3)>, const int>);
		static_assert(std::is_same_v<extents_of_t<decltype(a3)>, std::index_sequence<2, 4>>);
		static_assert(total_items_v<decltype(a3)> == 2*4);

		constexpr int a4[9][3][4]{};
		static_assert(extent_v<decltype(a4), 0> == 9);
		static_assert(extent_v<decltype(a4), 1> == 3);
		static_assert(extent_v<decltype(a4), 2> == 4);
		static_assert(std::is_same_v<remove_extent_t<decltype(a4)>, const int[3][4]>);
		static_assert(std::is_same_v<remove_all_extents_t<decltype(a4)>, const int>);
		static_assert(std::is_same_v<extents_of_t<decltype(a4)>, std::index_sequence<9, 3, 4>>);
		static_assert(total_items_v<decltype(a4)> == 9*3*4);

		constexpr std::array<int, 100> a5{};
		static_assert(extent_v<decltype(a5), 0> == 100);
		static_assert(std::is_same_v<remove_extent_t<decltype(a5)>, int>);
		static_assert(std::is_same_v<remove_all_extents_t<decltype(a5)>, int>);
		static_assert(std::is_same_v<extents_of_t<decltype(a5)>, std::index_sequence<100>>);
		static_assert(total_items_v<decltype(a5)> == 100);

		constexpr std::array<std::array<int, 22>, 11> a6{};
		static_assert(extent_v<decltype(a6), 0> == 11);
		static_assert(extent_v<decltype(a6), 1> == 22);
		static_assert(std::is_same_v<remove_extent_t<decltype(a6)>, std::array<int, 22>>);
		static_assert(std::is_same_v<remove_all_extents_t<decltype(a6)>, int>);
		static_assert(std::is_same_v<extents_of_t<decltype(a6)>, std::index_sequence<11, 22>>);
		static_assert(total_items_v<decltype(a6)> == 11*22);

		constexpr std::array<std::array<std::array<int, 100>, 50>, 25> a7{};
		static_assert(extent_v<decltype(a7), 0> == 25);
		static_assert(extent_v<decltype(a7), 1> == 50);
		static_assert(extent_v<decltype(a7), 2> == 100);
		static_assert(std::is_same_v<remove_extent_t<decltype(a7)>, std::array<std::array<int, 100>, 50>>);
		static_assert(std::is_same_v<remove_all_extents_t<decltype(a7)>, int>);
		static_assert(std::is_same_v<extents_of_t<decltype(a7)>, std::index_sequence<25, 50, 100>>);
		static_assert(total_items_v<decltype(a7)> == 25*50*100);
	}

	constexpr void indexer_support_checks()
	{
		constexpr int a1[1]{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a1)>, std::pair<std::index_sequence<0>, std::index_sequence<1>>>);
		constexpr int a2[10][20]{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a2)>, std::pair<std::index_sequence<0, 0>, std::index_sequence<10, 20>>>);
		constexpr int a3[5][6][3]{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a3)>, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<5, 6, 3>>>);
		constexpr std::array<int, 11> a4{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a4)>, std::pair<std::index_sequence<0>, std::index_sequence<11>>>);
		constexpr std::array<std::array<int, 5>, 12> a5{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a5)>, std::pair<std::index_sequence<0, 0>, std::index_sequence<12, 5>>>);
		constexpr std::array<std::array<std::array<int, 31>, 22>, 13> a6{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a6)>, std::pair<std::index_sequence<0, 0, 0>, std::index_sequence<13, 22, 31>>>);

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
			constexpr int a1[3]{};
			using indexer0_a1 = indexer_of_t<decltype(a1)>;
			static_assert(is_first_indexer_v<indexer0_a1>);
			static_assert(std::is_same_v<indexer0_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);
			using indexer1_a1 = next_indexer_t<indexer0_a1>;
			static_assert(std::is_same_v<indexer1_a1, std::pair<std::index_sequence<1>, std::index_sequence<3>>>);
			using indexer2_a1 = next_indexer_t<indexer1_a1>;
			static_assert(is_last_indexer_v<indexer2_a1>);
			static_assert(std::is_same_v<indexer2_a1, std::pair<std::index_sequence<2>, std::index_sequence<3>>>);
			using indexer3_a1 = next_indexer_t<indexer2_a1>;
			static_assert(std::is_same_v<indexer3_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);

			constexpr int a2[2][3]{};
			using indexer0_a2 = indexer_of_t<decltype(a2)>;
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

			constexpr int a3[2][3][2]{};
			using indexer_0_a3 = indexer_of_t<decltype(a3)>;
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
			constexpr std::array<int, 3> a1{};
			using indexer0_a1 = indexer_of_t<decltype(a1)>;
			static_assert(is_first_indexer_v<indexer0_a1>);
			static_assert(std::is_same_v<indexer0_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);
			using indexer1_a1 = next_indexer_t<indexer0_a1>;
			static_assert(std::is_same_v<indexer1_a1, std::pair<std::index_sequence<1>, std::index_sequence<3>>>);
			using indexer2_a1 = next_indexer_t<indexer1_a1>;
			static_assert(is_last_indexer_v<indexer2_a1>);
			static_assert(std::is_same_v<indexer2_a1, std::pair<std::index_sequence<2>, std::index_sequence<3>>>);
			using indexer3_a1 = next_indexer_t<indexer2_a1>;
			static_assert(std::is_same_v<indexer3_a1, std::pair<std::index_sequence<0>, std::index_sequence<3>>>);

			constexpr std::array<std::array<int, 3>, 2> a2{};
			using indexer0_a2 = indexer_of_t<decltype(a2)>;
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

			constexpr std::array<std::array<std::array<int, 2>, 3>, 2> a3{};
			using indexer_0_a3 = indexer_of_t<decltype(a3)>;
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

	std::cout << "###\n";

	return 0;
}
