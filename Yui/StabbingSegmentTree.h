#ifndef __YUI_STABBING_SEGMENT_TREE_H__
#define __YUI_STABBING_SEGMENT_TREE_H__

// Stores intervals of type T where each node has data of type U and allows stabbing queries in O(log(n)+k) time
// where n is the number of segments and k the number of segments fulfilling the query
// Warning: inserting intervals whose start and end are not included in the collection 'ends' passed in the constructor will result in invalid results.

template<class T, class U>
class StabbingSegmentTree
{
private:
	class Node
	{
	friend class StabbingSegmentTree<T, U>;
	private:		
		T start_;
		T end_;
		std::vector<U> data_;

	public:
		bool Contains(const T &t1)
		{
			return (start_ <= t1 && t1 <= end_);
		}
		bool IsContainedIn(const T &t1, const T &t2)
		{
			return (t1 <= start_ && t2 >= end_);
		}
		inline bool Intersects(const T &t1, const T &t2)	// Assumption: t1 <= t2
		{
			return (t1 <= end && t2 >= start)
		}
	};

	Node *nodes_;
	unsigned int num_nodes_;

	void BuildTree(const std::vector<T> &ends, int beg, int end, int node_idx)
	{
		Node &current = nodes_[node_idx];
		current.start_ = ends[beg];
		current.end_ = ends[end];
		//std::cout << "Node [" << current.start_ << "," << current.end_ << "]\n";
		if(beg == end)
			return;
	
		int mid = beg+(end-beg)/2;
		BuildTree(ends, beg, mid, 2*node_idx+1);
		BuildTree(ends, mid+1, end, 2*node_idx+2);
	}

	void InternalInsert(const T &t1, const T &t2, const U &data, int node_idx)
	{
		Node &current = nodes_[node_idx];
		if(current.IsContainedIn(t1, t2))
			current.data_.push_back(data);
		else
		{
			int left_idx = 2*node_idx+1;
			if(left_idx < num_nodes_ && nodes_[left_idx].Intersects(t1, t2))
				InternalInsert(t1, t2, data, left_idx);
			int right_idx = 2*node_idx+2;
			if(right_idx < num_nodes_ && nodes_[right_idx].Intersects(t1, t2))
				InternalInsert(t1, t2, data, right_idx);
		}
	}

	void InternalStabbingQuery(const T &t1, std::vector<U> &output, int node_idx)
	{
		Node &current = nodes_[node_idx];
		if(current.Contains(t1))
		{
			for(int i=0, end = current.data_.size(); i<end; ++i)
				output.push_back(current.data_[i]);
		}
		int left_idx = 2*node_idx+1;
		if(left_idx < num_nodes_ && nodes_[left_idx].Contains(t1))
			InternalStabbingQuery(t1, output, left_idx);
		int right_idx = 2*node_idx+2;
		if(right_idx < num_nodes_ && nodes_[right_idx].Contains(t1))
			InternalStabbingQuery(t1, output, right_idx);
	}

public:
	// Builds a segment tree whose elementary intervals are described by 'ends'
	StabbingSegmentTree(std::vector<T> &ends)
	{
		int tree_height = ceil(log2(ends.size()));
		num_nodes_ = 2*(1 << tree_height)-1;
		std::sort(ends.begin(), ends.end());
		//std::cout << "num_nodes=" << num_nodes_ << std::endl;
		nodes_ = new Node[num_nodes_ ];
		BuildTree(ends, 0, ends.size()-1, 0);
	}
	// Insert an interval of length in [t1, t2] containing 'data'
	void Insert(T t1, T t2, const U &data)
	{
		if(t1 > t2)
		{
			T buffer = t1;
			t1 = t2;
			t2 = buffer;
		}
		InternalInsert(t1, t2, data, 0);
	}

	void StabbingQuery(const T &t1, std::vector<U> &output)
	{
		InternalStabbingQuery(t1, output, 0);
	}
};

#if 0
int main(void)
{
	
	static const int arr[] = {1, 0, 2, 3, 4, 6, -1};
	std::vector<int> ends (arr, arr + sizeof(arr) / sizeof(arr[0]) );
	StabbingSegmentTree<int, char> tree(ends);
	tree.Insert(1, 3, 'A');
	tree.Insert(6, 3, 'B');
	tree.Insert(-1, 0, 'C');
	std::vector<char> res_stabbing_query;
	tree.StabbingQuery(3, res_stabbing_query);
	for(int i=0, end = res_stabbing_query.size(); i<end; ++i)
		std::cout << res_stabbing_query[i];
	std::cout << std::endl;
}
#endf

#endif
