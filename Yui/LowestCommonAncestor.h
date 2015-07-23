#ifndef __LOWEST_COMMON_ANCESTOR_H__
#define __LOWEST_COMMON_ANCESTOR_H__

#include "RangeMinimumQuery.h"

// Define this symbol to choose the data structure to use to keep track of the first occurrence of a node
// in the Euler tour representation
#ifndef _LCA_FIRST_OCCURRENCE_MAP_TYPE
#include <unordered_map>
#define _LCA_FIRST_OCCURRENCE_MAP_TYPE	std::unordered_map
#endif

// The algorithm needs an O(n) time preprocessing step and can then answer LCA queries in constant time.
// The first step builds the Euler tour representation of the input tree. Then RMQ is applied on the array of the node levels,
// ordered as in the Euler tour representation. Let R denote the array with the position of the first occurrence of a node
// in the Euler tour. Then LCA(n1, n2) = Node whose index is RMQ_level(R[n1], R[n2]).
// https://www.topcoder.com/community/data-science/data-science-tutorials/range-minimum-query-and-lowest-common-ancestor/
namespace Yui
{
	template<class T>
	class LCANode
	{
	private:
		T data_;
		int level_;
		std::vector<LCANode<T>*> children_;

	public:
		LCANode(T data) : data_(data), level_(0)	{}
		void AddChild(LCANode<T>* child)
		{
			children_.push_back(const_cast<LCANode<T>*>(child));
			child->level(level_ + 1);
		}
		inline const std::vector<LCANode<T>*> &children()	const	{ return children_; }
		inline T data()	const	{ return data_; }
		inline int level()	const	{ return level_; }
		inline void level(int l)	{ level_ = l; }
	};

	template<class T>
	void EulerTourDFS(const LCANode<T> *current, std::vector<LCANode<T>*> &euler_tour_nodes, std::vector<int> &levels, _LCA_FIRST_OCCURRENCE_MAP_TYPE<LCANode<T>*, int> &first_occurences_in_tour)
	{
		if (first_occurences_in_tour.find(const_cast<LCANode<T>*>(current)) == first_occurences_in_tour.end())
			first_occurences_in_tour[const_cast<LCANode<T>*>(current)] = euler_tour_nodes.size();
		euler_tour_nodes.push_back(const_cast<LCANode<T>*>(current));
		levels.push_back(current->level());
		int num_children = current->children().size();
		for (int i = 0; i < num_children; ++i)
		{
			EulerTourDFS(current->children()[i], euler_tour_nodes, levels, first_occurences_in_tour);
			euler_tour_nodes.push_back(const_cast<LCANode<T>*>(current));
			levels.push_back(current->level());
		}
	}

	// Input: tree rooted at 'root'. It is not checked whether the input is a tree or not!
	// Output: the ordered list of nodes in the Euler tour starting from the node 'root',
	// array of the levels of each node in the Euler tour, index in euler_tour_nodes of the first
	// occurrence for each node of the tree
	template<class T>
	void EulerTour(const LCANode<T> *root, std::vector<LCANode<T>*> &euler_tour_nodes, std::vector<int> &levels, _LCA_FIRST_OCCURRENCE_MAP_TYPE<LCANode<T>*, int> &first_occurences_in_tour)
	{
		EulerTourDFS(root, euler_tour_nodes, levels, first_occurences_in_tour);
		/*
		for (int i = 0; i < euler_tour_nodes.size(); ++i)
			cout << euler_tour_nodes[i]->data() << " ";
		cout << endl;
		for (int i = 0; i < levels.size(); ++i)
			cout << levels[i] << " ";
		cout << endl;
		for (auto it = first_occurences_in_tour.begin(); it != first_occurences_in_tour.end(); ++it)
			cout << it->first->data() << "(" << it->second << ") ";
		cout << endl;
		*/
	}


	template<class T>
	class LowestCommonAncestor
	{
		typedef	_LCA_FIRST_OCCURRENCE_MAP_TYPE<LCANode<T>*, int> FirstOccurrenceMap;

	private:
		std::vector<LCANode<T>*> euler_tour_nodes_;
		// Level of each node in euler_tour_nodes_. Restricted RMQ is applicable to this array
		std::vector<int> levels_;
		// Index in euler_tour_nodes of the first occurrence for each node of the tree
		FirstOccurrenceMap first_occurences_in_tour_;
		RestrictedRMQ *RMQ_;

	public:
		LowestCommonAncestor(const LCANode<T> *root)
		{
			EulerTour(root, euler_tour_nodes_, levels_, first_occurences_in_tour_);
			RMQ_ = new RestrictedRMQ(&levels_[0], levels_.size());
		}

		~LowestCommonAncestor()	{ delete RMQ_; }

		inline LCANode<T> *LCA(LCANode<T> *n1, LCANode<T> *n2) { return euler_tour_nodes_[RMQ_->MinInRange(first_occurences_in_tour_[n1], first_occurences_in_tour_[n2])]; }
	};
}

#if 0
int main() {
	Node<char> *root = new Node<char>('A');
	Node<char> *B = new Node<char>('B');
	Node<char> *C = new Node<char>('C');
	Node<char> *D = new Node<char>('D');
	Node<char> *E = new Node<char>('E');
	Node<char> *F = new Node<char>('F');
	root->AddChild(B);
	root->AddChild(C);
	B->AddChild(D);
	D->AddChild(E);
	D->AddChild(F);

	LowestCommonAncestor<char> LCA(root);
	cout << LCA.LCA(root, root)->data() << endl;
	cout << LCA.LCA(root, F)->data() << endl;
	cout << LCA.LCA(B, D)->data() << endl;
	cout << LCA.LCA(E, C)->data() << endl;
	cout << LCA.LCA(E, F)->data() << endl;

	return 0;
}
#endif

#endif
