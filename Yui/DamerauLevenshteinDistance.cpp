#include "DamerauLevenshteinDistance.h"

#include <algorithm>
#ifdef _DEBUG
#include <iostream>
#endif

// Compute the index of x in the circular array distance_matrix_
#define __DISTANCE_INDEX(x)	(((x) + 3) % 3)

namespace Yui
{
	DamerauLevenshteinDistance::DamerauLevenshteinDistance(const String &reference, const String &target)
		: reference_(reference), current_index_(0), num_rows_(1), min_distance_(0)
	{
		distance_matrix_ = new int*[3];
		for (int i = 0; i < 3; ++i)
			distance_matrix_[i] = new int[reference.length() + 1];
		// Computes the distance between the null string and reference
		distance_matrix_[0][0] = 0;
		for (int i = 1; i < reference.length() + 1; ++i)
			distance_matrix_[0][i] = i;
		UpdateDistance(target);
	}

	DamerauLevenshteinDistance::DamerauLevenshteinDistance(const DamerauLevenshteinDistance &distance)
		: reference_(distance.reference_), target_(distance.target_),
		current_index_(distance.current_index_), num_rows_(distance.num_rows_),
		min_distance_(distance.min_distance_), distance_(distance.distance_)
	{
		distance_matrix_ = new int*[3];
		for (int i = 0; i < 3; ++i)
			distance_matrix_[i] = new int[reference_.length() + 1];
		for (int i = 0; i < num_rows_; ++i)
		{
			for (int j = 0; j < reference_.length() + 1; ++j)
				distance_matrix_[i][j] = distance.distance_matrix_[i][j];
		}
	}

	DamerauLevenshteinDistance::~DamerauLevenshteinDistance()
	{
		for (int i = 0; i < 3; ++i)
			delete[] distance_matrix_[i];
		delete[] distance_matrix_;
	}

	void DamerauLevenshteinDistance::UpdateDistance(const String &s)
	{
		if (!s.empty())
		{
			target_ += s;
			int insertion_cost, deletion_cost, matching_cost, transposition_cost;
			for (int i = 1; i < s.length() + 1; ++i)
			{
				current_index_ = ((current_index_ + 1) % 3);
				num_rows_ = std::min(num_rows_ + 1, 3);

				// The distance between s and the null string is the length of s
				distance_matrix_[current_index_][0] = distance_matrix_[__DISTANCE_INDEX(current_index_ - 1)][0] + 1;
				for (int j = 1; j < reference_.length() + 1; ++j)
				{
					insertion_cost = distance_matrix_[current_index_][j - 1] + kInsertionCost;
					deletion_cost = distance_matrix_[__DISTANCE_INDEX(current_index_ - 1)][j] + kDeletionCost;
					matching_cost = distance_matrix_[__DISTANCE_INDEX(current_index_ - 1)][j - 1] + ((reference_[j - 1] == s[i - 1]) ? 0 : kSubstitutionCost);
					if (num_rows_ >= 2 && i > 1 && j > 1)
					{
						if (reference_[j - 1] == s[i - 2] && reference_[j - 2] == s[i - 1])
							transposition_cost = distance_matrix_[__DISTANCE_INDEX(current_index_ - 2)][j - 2] + kTranspositionCost;
						else
							transposition_cost = INT_MAX;
					}
					else
						transposition_cost = INT_MAX;
					distance_matrix_[current_index_][j] = std::min({ insertion_cost, deletion_cost, matching_cost, transposition_cost });
				}
			}
			// Update min_distance_
			min_distance_ = INT_MAX;
			for (int i = 0; i < num_rows_; ++i)
			{
				for (int j = 0; j < reference_.length() + 1; ++j)
					min_distance_ = std::min(min_distance_, distance_matrix_[i][j]);
			}
		}
		distance_ = distance_matrix_[current_index_][reference_.length()];
	}

#ifdef _DEBUG
	void DamerauLevenshteinDistance::PrintDistance()
	{
		for (int i = 0; i < num_rows_; ++i)
		{
			for (int j = 0; j < reference_.length() + 1; ++j)
				std::cout << distance_matrix_[i][j] << " ";
			std::cout << std::endl;
		}
	}
#endif
};
