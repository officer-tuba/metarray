#pragma once

#include <array>
#include <cstddef>
#include <numeric>
#include <type_traits>
#include <utility>

namespace c_array_meta {
namespace {
	template <std::size_t I, typename T, std::size_t S>
	constexpr void fill_extents(const T(&arr)[S], std::array<std::size_t, std::rank_v<T[S]>>& ranks)
	{
		ranks[I] = std::extent_v<T[S], I>;

		if constexpr (I > 0) {
			fill_extents<I - 1>(arr, ranks);
		}
	}

	template <std::size_t S, std::size_t E>
	constexpr std::size_t extent_offset_of(const std::array<std::size_t, S>& indices,
		const std::array<std::size_t, S>& extents)
	{
		if constexpr (E == 0) {
			return indices[0];
		}
		else {
			return indices[E] + (extents[E] * extent_offset_of<S, E - 1>(indices, extents));
		}
	}

	template <std::size_t S, std::size_t E = 0>
	constexpr std::size_t extent_index_of(std::size_t offset, const std::array<std::size_t, S>& extents)
	{
		static_assert(E < S);

		for (std::size_t ext_idx{E + 1}; ext_idx < S; ++ext_idx) {
			offset /= extents[ext_idx];
		}

		return offset % extents[E];
	}

	template <std::size_t S, std::size_t I = S - 1>
	constexpr void fill_indices(std::size_t offset, std::array<std::size_t, S>& indices, const std::array<std::size_t, S>& extents)
	{
		indices[I] = extent_index_of<S, I>(offset, extents);

		if constexpr (I > 0) {
			fill_indices<S, I - 1>(offset, indices, extents);
		}
	}

}//private namespace

	template <typename T, std::size_t S, typename TArray = T(&)[S]>
	using const_base_ptr_t = std::remove_all_extents_t<T> const*;

	template <typename T, std::size_t S, typename TArray = T(&)[S]>
	using base_ptr_t = std::remove_all_extents_t<T>*;

	template <typename T, std::size_t S, typename TArray = T(&)[S]>
	using const_base_ref_t = std::remove_all_extents_t<T> const&;

	template <typename T, std::size_t S, typename TArray = T(&)[S]>
	using base_ref_t = std::remove_all_extents_t<T>&;

	template <typename T, std::size_t S, typename TArray = T(&)[S]>
	using const_base_t = const std::remove_all_extents_t<T>;

	template <typename T, std::size_t S, typename TArray = T(&)[S]>
	using base_t = std::remove_all_extents_t<T>;

	template <typename T, std::size_t S>
	constexpr base_ptr_t<T, S> as_base_ptr(T(&arr)[S])
	{
		return reinterpret_cast<base_ptr_t<T, S>>(arr);
	}

	template <typename T, std::size_t S>
	constexpr const_base_ptr_t<T, S> as_base_ptr(const T(&arr)[S])
	{
		return reinterpret_cast<const_base_ptr_t<T, S>>(arr);
	}

	template <typename T, std::size_t S>
	constexpr std::array<std::size_t, std::rank_v<T[S]>> extents(const T(&arr)[S])
	{
		std::array<std::size_t, std::rank_v<T[S]>> ranks;
		fill_extents<std::rank_v<T[S]> - 1>(arr, ranks);
		return ranks;
	}

	template <typename T, std::size_t S>
	constexpr std::array<std::size_t, std::rank_v<T[S]>> indices(const T(&)[S])
	{
		return std::array<std::size_t, std::rank_v<T[S]>>{0};
	}

	template <typename T, std::size_t S>
	constexpr std::size_t total_items(const T(&arr)[S])
	{
		const auto arr_extents{extents(arr)};
		return std::accumulate(arr_extents.cbegin(), arr_extents.cend(), std::size_t{1},
			[](const auto t1, const auto t2){return t1 * t2;});
	}

	template <std::size_t S>
	bool increment_indices(std::array<std::size_t, S>& indices, const std::array<std::size_t, S>& extents)
	{// returns true if it incremented to the last element
		bool carry;
		std::size_t extent{extents.size() - 1};

		do {
			carry = false;

			if (++indices[extent] == extents[extent]) {
				indices[extent] = 0;

				if (extent > 0) {
					--extent;
				}
				else {
					return true;
				}

				carry = true;
			}
		} while(carry);

		return false;
	}

	template <std::size_t S>
	constexpr std::array<std::size_t, S> next_index(
		const std::array<std::size_t, S>& indices, const std::array<std::size_t, S>& extents)
	{
		bool carry;
		std::size_t extent{extents.size() - 1};
		auto next{indices};

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

		return next;
	}

	template <std::size_t S>
	constexpr std::size_t offset_of(const std::array<std::size_t, S>& indices, const std::array<std::size_t, S>& extents)
	{
		return extent_offset_of<S, S - 1>(indices, extents);
	}

	template <std::size_t S>
	constexpr std::array<std::size_t, S> index_of(std::size_t offset, const std::array<std::size_t, S>& extents)
	{
		std::array<std::size_t, S> indices;
		fill_indices(offset, indices, extents);
		return indices;
	}

	// template <typename T, std::size_t S, std::size_t...Is>
	// constexpr const_base_ref_t<T, S>
	// value(const T(&arr)[S], const std::array<std::size_t, sizeof...(Is)>& indices, const std::index_sequence<Is...>&)
	// {
		
	// }

	// template <std::size_t I, typename T, std::size_t S>
	// constexpr const_base_ref_t<T, S> value(const T(&arr)[S], std::size_t offset)
	// {
	// 	constexpr auto idx{index_of(offset, extents(arr))};

	// 	if constexpr (I == S - 1) {
	// 		return arr[I];
	// 	}
	// 	else {
	// 		return value<I + 1>(arr[I], offset);
	// 	}
	// }

	// template <typename T, std::size_t S>
	// constexpr auto value(const T(&arr)[S], std::size_t offset)
	// {
	// 	// return value(arr, index_of(offset, extents(arr)), std::make_index_sequence<std::rank_v<T[S]>>());

	// 	// if constexpr (std::rank_v<T[S]> == 1) {
	// 	// 	return arr[offset];
	// 	// }
	// 	// else {
	// 	// 	return value<0>(arr, offset);
	// 	// }

	// 	return arr[offset];
	// }

	template <typename T, std::size_t S>
	constexpr base_ref_t<T, S> at(T(&arr)[S], std::size_t offset)
	{//constexpr only works if if std::rank_v<T[S]> == 1
		return as_base_ptr<T, S>(arr)[offset];
	}

	template <typename T, std::size_t S>
	constexpr const_base_ref_t<T, S> at(const T(&arr)[S], std::size_t offset)
	{//constexpr only works if if std::rank_v<T[S]> == 1
		return as_base_ptr(arr)[offset];
	}

	template <typename T, std::size_t S>
	constexpr base_ref_t<T, S> at(T(&arr)[S], const std::array<std::size_t, std::rank_v<T[S]>>& indices)
	{//constexpr only works if if std::rank_v<T[S]> == 1
		return as_base_ptr<T, S>(arr)[offset_of(indices, extents(arr))];
	}

	template <typename T, std::size_t S>
	constexpr const_base_ref_t<T, S> at(const T(&arr)[S], const std::array<std::size_t, std::rank_v<T[S]>>& indices)
	{//constexpr only works if if std::rank_v<T[S]> == 1
		return as_base_ptr<T, S>(arr)[offset_of(indices, extents(arr))];
	}
}
