#ifndef __SORT_H__
#define __SORT_H__

#include "Heap.h"

namespace Yui
{
	template<typename T>
	bool IsLess(T &a, T &b)
	{
		return a < b;
	}

	enum Order
	{
		Increasing,
		Decreasing
	};

	// Sort the elements between the iterators it_begin and it_end-1 inclusive
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

	template<typename T, typename Comparator>
	void Merge(T it_array_1, size_t array_1_size, T it_array_2, size_t array_2_size, Comparator comparator)
	{
		T it_1 = it_array_1;
		T it_2 = it_array_2;
		T it_1_end = it_array_1 + array_1_size;
		T it_2_end = it_array_2 + array_2_size;

		T::value_type *buffer = new T::value_type[array_1_size + array_2_size];
		for (size_t buffer_idx = 0; buffer_idx < array_1_size + array_2_size; ++buffer_idx)
		{
			if (it_2 == it_2_end || comparator(*it_1, *it_2))
			{
				buffer[buffer_idx] = *it_1;
				++it_1;
			}
			else if (it_1 == it_1_end || comparator(*it_2, *it_1))
			{
				buffer[buffer_idx] = *it_2;
				++it_2;
			}
		}
		for (size_t buffer_idx = 0; buffer_idx < array_1_size + array_2_size; ++buffer_idx)
			*(it_array_1 + buffer_idx) = buffer[buffer_idx];
		delete buffer;
	}

	// Sort the elements between the iterators it_begin and it_end inclusive
	template<typename T, typename Comparator>
	T InternalMergeSort(T it_begin, T it_end, Comparator comparator)
	{
		int range = it_end - it_begin;
		if (range == 0)
			return it_begin;
		T it_middle = it_begin + (range >> 1);
		T sorted_array_1 = InternalMergeSort(it_begin, it_middle, comparator);
		T sorted_array_2 = InternalMergeSort(it_middle + 1, it_end, comparator);
		Merge(sorted_array_1, it_middle - it_begin + 1, sorted_array_2, it_end - it_middle, comparator);
		return it_begin;
	}

	// Sort the elements between the iterators it_begin and it_end-1 inclusive
	template<typename T, typename Comparator>
	void MergeSort(T it_begin, T it_end, Comparator comparator)
	{
		if (it_begin < it_end)
			InternalMergeSort(it_begin, it_end - 1, comparator);
	}

	template<typename T>
	void MergeSort(T it_begin, T it_end)
	{
		MergeSort(it_begin, it_end, IsLess<T::value_type>);
	}
};

#endif