#ifndef __SEGMENT_TREE_H__
#define __SEGMENT_TREE_H__

#include <vector>
#include <iostream>

#define __MIN(a, b)	(((a) < (b)) ? (a) : (b))

namespace Yui
{
	template<typename T>
	class SegmentTree
	{
	public:
		template<typename T>
		class Node
		{
		public:
			bool defined_ = false;
			T min_;
			size_t start_;
			size_t end_;

			Node()	{}
			Node(T min, size_t start, size_t end) : min_(min), start_(start), end_(end)
			{}
		};

	private:
		Node<T> *nodes_;
		size_t num_nodes_;

		T BuildSegmentTree(const std::vector<T> &input, size_t start, size_t end, size_t index)
		{
			nodes_[index].defined_ = true;
			nodes_[index].start_ = start;
			nodes_[index].end_ = end;
			if (start == end)
				nodes_[index].min_ = input[start];
			else
			{
				size_t mid = start + ((end - start) >> 1);
				size_t left_child_index = (index << 1) + 1;
				nodes_[index].min_ = __MIN(BuildSegmentTree(input, start, mid, left_child_index), BuildSegmentTree(input, mid + 1, end, left_child_index + 1));
			}

			return nodes_[index].min_;
		}

		T GetMinInRange(size_t start, size_t end, size_t index)
		{
			// Node range entirely included in query range
			if (nodes_[index].start_ >= start && nodes_[index].end_ <= end)
				return nodes_[index].min_;
			// Query range outside in the node range
			else if (end < nodes_[index].start_ || start > nodes_[index].end_)
				return std::numeric_limits<T>::max();
			else
			{
				size_t left_child_index = (index << 1) + 1;
				return __MIN(GetMinInRange(start, end, left_child_index), GetMinInRange(start, end, left_child_index + 1));
			}
		}

	public:
		SegmentTree(const std::vector<T> &input)
		{
			int height = (int)(ceil(log2(input.size()))); //Height of segment tree
			num_nodes_ = 2 << (int)pow(2, height); //Maximum size of segment tree
			nodes_ = new Node<T>[num_nodes_];
			BuildSegmentTree(input, 0, input.size() - 1, 0);
		}

		virtual ~SegmentTree()	{ delete[] nodes_; }

		T GetMinInRange(size_t start, size_t end)
		{
			if (start > end)
				return std::numeric_limits<T>::max();
			return	GetMinInRange(start, end, 0);
		}

		Node<T>* nodes()	const	{ return nodes_; }
		inline size_t num_nodes()	const	{ return num_nodes_; }
	};
}

#undef __MIN

#endif