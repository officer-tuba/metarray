#include <array>
#include <iostream>
#include <type_traits>
#include <utility>
#include "array_meta.hpp"
#include "demangle.hpp"

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

	constexpr void indexer_creation()
	{
		constexpr int a1[1]{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a1)>, std::index_sequence<0>>);

		constexpr int a2[10][20]{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a2)>, std::index_sequence<0, 0>>);

		constexpr int a3[5][6][3]{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a3)>, std::index_sequence<0, 0, 0>>);

		constexpr std::array<int, 11> a4{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a4)>, std::index_sequence<0>>);

		constexpr std::array<std::array<int, 5>, 12> a5{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a5)>, std::index_sequence<0, 0>>);

		constexpr std::array<std::array<std::array<int, 31>, 22>, 13> a6{};
		static_assert(std::is_same_v<indexer_of_t<decltype(a6)>, std::index_sequence<0, 0, 0>>);

		using s1 = concat_sequence<std::index_sequence<0>>::type;
		static_assert(std::is_same_v<s1, std::index_sequence<0>>);
		using s2 = concat_sequence<std::index_sequence<0>, std::index_sequence<1>>::type;
		static_assert(std::is_same_v<s2, std::index_sequence<0, 1>>);
		using s3 = concat_sequence<std::index_sequence<0>, std::index_sequence<1, 2>>::type;
		static_assert(std::is_same_v<s3, std::index_sequence<0, 1, 2>>);
		using s4 = concat_sequence<std::index_sequence<0>, std::index_sequence<1, 2, 3, 4, 5>>::type;
		static_assert(std::is_same_v<s4, std::index_sequence<0, 1, 2, 3, 4, 5>>);
		using s5 = concat_sequence<std::index_sequence<2, 4>, std::index_sequence<6, 8>>::type;
		static_assert(std::is_same_v<s5, std::index_sequence<2, 4, 6, 8>>);

		static_assert(index_sequence_tail<std::index_sequence<0>>::value == 0);
		static_assert(index_sequence_tail<std::index_sequence<0, 1>>::value == 1);
		static_assert(index_sequence_tail<std::index_sequence<0, 1, 2>>::value == 2);

		constexpr int a7[3]{};
		using extents_a7 = extents_of_t<decltype(a7)>;
		using indexer0_a7 = indexer_of_t<decltype(a7)>;
		using indexer1_a7 = next_index<indexer0_a7, extents_a7>::type;
		using indexer2_a7 = next_index<indexer1_a7, extents_a7>::type;
		using indexer3_a7 = next_index<indexer2_a7, extents_a7>::type;
		static_assert(std::is_same_v<indexer0_a7, indexer3_a7>);

		std::cout << diagnostic::demangle<indexer0_a7>() << '\n';
		std::cout << diagnostic::demangle<indexer1_a7>() << '\n';
		std::cout << diagnostic::demangle<indexer2_a7>() << '\n';
		std::cout << diagnostic::demangle<indexer3_a7>() << '\n';

		constexpr int a8[2][3]{};
		using extents_a8 = extents_of_t<decltype(a8)>;
		using indexer0_a8 = indexer_of_t<decltype(a8)>;
		static_assert(std::is_same_v<indexer0_a8, std::index_sequence<0, 0>>);
		using indexer1_a8 = next_index<indexer0_a8, extents_a8>::type;
		using indexer2_a8 = next_index<indexer1_a8, extents_a8>::type;
		using indexer3_a8 = next_index<indexer2_a8, extents_a8>::type;
		using indexer4_a8 = next_index<indexer3_a8, extents_a8>::type;
		using indexer5_a8 = next_index<indexer4_a8, extents_a8>::type;
		using indexer6_a8 = next_index<indexer5_a8, extents_a8>::type;
		
		std::cout << diagnostic::demangle<indexer0_a8>() << '\n';
		std::cout << diagnostic::demangle<indexer1_a8>() << '\n';
		std::cout << diagnostic::demangle<indexer2_a8>() << '\n';
		std::cout << diagnostic::demangle<indexer3_a8>() << '\n';
		std::cout << diagnostic::demangle<indexer4_a8>() << '\n';
		std::cout << diagnostic::demangle<indexer5_a8>() << '\n';
		std::cout << diagnostic::demangle<indexer6_a8>() << '\n';
	}
}

int main()
{
	test::type_checks();
	test::rank_checks();
	test::extent_checks();
	test::indexer_creation();

	return 0;
}
