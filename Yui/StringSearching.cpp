#include "StringSearching.h"

int KMPSearch(const std::string &text, const std::string &pattern)
{
	if (text.empty() || pattern.empty() || pattern.size() > text.size())
		return -1;
	// Build the failure function
	// lps[i] is the longest proper suffix of pattern.substr(i+1) which is also a prefix of pattern.substr(i+1)
	int *lps = new int[pattern.size()];
	lps[0] = 0;
	for (int i = 1; i < pattern.size(); ++i)
	{
		int index_of_next_largest_partial_match = lps[i - 1];
		// Try to expand the prefix of length 'index_of_next_largest_partial_match' with the pattern[i-1]
		while (true)
		{
			if (pattern[i - 1] == pattern[index_of_next_largest_partial_match])
			{
				lps[i] = index_of_next_largest_partial_match + 1;
				break;
			}
			if (index_of_next_largest_partial_match == 0)
			{
				lps[i] = 0;
				break;
			}		
			index_of_next_largest_partial_match = lps[index_of_next_largest_partial_match-1];
		}
	}
	int index_pattern = 0;
	int index_text = 0;
	while (index_text < text.size())
	{
		if (pattern[index_pattern] == text[index_text])
		{
			++index_text;
			++index_pattern;
			if (index_pattern == pattern.size())
				return index_text - index_pattern;
		}
		else if (index_pattern > 0)
			// The first index_pattern characters of pattern match text[index_text-index_pattern-1 ... index_text-1]
			// and lps[index_pattern-1] characters of pattern are both prefix and suffix.
			// Resume the search from index lps[index_pattern-1].
			index_pattern = lps[index_pattern-1];
		else
			++index_text;
	}
	delete lps;
}
