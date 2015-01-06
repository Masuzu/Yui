#ifndef __SORT_H__
#define __SORT_H__

#include "Heap.h"

#ifndef __MERGE_SORT_GRANULARITY
// Yui::MergeSort will fall back to std::sort when there are less than __MERGE_SORT_GRANULARITY elements.
#define __MERGE_SORT_GRANULARITY 100
#endif

// Uncomment to divide the two calls to merge sort (on the left and right side of the array to sort) among the available threads
#define __MERGE_SORT_OMP_SECTIONS

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
			if (it_2 == it_2_end || (it_1 != it_1_end && comparator(*it_1, *it_2)))
			{
				buffer[buffer_idx] = *it_1;
				++it_1;
			}
			else
			{
				buffer[buffer_idx] = *it_2;
				++it_2;
			}
		}
		for (size_t buffer_idx = 0; buffer_idx < array_1_size + array_2_size; ++buffer_idx)
			*(it_array_1 + buffer_idx) = buffer[buffer_idx];
		delete buffer;
	}

	template<typename T, typename U, typename Comparator>
	void MergeInto(T it_array_1, size_t array_1_size, T it_array_2, size_t array_2_size, Comparator comparator, U it_output)
	{
		T it_1 = it_array_1;
		T it_2 = it_array_2;
		T it_1_end = it_array_1 + array_1_size;
		T it_2_end = it_array_2 + array_2_size;

		for (size_t buffer_idx = 0; buffer_idx < array_1_size + array_2_size; ++buffer_idx)
		{
			if (it_2 == it_2_end || (it_1 != it_1_end && comparator(*it_1, *it_2)))
			{
				*(it_output + buffer_idx) = *it_1;
				++it_1;
			}
			else
			{
				*(it_output + buffer_idx) = *it_2;
				++it_2;
			}
		}
	}

	template<typename T, typename V, typename Comparator>
	T BinarySearch(V value, T it, size_t array_size, Comparator comparator)
	{
		T it_end = it + array_size;
		while (it < it_end)
		{
			T it_mid = it + ((it_end - it) / 2);
			if (comparator(*it_mid, value))
				it = it_mid + 1;
			else
				it_end = it_mid;
		}
		return it_end;
	}

	template<typename T, typename Comparator>
	void PMerge(T it_array_1, size_t array_1_size, T it_array_2, size_t array_2_size, Comparator comparator, unsigned int threads)
	{
		T::value_type *buffer = new T::value_type[array_1_size + array_2_size];
		PMergeInto(it_array_1, array_1_size, it_array_2, array_2_size, comparator, buffer, threads);
		for (size_t buffer_idx = 0; buffer_idx < array_1_size + array_2_size; ++buffer_idx)
			*(it_array_1 + buffer_idx) = buffer[buffer_idx];
		delete buffer;
	}

	template<typename T, typename U, typename Comparator>
	void PMergeInto(T it_array_1, size_t array_1_size, T it_array_2, size_t array_2_size, Comparator comparator, U it_output, unsigned int threads)
	{
		// Ensure that the first array has more element than the second one
		if (array_1_size < array_2_size)
		{
			std::swap(it_array_1, it_array_2);
			std::swap(array_1_size, array_2_size);
		}
		if (array_1_size == 0)
			return;
		else
		{
			T it_pivot = it_array_1 + (array_1_size >> 1);
			T it_pivot_2 = BinarySearch(*it_pivot, it_array_2, array_2_size, comparator);
			U it_pivot_in_output_array = it_output + ((it_pivot - it_array_1) + (it_pivot_2 - it_array_2));
			*it_pivot_in_output_array = *it_pivot;

			int left_size = (array_1_size >> 1);
			int right_size = it_pivot_2 - it_array_2;

			if (threads > 1)
			{
#pragma omp parallel sections num_threads(threads) 
				{
#pragma omp section
					{
						PMergeInto(it_array_1, left_size, it_array_2, right_size, comparator, it_output, threads >> 1);
					}
#pragma omp section
					{
						PMergeInto(it_pivot + 1, array_1_size - left_size - 1, it_pivot_2, array_2_size - right_size, comparator, it_pivot_in_output_array + 1, threads - (threads >> 1));
					}
				}
			}
			else
			{
				PMergeInto(it_array_1, left_size, it_array_2, right_size, comparator, it_output, 1);
				PMergeInto(it_pivot + 1, array_1_size - left_size - 1, it_pivot_2, array_2_size - right_size, comparator, it_pivot_in_output_array + 1, 1);
			}
		}
	}

	// Sort the elements between the iterators it_begin and it_end inclusive
	template<typename T, typename Comparator>
	T InternalMergeSort(T it_begin, T it_end, Comparator comparator, unsigned int threads)
	{
		int range = it_end - it_begin;
		if (range == 0)
			return it_begin;
		if (range < __MERGE_SORT_GRANULARITY)
		{
			std::sort(it_begin, it_end + 1);
			return it_begin;
		}
		T it_middle = it_begin + (range >> 1);
		T sorted_array_1;
		T sorted_array_2;
#ifdef __MERGE_SORT_OMP_SECTIONS
		if (threads > 1)
		{
#pragma omp parallel sections num_threads(threads) 
			{
#pragma omp section
				{
					sorted_array_1 = InternalMergeSort(it_begin, it_middle, comparator, threads >> 1);
				}
#pragma omp section
				{
					sorted_array_2 = InternalMergeSort(it_middle + 1, it_end, comparator, threads - (threads >> 1));
				}
			}
		}
		else
		{
			sorted_array_1 = InternalMergeSort(it_begin, it_middle, comparator, 1);
			sorted_array_2 = InternalMergeSort(it_middle + 1, it_end, comparator, 1);
		}
#else
#pragma omp task firstprivate(it_begin, it_middle, comparator)
		{
			sorted_array_1 = InternalMergeSort(it_begin, it_middle, comparator, threads);
		}
#pragma omp task firstprivate(it_middle, it_end, comparator)
		{
			sorted_array_2 = InternalMergeSort(it_middle + 1, it_end, comparator, threads);
		}
#pragma omp taskwait
#endif
		// Merge the sorted halves
		if (threads > 1)
			PMerge(sorted_array_1, it_middle - it_begin + 1, sorted_array_2, it_end - it_middle, comparator, threads);
		else
			Merge(sorted_array_1, it_middle - it_begin + 1, sorted_array_2, it_end - it_middle, comparator);

		return it_begin;
	}

	// Sort the elements between the iterators it_begin and it_end inclusive
	template<typename T, typename U, typename Comparator>
	T InternalMergeSort(T it_begin, T it_end, Comparator comparator, U output, unsigned int threads)
	{
		int range = it_end - it_begin;
		if (range == 0)
			return it_begin;
		if (range < __MERGE_SORT_GRANULARITY)
		{
			std::sort(it_begin, it_end + 1);
			return it_begin;
		}
		T it_middle = it_begin + (range >> 1);
		T sorted_array_1;
		T sorted_array_2;
#ifdef __MERGE_SORT_OMP_SECTIONS
		if (threads > 1)
		{
#pragma omp parallel sections num_threads(threads) 
			{
#pragma omp section
				{
					sorted_array_1 = InternalMergeSort(it_begin, it_middle, comparator, output, threads >> 1);
				}
#pragma omp section
				{
					sorted_array_2 = InternalMergeSort(it_middle + 1, it_end, comparator, output + ((it_middle + 1) - it_begin), threads - (threads >> 1));
				}
			}
		}
		else
		{
			sorted_array_1 = InternalMergeSort(it_begin, it_middle, comparator, output, 1);
			sorted_array_2 = InternalMergeSort(it_middle + 1, it_end, comparator, output + ((it_middle + 1) - it_begin), 1);
		}
#else
#pragma omp task firstprivate(it_begin, it_middle, comparator)
		{
			sorted_array_1 = InternalMergeSort(it_begin, it_middle, comparator, threads);
		}
#pragma omp task firstprivate(it_middle, it_end, comparator)
		{
			sorted_array_2 = InternalMergeSort(it_middle + 1, it_end, comparator, threads);
		}
#pragma omp taskwait
#endif
		// Merge the sorted halves
		if (threads > 1)
			PMergeInto(sorted_array_1, it_middle - it_begin + 1, sorted_array_2, it_end - it_middle, comparator, output, threads);
		else
			MergeInto(sorted_array_1, it_middle - it_begin + 1, sorted_array_2, it_end - it_middle, comparator, output);

		return it_begin;
	}

	// Sort the elements between the iterators it_begin and it_end-1 inclusive
	template<typename T, typename Comparator>
	void MergeSort(T it_begin, T it_end, Comparator comparator, unsigned int threads)
	{
		if (it_begin < it_end)
#ifndef __MERGE_SORT_OMP_SECTIONS
#pragma omp parallel num_threads(threads) 
#pragma omp single
#endif
		{
			// If the number of elements is lower than the granularity then fall back to quicksort
			size_t num_elements = it_end - it_begin;
			if (num_elements > __MERGE_SORT_GRANULARITY)
			{	
				T::value_type *output = new T::value_type[num_elements];
				InternalMergeSort(it_begin, it_end - 1, comparator, output, threads);
				for (size_t i = 0; i < num_elements; ++i)
					*(it_begin + i) = output[i];
				delete output;
			}
			else
				InternalMergeSort(it_begin, it_end - 1, comparator, threads);
		}
	}

	// Sorts the elements between it_begin and it_end-1 inclusive. Falls back to std::sort when there are less than __MERGE_SORT_GRANULARITY
	// elements.
	template<typename T>
	void MergeSort(T it_begin, T it_end, unsigned int threads = 1)
	{
		MergeSort(it_begin, it_end, IsLess<T::value_type>, threads);
	}
};

#endif