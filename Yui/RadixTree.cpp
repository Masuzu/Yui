#include "RadixTree.h"

#include <stack>

#define __MIN(x, y)	(((x) < (y)) ? (x) : (y))

namespace Yui
{
	bool RadixTree::Node::InternalInsert(const Character *s, unsigned int length_of_s)
	{
		// Find the node which has a common prefix with s
		unsigned int l = LongestCommonPrefix(s, length_of_s);
		if (l == 0)
		{
			if (next_)
				return next_->InternalInsert(s, length_of_s);
			else
			{
				next_ = new Node(s, length_of_s);
				return true;
			}
		}
		else
		{
			Node *n = Split(l);
			if (l < length_of_s)
			{
				if (n->link_)
					return n->link_->InternalInsert(s + l, length_of_s - l);
				else
				{
					n->link_ = new Node(s + l, length_of_s - l);
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

	RadixTree::Node *RadixTree::Node::InternalFind(const Character *s, unsigned int length_of_s)
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

	void RadixTree::Node::InternalDelete(const Character *s, unsigned int length_of_s)
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

	RadixTree::Node *RadixTree::Node::Split(unsigned int k)
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

	void RadixTree::Node::ExactMatching(const Character *s, unsigned int length_of_s, std::vector<String> &v, const String &prefix)
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

	void RadixTree::Node::DFS(std::vector<String> &v, const String &prefix)
	{
		if (leaf_node_)
			v.push_back(prefix + prefix_);
		if (link_)
			link_->DFS(v, prefix + prefix_);
		if (next_)
			next_->DFS(v, prefix);
	}

	void RadixTree::Node::ApproximateMatching(std::vector<String> &v, DamerauLevenshteinDistance distance, int max_distance)
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

	void RadixTree::Node::MergeWithLink()
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
					// Make node_to_be_merged an orphan to avoid deleting its children
					node_to_be_merged->link_ = nullptr;
					node_to_be_merged->next_ = nullptr;
					delete node_to_be_merged;
				}
			}
			else if (link_->IsOrphan())
				link_ = nullptr;
		}
	}

	void RadixTree::Node::MergeWithNext()
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
						// Make node_to_be_merged an orphan to avoid deleting its children
						node_to_be_merged->link_ = nullptr;
						node_to_be_merged->next_ = nullptr;
						delete node_to_be_merged;
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
						// Make node_to_be_merged an orphan to avoid deleting its children
						node_to_be_merged->link_ = nullptr;
						node_to_be_merged->next_ = nullptr;
						delete node_to_be_merged;
					}
				}
			}
			else if (next_->IsOrphan())
				next_ = nullptr;
		}
	}

	bool RadixTree::Node::IsOrphan()
	{
		return !(next_ || link_ || leaf_node_);
	}

	unsigned int RadixTree::Node::LongestCommonPrefix(const Character *s, unsigned int length_of_s)
	{
		unsigned int count = 0;
		for (; count < __MIN(length_of_s, prefix_.length()); ++count)
		{
			if (s[count] != prefix_[count])
				break;
		}
		return count;
	}

	RadixTree::~RadixTree()
	{
		if (root_node_)
			delete root_node_;
	}

	void RadixTree::Insert(const Character *s)
	{
		if (strlen(s) == 0)
			return;
		if (!root_node_)
		{
			root_node_ = new Node(s);
			++num_words_;
		}
		else
		{
			if (root_node_->InternalInsert(s, strlen(s)))
				++num_words_;
		}
	}

	void RadixTree::Insert(const String &s)
	{
		if (s.empty())
			return;
		if (!root_node_)
		{
			root_node_ = new Node(s);
			++num_words_;
		}
		else
		{
			if (root_node_->InternalInsert(s.c_str(), s.length()))
				++num_words_;
		}
	}

	bool RadixTree::Find(const Character *s)
	{
		if (root_node_)
		{
			Node *n = root_node_->InternalFind(s, strlen(s));
			if (n)
				return n->leaf_node_;
		}
		return false;
	}

	bool RadixTree::Find(const String &s)
	{
		if (root_node_)
		{
			Node *n = root_node_->InternalFind(s.c_str(), s.length());
			if (n)
				return n->leaf_node_;
		}
		return false;
	}

	void RadixTree::Delete(const Character *s)
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

	void RadixTree::Delete(const String &s)
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

	void RadixTree::ExactMatching(const String &s, std::vector<String> &v)
	{
		if (root_node_)
			root_node_->ExactMatching(s.c_str(), s.length(), v, "");
	}

	void RadixTree::ApproximateMatching(const String &s, std::vector<String> &v)
	{
		if (root_node_)
			root_node_->ApproximateMatching(v, DamerauLevenshteinDistance(s), __MIN(unsigned int(3), s.length() / 4));
	}
};
