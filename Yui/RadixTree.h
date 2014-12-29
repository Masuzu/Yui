#ifndef __RADIX_TREE_H__
#define __RADIX_TREE_H__

// Define __USE_CHAR or __USE_WCHAR_T depending on whether you work with char or wchar_t
#ifdef __USE_CHAR
#undef __USE_WCHAR_T
#else
#define __USE_CHAR
#undef __USE_WCHAR_T
#endif

#ifdef __USE_WCHAR_T
#undef __USE_CHAR
#endif

#include <string>
#include <vector>

#include "DamerauLevenshteinDistance.h"

namespace Yui
{
	class RadixTree
	{
	public:
#ifdef __USE_CHAR
		typedef std::string String;
#else
		typedef std::wstring String;
#endif
		typedef String::value_type Character;

	private:
		// Stores the common strings between the words in the radix tree
		class Node
		{
		public:
			// All the nodes following link_ begin with prefix_
			String prefix_;
			// Pointer to the next prefix
			Node *next_ = nullptr;
			// Pointer to the next substring starting with prefix_
			Node *link_ = nullptr;
			bool leaf_node_ = true;

			// Returns the length of the longest common prefix between prefix_ and s
			unsigned int LongestCommonPrefix(const Character *s, unsigned int length_of_s);

			// Returns true if s did not exist already in the radix tree
			bool InternalInsert(const Character *s, unsigned int length_of_s);
			Node *InternalFind(const Character *s, unsigned int length_of_s);

			// Returns a pointer to the node to be deleted
			void InternalDelete(const Character *s, unsigned int length_of_s);

			// Split this node into 2 nodes: n1=Node(prefix_.substr(0, k)) and n2= Node(prefix_.substr(k)).
			// Returns the new node n1.
			Node *Split(unsigned int k);

			// Matches s with prefix_. prefix is used to concatenate the prefixes of the traversed nodes to rebuild the words.
			void ExactMatching(const Character *s, unsigned int length_of_s, std::vector<String> &v, const String &prefix);
			// DFS from this node while adding the words found to v
			void DFS(std::vector<String> &v, const String &prefix);

			// Updates the distance between distance.reference_ and distance.target_ by appending prefix_ to distance.target_.
			// The distance can't go lower than distance.min_distance_ when the next nodes are explored, by definition of the
			// Damerau-Levenshtein distance. If distance.min_distance <= max_distance, the next nodes are explored recursively.
			// Worst case running time: O(|A|^(k+max_distance)) where A is the alphabet and k the length of distance.reference_.
			// However, thanks to search space pruning, it runs fast enough for spell-checker applications.
			void ApproximateMatching(std::vector<String> &v, DamerauLevenshteinDistance distance, int max_distance);

			void MergeWithLink();
			void MergeWithNext();
			// Returns true if the node has no children and is not the end of a word
			bool IsOrphan();

			// Creates a new node with prefix_ = s[0] s[1] ... s[string_length-1]
			inline Node(const Character *s, unsigned int string_length) : prefix_(s, s + string_length)	{}
			inline explicit Node(const Character *s) : prefix_(s)	{}
			inline explicit Node(const String &s) : prefix_(s)	{}
			inline ~Node()	{}
		};

		Node *root_node_ = nullptr;
		int num_words_ = 0;

	public:
		inline RadixTree()	{}
		~RadixTree();
		// Inserts s in the radix tree in O(m) time where m = strlen(s) and splits the existing nodes if they share a common substring with s
		void Insert(const Character *s);
		// Inserts s in the radix tree in O(m) time where m = s.length() and splits the existing nodes if they share a common substring with s
		void Insert(const String &s);
		// Searches for s in the radix tree in O(m) time where m = strlen(s)
		bool Find(const Character *s);
		// Searches for s in the radix tree in O(m) time where m = s.length()
		bool Find(const String &s);
		// Deletes s and recursively merges the nodes found on the way
		void Delete(const Character *s);
		// Deletes s and recursively merges the nodes found on the way
		void Delete(const String &s);
		// Returns in v the strings beginning with s
		void ExactMatching(const String &s, std::vector<String> &v);
		// Returns in v the strings whose Damerau-Levenshtein distance from s is lower than 3
		void ApproximateMatching(const String &s, std::vector<String> &v);
	};
};

#undef __USE_CHAR_T
#undef __USE_WCHAR_T

#endif