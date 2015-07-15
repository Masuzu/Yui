#ifndef __RANGE_MINIMUM_QUERY_H__
#define __RANGE_MINIMUM_QUERY_H__

// Implementation of range minimum query : generalized case with precalculation in O(n*log(n)) time and O(1) query time
// Special case where the input array is such that the difference between 2 consecutive elements is +1 or -1. In this case,
// precalculation in O(n) time and O(1) query time. This can be used to solve the lowest common ancestor problem efficiently.
#include <vector>
#include <math.h>
#include <limits.h>

using namespace std;

#define _MIN(x,y) ((x) < (y) ? (x) : (y))
#define _MAX(x,y) ((x) > (y) ? (x) : (y))

namespace Yui
{
	// Range Minimum Query algorithm
	// O(nlog(n)) preprocessing and O(1) queries
	template<class T>
	class SparseTableRMQ
	{
	private:
		// RMQ[i][j]: index of the minimum among A[i], A[i+1], ..., A[i+2^j-1]
		int **RMQ_;
		int size_;
		// Used to access the values of the input array since RMQ tables only store indices
		T *A_;

	public:
		SparseTableRMQ(const T *A, int size) : size_(size)
		{
			A_ = new T[size];
			memcpy(A_, A, sizeof(T)*size);
			int width = log2(size) + 1;
			RMQ_ = new int*[size];
			for (int i = 0; i < size; ++i) {
				RMQ_[i] = new int[width];
				RMQ_[i][0] = i;
			}

			for (int j = 1; j < width; ++j) {
				for (int i = 0; i < size; ++i) {
					int b = _MIN(i + (1 << (j - 1)), size - (1 << (j - 1)));
					if (A[RMQ_[i][j - 1]] < A[RMQ_[b][j - 1]])
						RMQ_[i][j] = RMQ_[i][j - 1];
					else
						RMQ_[i][j] = RMQ_[b][j - 1];
				}
			}
		}

		~SparseTableRMQ() {
			for (int i = 0; i < size_; ++i)
				delete[] RMQ_[i];
			delete[] RMQ_;
			delete[] A_;
		}
		int MinInRange(int i, int j)
		{
			if (i > j)
				return MinInRange(j, i);
			else
			{
				if (i == j)
					return i;
				int l = log2(j - i + 1);
				int idx1 = RMQ_[i][l];
				int idx2 = RMQ_[j - (1 << l) + 1][l];
				return (A_[idx1] < A_[idx2] ? idx1 : idx2);
			}
		}

		inline const int **RMQ()	{ return RMQ_; }
	};

	// Range Minimum Query for the special case of integer arrays in which each pair of consecutive elements
	// is separated by distance 1.
	// O(n) preprocessing and O(1) queries
	class RestrictedRMQ
	{
	private:
		// Used to access the values of the input array since RMQ tables only store indices
		int *A_;

		// RMQ[i][j]: index of the minimum among A[i], A[i+1], ..., A[i+2^j-1]
		int **RMQ_;
		// Stores the minimum in each block of size 'block_size_' of the input array A
		int *A2_;
		// Stores the index of the minimum in each block of size 'block_size_' of the input array A
		int *A2_idx_;
		// See 'GetIdentifierOf'. Labels each block so that the appropriate RMQ table
		// is used to get the minimum in range
		int *block_ids_;
		int num_blocks_;
		int block_size_;
		// RMQ table for all the combinations of blocks of size 'block_size_ and where
		// consecutive elements differ by 1
		SparseTableRMQ<int> **RMQ_tables_;
		SparseTableRMQ<int> *A2_RMQ_table_;

		// Calculate the minimum in each block of A and store the result in 'A2_'
		void CalculateBlockMinimum(const int *A, int size)
		{
			A2_ = new int[num_blocks_];
			A2_idx_ = new int[num_blocks_];
			for (int block_idx = 0; block_idx < num_blocks_; ++block_idx)
			{
				int block_end = _MIN((block_idx + 1)*block_size_, size);
				A2_[block_idx] = INT_MAX;
				A2_idx_[block_idx] = 0;
				for (int j = block_idx*block_size_; j<block_end; ++j)
				{
					if (A2_[block_idx] > A[j]) {
						A2_[block_idx] = A[j];
						A2_idx_[block_idx] = j;
					}
				}
			}
		}

		void BuildRestrictedRMQTables()
		{
			int num_RMQ_tables = (1 << (block_size_ - 1));
			RMQ_tables_ = new SparseTableRMQ<int>*[num_RMQ_tables];
			std::vector<int> A;
			for (int j = 0; j < block_size_; ++j)
				A.push_back(0);
			int num_bits = ceil(log2(num_RMQ_tables));
			for (int i = 0; i < num_RMQ_tables; ++i) {
				// When writing i in binary representation, i = 0011100...
				// A[0] = 0 and the following elements are computed using the following rule:
				// if the k-th bit of i is 0, A[k+1] = A[k]+1
				// otherwise, A[k+1] = A[k]-1

				for (int j = 1; j <= num_bits; ++j) {
					if ((1 << (j - 1)) & i)
						A[j] = A[j - 1] - 1;
					else
						A[j] = A[j - 1] + 1;
				}

				RMQ_tables_[i] = new SparseTableRMQ<int>(&A[0], block_size_);
			}
		}

