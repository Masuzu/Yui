#include "StringSearching.h"

namespace Yui
{
	int KMPSearch(const std::string &text, const std::string &pattern)
	{
		if (text.empty() || pattern.empty() || pattern.size() > text.size())
			return -1;
		// partial_match_table[i] is the longest proper prefix of pattern.substr(i) which is also a suffix of pattern.substr(i)
		// Index 0 is the empty character
		int *partial_match_table = new int[pattern.size() + 1];
		partial_match_table[0] = 0;
		partial_match_table[1] = 0;
		// Lenth of the longest proper prefix at the current position in pattern
		int longest_proper_prefix_length = 0;
		for (int i = 2; i <= pattern.size(); ++i)
		{
			int index_of_next_largest_partial_match = partial_match_table[i - 1];
			// Try to expand the prefix of length 'index_of_next_largest_partial_match' with the pattern[i-1]
			while (true)
			{
				if (pattern[i - 1] == pattern[index_of_next_largest_partial_match])
				{
					partial_match_table[i] = index_of_next_largest_partial_match + 1;
					break;
				}
				if (index_of_next_largest_partial_match == 0)
				{
					partial_match_table[i] = 0;
					break;
				}
				index_of_next_largest_partial_match = partial_match_table[index_of_next_largest_partial_match];
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
				index_pattern = partial_match_table[index_pattern];
			else
				++index_text;
		}

		delete partial_match_table;
	}
}