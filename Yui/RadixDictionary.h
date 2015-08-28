#ifndef __RADIX_DICTIONARY_H__
#define __RADIX_DICTIONARY_H__

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
#include <map>
#include "DamerauLevenshteinDistance.h"

#define __MIN(x, y) (((x) < (y)) ? (x) : (y))

namespace Yui
{
	template<class T>
	class RadixDictionary
	{
	public:
#ifdef __USE_CHAR
		typedef std::string String;
#else
		typedef std::wstring String;
#endif
		typedef String::value_type Character;

	public:
		// Stores the common strings between the words in the radix tree
		class Node
		{
			friend class RadixDictionary;
		private:
			// All the nodes following link_ begin with prefix_
			String prefix_;
			// Pointer to the next prefix
			Node *next_;
			// Pointer to the next substring starting with prefix_
			Node *link_;
			bool leaf_node_;
			T *data_;

			// Returns true if s did not exist already in the radix tree
			bool InternalInsert(const Character *s, unsigned int length_of_s, T *data)
			{
				// Find the node which has a common prefix with s
				unsigned int l = LongestCommonPrefix(s, length_of_s);
				if (l == 0)
				{
					if (next_)
						return next_->InternalInsert(s, length_of_s, data);
					else
					{
						next_ = new Node(s, length_of_s);
						next_->data_ = data;
						return true;
					}
				}
				else
				{
					Node *n = Split(l);
					if (l < length_of_s)
					{
						if (n->link_)
							return n->link_->InternalInsert(s + l, length_of_s - l, data);
						else
						{
							n->link_ = new Node(s + l, length_of_s - l);
							n->link_->data_ = data;
							return true;
						}
					}
					else
					{
						bool new_node = !n->leaf_node_;
						n->leaf_node_ = true;
						return new_node;
					}
				}
			}

			Node *InternalFind(const Character *s, unsigned int length_of_s)
			{
				// Find the node which has a common prefix with s
				unsigned int l = LongestCommonPrefix(s, length_of_s);
				if (l == 0)
				{
					if (next_)
						return next_->InternalFind(s, length_of_s);
					else
						// String not found
						return nullptr;
				}
				else
				{
					if (l == length_of_s)
					{
						if (l == prefix_.length())
							return this;
						else
							return nullptr;
					}
					else
					{
						if (link_)
							return link_->InternalFind(s + l, length_of_s - l);
						else
							return nullptr;
					}
				}
			}

			// Returns a pointer to the node to be deleted
			void InternalDelete(const Character *s, unsigned int length_of_s)
			{
				unsigned int l = LongestCommonPrefix(s, length_of_s);
				if (l == 0)
				{
					if (next_)
					{
						next_->InternalDelete(s, length_of_s);
						MergeWithNext();
					}
				}
				else
				{
					if (l == length_of_s)
					{
						leaf_node_ = false;
						if (link_)
							MergeWithLink();
						if (next_)
							MergeWithNext();
					}
					else
					{
						if (link_)
						{
							link_->InternalDelete(s + l, length_of_s - l);
							MergeWithLink();
						}
					}
				}
			}

			// Split this node into 2 nodes: n1=Node(prefix_.substr(0, k)) and n2= Node(prefix_.substr(k)).
			// Returns the new node n1.
			Node *Split(unsigned int k)
			{
				if (prefix_.length() == k)
					return this;
				Node *n2 = new Node(prefix_.substr(k));
				prefix_ = prefix_.substr(0, k);
				n2->link_ = link_;
				link_ = n2;
				n2->leaf_node_ = leaf_node_;
				leaf_node_ = false;
				return this;
			}

			// Matches s with prefix_. prefix is used to concatenate the prefixes of the traversed nodes to rebuild the words.
			void ExactMatching(const Character *s, unsigned int length_of_s, std::vector<String> &v, const String &prefix)
			{
				unsigned int l = LongestCommonPrefix(s, length_of_s);
				if (l == 0)
				{
					if (next_)
						next_->ExactMatching(s, length_of_s, v, prefix);
				}
				else
				{
					if (l == length_of_s)
					{
						// Perform depth first seach from this node and push the nodes in v
						if (leaf_node_)
							v.push_back(prefix + prefix_);
						if (link_)
							link_->DFS(v, prefix + prefix_);
					}
					else
					{
						if (link_)
							link_->ExactMatching(s + l, length_of_s - l, v, prefix + prefix_);
					}
				}
			}

