#ifndef __SORT_H__
#define __SORT_H__

#include "Heap.h"

enum Order
{
	Increasing,
	Decreasing
};

// Sort the elements between the iterators it_begin and it_end-1 included
template<typename T>
void HeapSort(T it_begin, T it_end, Order order = Order::Increasing)
{
	int num_elements = it_end - it_begin;
	if (order == Order::Increasing)
	{
		std::greater < T::value_type > comparator;
		for (T it = it_begin + (num_elements >> 1); it > it_begin; --it)
			Heapify(it, it_begin, it_end, comparator);
		Heapify(it_begin, it_begin, it_end, comparator);
		for (T it = it_end - 1; it > it_begin; --it)
		{
			T::value_type max = *it_begin;
			*it_begin = *it;
			*it = max;
			BubbleDownNonRecursive(it_begin, it_begin, it, comparator);
		}
	}
	else
	{
		std::less < T::value_type > comparator;
		for (T it = it_begin + (num_elements >> 1); it > it_begin; --it)
			Heapify(it, it_begin, it_end, comparator);
		Heapify(it_begin, it_begin, it_end, comparator);
		for (T it = it_end - 1; it > it_begin; --it)
		{
			T::value_type min = *it_begin;
			*it_begin = *it;
			*it = min;
			BubbleDownNonRecursive(it_begin, it_begin, it, comparator);
		}
	}
	
}
#endif