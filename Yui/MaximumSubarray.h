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
}

#endif

/*
#include <iostream>
using namespace std;

int main() {
	int a[] = {-5, 2, -3, 4, 11, 100, -500};
	cout << Yui::LargestContiguousSum(a, sizeof(a)/sizeof(int)) << endl;
	return 0;
}
*/