			void ExactMatching(const Character *s, unsigned int length_of_s, std::map<String, T*> &v, const String &prefix)
			{
				unsigned int l = LongestCommonPrefix(s, length_of_s);
				if (l == 0)
				{
					if (next_)
						next_->ExactMatching(s, length_of_s, v, prefix);
				}
				else
				{
					if (l == length_of_s)
					{
						// Perform depth first seach from this node and push the nodes in v
						if (leaf_node_)
							v[prefix + prefix_] = data_;
						if (link_)
							link_->DFS(v, prefix + prefix_);
					}
					else
					{
						if (link_)
							link_->ExactMatching(s + l, length_of_s - l, v, prefix + prefix_);
					}
				}
			}

			// DFS from this node while adding the words found to v
			void DFS(std::vector<String> &v, const String &prefix)
			{
				if (leaf_node_)
					v.push_back(prefix + prefix_);
				if (link_)
					link_->DFS(v, prefix + prefix_);
				if (next_)
					next_->DFS(v, prefix);
			}

			// DFS from this node while adding the nodes found to v
			void DFS(std::map<String, T*> &v, const String &prefix)
			{
				if (leaf_node_)
					v[prefix + prefix_] = data_;
				if (link_)
					link_->DFS(v, prefix + prefix_);
				if (next_)
					next_->DFS(v, prefix);
			}

			// Updates the distance between distance.reference_ and distance.target_ by appending prefix_ to distance.target_.
			// The distance can't go lower than distance.min_distance_ when the next nodes are explored, by definition of the
			// Damerau-Levenshtein distance. If distance.min_distance <= max_distance, the next nodes are explored recursively.
			// Worst case running time: O(|A|^(k+max_distance)) where A is the alphabet and k the length of distance.reference_.
			// However, thanks to search space pruning, it runs fast enough for spell-checker applications.
			void ApproximateMatching(std::vector<String> &v, DamerauLevenshteinDistance distance, int max_distance)
			{
				if (distance.min_distance() <= max_distance)
				{
					if (next_)
						next_->ApproximateMatching(v, distance, max_distance);
				}
				distance.UpdateDistance(prefix_);
				if (leaf_node_ && distance.distance() <= max_distance)
					v.push_back(distance.target());
				if (distance.min_distance() <= max_distance)
				{
					if (link_)
						link_->ApproximateMatching(v, distance, max_distance);
				}
			}

			void ApproximateMatching(std::map<String, T*> &v, DamerauLevenshteinDistance distance, int max_distance)
			{
				if (distance.min_distance() <= max_distance)
				{
					if (next_)
						next_->ApproximateMatching(v, distance, max_distance);
				}
				distance.UpdateDistance(prefix_);
				if (leaf_node_ && distance.distance() <= max_distance)
					v[distance.target()] = data_;
				if (distance.min_distance() <= max_distance)
				{
					if (link_)
						link_->ApproximateMatching(v, distance, max_distance);
				}
			}

			void MergeWithLink()
			{
				if (link_)
				{
					if (!leaf_node_)
					{
						if (!link_->next_)
						{
							Node *node_to_be_merged = link_;
							prefix_ += link_->prefix_;
							leaf_node_ = link_->leaf_node_;
							link_ = link_->link_;
							delete node_to_be_merged;
							MergeWithLink();
						}
					}
					else if (link_->IsOrphan())
						link_ = nullptr;
				}
			}

			void MergeWithNext()
			{
				if (next_)
				{
					if (!leaf_node_)
					{
						if (!next_->leaf_node_)
						{
							if (!next_->link_)
							{
								Node *node_to_be_merged = next_;
								next_ = next_->next_;
								delete node_to_be_merged;
								MergeWithNext();
							}
						}
						else
						{
							if (!link_)
							{
								// Replace this node with next_
								Node *node_to_be_merged = next_;
								prefix_ = next_->prefix_;
								leaf_node_ = true;
								link_ = next_->link_;
								next_ = next_->next_;
								delete node_to_be_merged;
								MergeWithNext();
							}
						}
					}
					else if (next_->IsOrphan())
						next_ = nullptr;
				}
			}

			// Returns true if the node has no children and is not the end of a word
			bool IsOrphan()
			{
				return !(next_ || link_ || leaf_node_);
			}

			// Returns the length of the longest common prefix between prefix_ and s
			unsigned int LongestCommonPrefix(const Character *s, unsigned int length_of_s)
			{
				unsigned int count = 0;
				for (; count < __MIN(length_of_s, prefix_.length()); ++count)
				{
					if (s[count] != prefix_[count])
						break;
				}
				return count;
			}

