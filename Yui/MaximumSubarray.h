#ifndef __MAXIMUM_SUBARRAY_H__
#define __MAXIMUM_SUBARRAY_H__

namespace Yui
{
	// Note: this will return 0 if all the elements are negative
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
}

#endif

/*
#include <iostream>
using namespace std;

int main() {
	int a[] = {-5, 2, -3, 4, 11, 100, -500};
	cout << Yui::LargestContiguousSum(a, sizeof(a)/sizeof(int)) << endl;
	cout << Yui::LargestContiguousProduct(a, sizeof(a)/sizeof(int)) << endl;
	return 0;
}
*/