		void CalculateBlockIds(const int *A, int size)
		{
			block_ids_ = new int[num_blocks_];
			for (int block_idx = 0; block_idx < num_blocks_; ++block_idx)
			{
				int block_start = block_idx*block_size_;
				int block_end = _MIN((block_idx + 1)*block_size_, size);
				block_ids_[block_idx] = GetIdentifierOf(A + block_start, block_end - block_start);
			}
		}

	public:
		RestrictedRMQ(const int *A, int size)
		{
			A_ = new int[size];
			memcpy(A_, A, sizeof(int)*size);
			block_size_ = _MAX(1, log2(size) / 2);
			// Split A into blocks of size log2(size)/2
			num_blocks_ = ceil(double(size) / block_size_);
			CalculateBlockMinimum(A, size);
			BuildRestrictedRMQTables();
			CalculateBlockIds(A, size);
			A2_RMQ_table_ = new SparseTableRMQ<int>(A2_, num_blocks_);
		}

		~RestrictedRMQ()
		{
			int num_RMQ_tables = (1 << (block_size_ - 1));
			for (int i = 0; i < num_RMQ_tables; ++i)
				delete RMQ_tables_[i];
			delete[] RMQ_tables_;
			delete[] A2_;
			delete[] A2_idx_;
			delete A2_RMQ_table_;
			delete[] A_;
		}

		// 'A' must be an array such that |A[k+1]-A[k]| = 1
		// Returns an integer defined as sum over i from 0 to size-2 included
		// of (2^i if A[i+1]-A[i] == -1 and 0 otherwise)
		int GetIdentifierOf(const int *A, int size)
		{
			int res = 0;
			for (int i = 0; i <= size - 2; ++i) {
				if (A[i + 1] - A[i] == -1)
					res += (1 << i);
			}
			return res;
		}

		int MinInRange(int beg, int end)
		{
			if (beg > end)
				return MinInRange(end, beg);
			else
			{
				// Find the blocks in which beg and end indices are included
				int block_first = beg / block_size_;
				int block_last = end / block_size_;

				// Index of 'beg' with regards to a block
				int block_first_beg_idx = beg - block_first*block_size_;
				// Index of 'end' with regards to a block
				int block_last_end_idx = end - block_last*block_size_;

				int idx1 = RMQ_tables_[block_ids_[block_first]]->MinInRange(block_first_beg_idx, (block_first == block_last) ? block_last_end_idx : block_size_ - 1);
				//  Retrieve the index in A of the minimum element in the block 'block_first' 
				idx1 = idx1 + block_first*block_size_;

				int idx2 = idx1;
				if (block_first + 1 <= block_last - 1) {
					idx2 = A2_RMQ_table_->MinInRange(block_first + 1, block_last - 1);
					//  Retrieve the index in A of the minimum element in the blocks A2 
					idx2 = A2_idx_[idx2];
				}

				int idx3 = RMQ_tables_[block_ids_[block_last]]->MinInRange((block_first == block_last) ? block_first_beg_idx : 0, block_last_end_idx);
				//  Retrieve the index in A of the minimum element in the block 'block_last' 
				idx3 = idx3 + block_last*block_size_;

				if (A_[idx1] < A_[idx2])
				{
					if (A_[idx1] < A_[idx3])
						return idx1;
					else
						return idx3;
				}
				else
				{
					if (A_[idx2] > A_[idx3])
						return idx3;
					else
						return idx2;
				}
			}
		}
	};
}

#if 0
int main() {
	// Size: 10
	//int A[] = {1, 5, 3, 2, 47, 2, 46, 23, 45, 68};
	// Size: 262
	int A[] = { 1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4, 3, 2,
		1, 2, 1, 0, -1, -2, -3, -2, -1, -2, -1, 0, 1, 2, 3, 2, 3, 4, 5, 4 };

	RestrictedRMQ RMQ(A, sizeof(A) / sizeof(int));
	cout << endl << RMQ.MinInRange(0, 0) << endl;
	cout << endl << RMQ.MinInRange(0, 1) << endl;
	cout << endl << RMQ.MinInRange(0, 5) << endl;
	cout << endl << RMQ.MinInRange(0, 8) << endl;
	cout << endl << RMQ.MinInRange(0, 10) << endl;
	cout << endl << RMQ.MinInRange(63, 56) << endl;
	cout << endl << RMQ.MinInRange(60, 63) << endl;
	cout << endl << RMQ.MinInRange(0, 261) << endl;

	return 0;
}
#endif

#endif