		public:
			// Creates a new node with prefix_ = s[0] s[1] ... s[string_length-1]
			inline Node(const Character *s, unsigned int string_length) :
				next_(nullptr), link_(nullptr), leaf_node_(true), prefix_(s, s + string_length), data_(nullptr) {}
			inline explicit Node(const Character *s) :
				next_(nullptr), link_(nullptr), leaf_node_(true), prefix_(s), data_(nullptr) {}
			inline explicit Node(const String &s) :
				next_(nullptr), link_(nullptr), leaf_node_(true), prefix_(s), data_(nullptr) {}
			virtual ~Node()
			{
				if (next_) delete next_;
				if (link_) delete link_;
			}
		};

	private:
		Node *root_node_;
		int num_words_;

	public:
		inline RadixDictionary() : root_node_(nullptr), num_words_(0)  {}
		virtual ~RadixDictionary()
		{
			if (root_node_)
				delete root_node_;
		}

		// Inserts s in the radix tree in O(m) time where m = strlen(s) and splits the existing nodes if they share a common substring with s
		void Insert(const Character *s, T *data)
		{
			{
				if (strlen(s) == 0)
					return;
				if (!root_node_)
				{
					root_node_ = new Node(s);
					root_node_->data_ = data;
					++num_words_;
				}
				else
				{
					if (root_node_->InternalInsert(s, strlen(s), data))
						++num_words_;
				}
			}
		}

		// Inserts s in the radix tree in O(m) time where m = s.length() and splits the existing nodes if they share a common substring with s
		void Insert(const String &s, T *data)
		{
			if (s.empty())
				return;
			if (!root_node_)
			{
				root_node_ = new Node(s);
				root_node_->data_ = data;
				++num_words_;
			}
			else
			{
				if (root_node_->InternalInsert(s.c_str(), s.length(), data))
					++num_words_;
			}
		}

		// Searches for s in the radix tree in O(m) time where m = strlen(s)
		bool Find(const Character *s)
		{
			if (root_node_)
			{
				Node *n = root_node_->InternalFind(s, strlen(s));
				if (n)
					return n->leaf_node_;
			}
			return false;
		}

		// Searches for s in the radix tree in O(m) time where m = s.length()
		bool Find(const String &s)
		{
			if (root_node_)
			{
				Node *n = root_node_->InternalFind(s.c_str(), s.length());
				if (n)
					return n->leaf_node_;
			}
			return false;
		}

		T *Get(const Character *s)
		{
			if (root_node_)
			{
				Node *n = root_node_->InternalFind(s, strlen(s));
				if (n && n->leaf_node_)
					return n->data_;
			}
			return nullptr;
		}
		
		T *Get(const String &s)
		{
			if (root_node_)
			{
				Node *n = root_node_->InternalFind(s.c_str(), s.length());
				if (n && n->leaf_node_)
					return n->data_;
			}
			return nullptr;
		}
		
		// Deletes s and recursively merges the nodes found on the way
		void Delete(const Character *s)
		{
			if (root_node_)
			{
				root_node_->InternalDelete(s, strlen(s));
				if (root_node_->IsOrphan())
				{
					delete root_node_;
					root_node_ = nullptr;
				}
			}
		}

		// Deletes s and recursively merges the nodes found on the way
		void Delete(const String &s)
		{
			if (root_node_)
			{
				root_node_->InternalDelete(s.c_str(), s.length());
				if (root_node_->IsOrphan())
				{
					delete root_node_;
					root_node_ = nullptr;
				}
			}
		}

		// Returns in v the data whose keys begin with s
		void ExactMatching(const String &s, std::map<String, T*> &v)
		{
			if (root_node_)
				root_node_->ExactMatching(s.c_str(), s.length(), v, "");
		}

		// Returns in v the strings beginning with s
		void ExactMatching(const String &s, std::vector<String> &v)
		{
			if (root_node_)
				root_node_->ExactMatching(s.c_str(), s.length(), v, "");
		}

		// Returns in v the strings whose Damerau-Levenshtein distance from s is lower than 3
		void ApproximateMatching(const String &s, std::vector<String> &v)
		{
			if (root_node_)
				root_node_->ApproximateMatching(v, DamerauLevenshteinDistance(s), __MIN((unsigned int)3, s.length() / 4));
		}

		void ApproximateMatching(const String &s, std::map<String, T*> &v)
		{
			if (root_node_)
				root_node_->ApproximateMatching(v, DamerauLevenshteinDistance(s), __MIN((unsigned int)3, s.length() / 4));
		}
	};
};

#undef __MIN

#undef __USE_CHAR_T
#undef __USE_WCHAR_T
#endif
