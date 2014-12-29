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

	// Returns the kth largest element between it_begin and it_end-1 inclusive
	template<typename T>
	T QuickSelect(T it_begin, T it_end, unsigned int k)
	{
		while (it_begin != it_end)
		{
			T pivot = Partition(it_begin, it_end - 1, it_end - 1);
			if (pivot - it_begin == k)
				return it_begin;
			else if (pivot - it_begin < k)
				it_begin = pivot + 1;
			else
				it_end = pivot - 1;
		}
		return it_begin;
	}
};

#endif