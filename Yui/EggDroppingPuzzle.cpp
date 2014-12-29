#include "EggDroppingPuzzle.h"

#include <limits>

#define __MIN(x, y)	(((x) < (y)) ? (x) : (y))
#define __MAX(x, y)	(((x) > (y)) ? (x) : (y))

namespace EggDroppingPuzzle
{
	unsigned int SolveRecursive(unsigned int n, unsigned int k)
	{
		if (n == 0)	// There must be at least one egg
			return 0;
		// With only one egg, each of the k floors must be tested in ascending order
		if (n == 1)
			return k;
		// If there are no floors or only one floor
		if (k == 0 || k == 1)
			return k;
		unsigned min = UINT_MAX;
		for (unsigned int floor = 1; floor <= k; ++floor)
			min = __MIN(min, __MAX(SolveRecursive(n - 1, floor - 1), SolveRecursive(n, k - floor)));

		return min + 1;
	}

	unsigned int SolveDP(unsigned int n, unsigned int k)
	{
		unsigned int **T = new unsigned int*[n + 1];
		for (unsigned int egg = 1; egg < n + 1; ++egg)
			T[egg] = new unsigned int[k + 1];
		// With only one egg, each of the k floors must be tested in ascending order
		for (unsigned int floor = 0; floor < k + 1; ++floor)
			T[1][floor] = floor;
		for (unsigned int egg = 1; egg < n + 1; ++egg)
		{
			T[egg][0] = 0;
			T[egg][1] = 1;
		}
		// T(n,k) = 1 + min{max(T(n − 1, x − 1), T(n,k − x)): x = 1, 2, ..., k}
		for (unsigned int egg = 2; egg < n + 1; ++egg)
		{
			for (unsigned int floor = 1; floor < k + 1; ++floor)
			{
				unsigned int min = UINT_MAX;
				for (unsigned int x = 1; x <= floor; ++x)
					min = __MIN(min, __MAX(T[egg - 1][x - 1], T[egg][floor - x]));
				T[egg][floor] = min + 1;
			}
		}

		unsigned int result = T[n][k];

		for (unsigned int egg = 1; egg < n + 1; ++egg)
			delete[] T[egg];
		delete[] T;

		return result;
	}
};