#ifndef __MAXIMUM_SUBARRAY_H__
#define __MAXIMUM_SUBARRAY_H__

namespace Yui
{
    template<class T>
    T LargestContiguousSum(T *a, int length)
    {
    	if(length == 0)
    		return 0;
    	T max_so_far = a[0];
    	T max_ending_here = a[0];
    	for(int i=1; i<length; ++i)
    	{
    		max_ending_here += a[i];
    		if(max_ending_here > max_so_far)
    			max_so_far = max_ending_here;
    		if(max_ending_here < 0)
    			max_ending_here = 0;
    	}
    	return max_so_far;
    }

    template<class T>
    T LargestContiguousProduct(T *a, int length)
    {
    	static auto max = [](const T& a, const T& b) -> const T&
	    {
	        return (a < b) ? b : a;
	    };
	    
    	if(length == 0)
    		return 0;
    	T min_so_far = a[0];
    	T max_so_far = a[0];
    	T global_max = a[0];
    	for(int i=1; i<length; ++i)
    	{
    		T buffer = max_so_far;
    		max_so_far = Yui::max(max(a[i], max_so_far*a[i]), min_so_far*a[i]);
    		min_so_far = max(max(a[i], buffer*a[i]), min_so_far*a[i]);
    		global_max = max(global_max, max_so_far);
    	}
    	return global_max;
    }
    
    // Return a pair (subarray.start_index, subarray.end_index) for the subarray of A whose sum of elements is the closest to 'target' in O(nlog(n)) time
	template<class T, class U>
	void ClosestSubsetSum(std::vector<T> &A, const U &target, std::pair<size_t, size_t> &indices)
	{
		// Returns an iterator on the first element in map <= value 
		static auto ClosestSubsetSum_LowerBound = [](const std::map<U, size_t> &map, const U &value) -> std::map<U, size_t>::const_iterator
		{
			auto lower_bound = map.lower_bound(value);	// First element in map >= extra 
			if (lower_bound == map.end())
				--lower_bound;	// map has at least 1 entry {0, -1}
			if (lower_bound->first > value)	// Case when the returned iterator is > extra
			{
				if (lower_bound != map.begin())
					--lower_bound;
				else
					lower_bound = map.end();
			}
			return lower_bound;
		};
	
		U sum = 0;
		// Map of (subarray sum from index 0 to i included, i)
		std::map<U, size_t> map;
		U min_diff = INT_MAX;
		map.insert(std::make_pair(0, -1));
	
		for (size_t i = 0; i < A.size(); ++i)
		{
			sum += A[i];
			U extra = sum - target;
	
			auto lower_bound = ClosestSubsetSum_LowerBound(map, extra);	// first element in map <= extra
			if (lower_bound != map.end())
			{
				U subarray_sum = sum - lower_bound->first;
				U dist_to_target = std::abs(subarray_sum - target);
				if (dist_to_target < min_diff)
				{
					min_diff = dist_to_target;
					indices = { lower_bound->second + 1, i};
				}
			}
	
			auto upper_bound = map.lower_bound(extra);	// first element in map >= extra
			if (upper_bound != map.end())
			{
				U subarray_sum = sum - upper_bound->first;
				U dist_to_target = std::abs(subarray_sum - target);
				if (dist_to_target < min_diff)
				{
					min_diff = dist_to_target;
					indices = { upper_bound->second + 1, i };
				}
			}
			map.insert(std::make_pair(sum, i));
		}
	}
}

#endif

/*
#include <iostream>
#include <assert.h>
using namespace std;

int main() {
	int a[] = {-5, 2, -3, 4, 11, 100, -500};
	cout << Yui::LargestContiguousSum(a, sizeof(a)/sizeof(int)) << endl;
	cout << Yui::LargestContiguousProduct(a, sizeof(a)/sizeof(int)) << endl;
	
	std::vector<double> b = { 10, 1, 2.6, 6.9, 6, 1, 5 };
	std::pair <size_t, size_t> indices;
	ClosestSubsetSum(b, 8.0, indices);
	std::pair<size_t, size_t> expected_indices = { 4, 5 };
	assert(indices == expected_indices);
	
	return 0;
}
*/
