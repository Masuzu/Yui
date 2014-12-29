#ifndef __DAMERAU_LEVENSHTEIN_DISTANCE_H__
#define __DAMERAU_LEVENSHTEIN_DISTANCE_H__

#include <string>

namespace Yui
{
	// The Damerau�Levenshtein distance is a distance between two strings given by counting the minimum number of operations needed to 
	// transform one string into the other, where an operation is defined as an insertion, deletion, or substitution of a single character,
	// or a transposition of two adjacent characters.
	// http://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
	// To save up space, since only the distance between the strings is needed, only the last 3 rows of the distance
	// matrix are saved in a circular 2D array.
	class DamerauLevenshteinDistance
	{
	private:
		// A circular 2D array of distances with 3 rows
		int **distance_matrix_;
		std::string reference_;
		std::string target_;
		// Index of the current row of distance_matrix_
		int current_index_;
		// Number of rows used in distance_matrix_, integer in {1, 2, 3}
		int num_rows_;
		// Minimum value of distance_matrix_
		int min_distance_;
		// Distance between target_ and reference_
		int distance_;

		const int kTranspositionCost = 1;
		const int kSubstitutionCost = 1;
		const int kInsertionCost = 1;
		const int kDeletionCost = 1;

	public:
		// Computes the distance from target to reference
		DamerauLevenshteinDistance(const std::string &reference, const std::string &target = "");
		DamerauLevenshteinDistance(const DamerauLevenshteinDistance &distance);
		~DamerauLevenshteinDistance();

		// Computes the distance between target_+s and reference_. The distance matrix is updated accordingly.
		void UpdateDistance(const std::string &s);

		inline int min_distance()	{ return min_distance_; }
		inline int distance()	{ return distance_; }
		inline const std::string &reference()	{ return reference_; }
		inline const std::string &target()	{ return target_; }
#ifdef _DEBUG
		void PrintDistance();
#endif
	};
};


#endif