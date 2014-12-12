#ifndef __HEAP_H__
#define __HEAP_H__

#include <functional>
#include <vector>

// Find the minimum among the elements at *it, its left and right children in the binary heap structure; then
// swap the latter with the element at *it. Recursively swap the elements downstream in the binary heap if a swap occured.
// Recursive function.
template<typename T, typename Comparator>
void BubbleDown(T it, T it_begin, T it_end, Comparator &comparator)
{
	int diff = it - it_begin;
	int range = it_end - it_begin;	// (it_begin + range) is out of range, that is to say past of it_end
	int right = ((diff + 1) << 1);	// Increment to get the right child of it
	T it_right_child, it_left_child;
	// Find the minimum
	T it_min = it;
	if (right < range)
	{
		it_right_child = it_begin + right;
		if (comparator(*it_right_child, *it_min))
			it_min = it_right_child;
	}
	if (right - 1 < range)
	{
		it_left_child = it_begin + right - 1;
		if (comparator(*it_left_child, *it_min))
			it_min = it_left_child;
	}
	if (it_min != it)
	{
		// Swap the values of *it and *it_min
		auto min_value = *it_min;
		*it_min = *it;
		*it = min_value;
		BubbleDown(it_min, it_begin, it_end, comparator);
	}
}

// Find the minimum among the elements at *it, its left and right children in the binary heap structure; then
// swap the latter with the element at *it. Recursively swap the elements downstream in the binary heap if a swap occured.
// Non recursive function.
template<typename T, typename Comparator>
void BubbleDownNonRecursive(T it, T it_begin, T it_end, Comparator &comparator)
{
	int diff = it - it_begin;
	int range = it_end - it_begin;	// (it_begin + range) is out of range, that is to say past of it_end
	int right = ((diff + 1) << 1);	// Increment to get the right child of it
	T it_right_child, it_left_child;
	// Find the minimum
	T it_min = it;
	T it_current = it;
	while (true)
	{
		if (right < range)
		{
			it_right_child = it_begin + right;
			if (comparator(*it_right_child, *it_min))
				it_min = it_right_child;
		}
		if (right - 1 < range)
		{
			it_left_child = it_begin + right - 1;
			if (comparator(*it_left_child, *it_min))
				it_min = it_left_child;
		}
		if (it_min != it_current)
		{
			// Swap the values of *it_current and *it_min
			auto min_value = *it_min;
			*it_min = *it_current;
			*it_current = min_value;
			it_current = it_min;
			diff = it_current - it_begin;
			right = ((diff + 1) << 1);
		}
		else
			break;
	}
}

// Transforms the range of elements between it_root and it_end-1 included into a binary heap.
// it_begin points on the root of the heap (it_begin<=it_root<=it_end)
template<typename T, typename Comparator>
void Heapify(T it_root, T it_begin, T it_end, Comparator &comparator)
{
	BubbleDown(it_root, it_begin, it_end, comparator);
}

template<typename T, typename Comparator = std::less<T>>
class Heap
{
private:
	std::vector<T> data_;
	Comparator comparator_;

	// Find the minimum among the elements at i, its left and right children in the binary heap structure; then
	// swap the latter with the element at i. Recursively swap the elements downstream in the binary heap if a swap occured.
	// Non recursive function.
	void BubbleDown(unsigned int i)
	{
		unsigned int min = i;
		unsigned int current_index = i;
		unsigned int right = ((current_index + 1) << 1);
		unsigned int left = right - 1;
		// Recursively bubble down the minimum value
		while (true)
		{
			// Find the minimum
			if (right < data_.size())
			{
				if (comparator_(data_[right], data_[min]))
					min = right;
			}
			if (left < data_.size())
			{
				if (comparator_(data_[left], data_[min]))
					min = left;
			}
			if (min != current_index)
			{
				T min_value = data_[min];
				data_[min] = data_[current_index];
				data_[current_index] = min_value;
				current_index = min;
				right = ((current_index + 1) << 1);
				left = right - 1;
			}
			else
				break;
		}
	}

	// Find the minimum among it_child and its parent in the binary heap structure; then
	// swap the latter with the parent. Recursively swap the elements upstream in the binary heap if a swap occured.
	// Non recursive function.
	void BubbleUp(int i)
	{			
		if (i == 0)
			return;
		T value_to_bubble_up = data_[i];
		int child = i;
		int parent = ((child-1) >> 2);
		
		while (child != parent)
		{
			if (comparator_(value_to_bubble_up, data_[parent]))
			{
				data_[child] = data_[parent];
				child = parent;
				if (child > 0)
					parent = ((child - 1) >> 2);
			}
			else
			{
				// We found the final position of value_to_bubble_up in the heap	
				break;
			}
		}
		data_[child] = value_to_bubble_up;
	}

public:
	Heap()	{}
	void Push(const T &e)
	{
		data_.push_back(e);
		BubbleUp(data_.size() - 1);
	}

	void Push(const T &&e)
	{
		data_.push_back(std::move(e));
		BubbleUp(data_.size() - 1);
	}

	void Pop(T &e)
	{
		e = data_[0];
		data_[0] = data_[data_.size() - 1];
		data_.pop_back();
		BubbleDown(0);
	}

	inline bool IsEmpty()
	{
		return data_.empty();
	}
};

#endif