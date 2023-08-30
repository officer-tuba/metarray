#include <array>
#include <iostream>
#include "std_array_meta.hpp"

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
		static_assert(array_meta::total_items(a) == 7*8*9);

		constexpr auto ext{array_meta::extents_of(a)};
		static_assert(ext.size() == 3);
		static_assert(ext[0] == 9);
		static_assert(ext[1] == 8);
		static_assert(ext[2] == 7);

		constexpr auto idx{array_meta::indexer(a)};
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

		constexpr auto idx0{array_meta::indexer(a)};
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
	}

	return 0;
}
// reset && make && build/array-picker && printf '\n###\n' || printf '\nfailed\n'
