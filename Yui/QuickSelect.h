#ifndef __QUICK_SELECT_H__
#define __QUICK_SELECT_H__

#include <algorithm>

namespace Yui
{
	template<typename T>
	T Partition(T it_begin, T it_end, T pivot)
	{
		T::value_type pivot_value = *pivot;
		std::swap(*pivot, *it_end);
		T pivot_index = it_begin;
		for (T it = it_begin; it < it_end; ++it)
		{
			if (*it < pivot_value)
			{
				std::swap(*pivot_index, *it);
				++pivot_index;
			}
		}
		std::swap(*pivot_index, *it_end);
		return pivot_index;
	}

	// Returns the kth smallest element between it_begin and it_end-1 inclusive
	template<typename T>
	T QuickSelect(T it_begin, T it_end, unsigned int k)
	{
		--it_end;
		if (it_begin >= it_end)
			return it_begin;
		T it_begin_ref = it_begin;
		while (true)
		{
			T pivot = Partition(it_begin, it_end, it_end);
			if (pivot - it_begin_ref == k)
				return pivot;
			else if (pivot - it_begin_ref < k)
				it_begin = pivot + 1;
			else
				it_end = pivot - 1;
		}
	}

	// Returns the kth largest element between it_begin and it_end-1 inclusive
	template<typename T>
	T QuickSelectMax(T it_begin, T it_end, unsigned int k)
	{
		--it_end;
		if (it_begin >= it_end)
			return it_begin;
		T it_end_ref = it_end;
		while (true)
		{
			T pivot = Partition(it_begin, it_end, it_end);
			if (it_end_ref - pivot == k)
				return pivot;
			else if (it_end_ref - pivot < k)
				it_end = pivot - 1;
			else
				it_begin = pivot + 1;
		}
	}
};

#endif