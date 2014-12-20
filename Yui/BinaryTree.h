#include <algorithm>
#include <map>
#include <list>
#include <stack>

// Non-balanced binary search tree
template<typename Element>
class BST
{
public:
	// Nodes hold pointers on data objects of type Element
	template<typename Element>
	class Node
	{
		friend class BST<Element>;

	private:
		Element data_;
		Node *left_ = nullptr;
		Node *right_ = nullptr;
		Node *parent_;
		Node *successor_ = nullptr;
		Node *predecessor_ = nullptr;

	public:
		Node(const Element &e, Node<Element> *parent = nullptr) : data_(e), parent_(parent)	{}
		Node(const Element &&e, Node<Element> *parent = nullptr) : data_(std::move(e)), parent_(parent)	{}
		~Node()	{}
		Node *left()	const	{ return left_; }
		Node *right()	const	{ return right_; }
		Node *parent()	const	{ return parent_; }
		Node *successor()	const	{ return successor_; }
		Node *predecessor()	const	{ return predecessor_; }
		const Element &data()	const	{ return data_; }
	};

private:
	Node<Element> *root_ = nullptr;
	Node<Element> *min_ = nullptr;
	Node<Element> *max_ = nullptr;

	void UpdateMinMax(Node<Element> *n)
	{
		if (!min_)
		{
			min_ = n;
			max_ = n;
		}
		else
		{
			if (n->data() < min_->data())
				min_ = n;
			if (n->data() > max_->data())
				max_ = n;
		}
	}

	// Find n's predecessor in log(h) time
	Node<Element> *FindPredecessor(Node<Element> * n)
	{
		if (n->left())
			return GetMax(n->left());
		else
		{
			Node<Element> *child = n;
			Node<Element> *parent = n->parent();
			while (parent)
			{
				if (parent->right() == child)
					return parent;
				child = parent;
				parent = parent->parent();
			}
			return parent;
		}
	}

	// Find n's successor in log(h) time
	Node<Element> *FindSuccessor(Node<Element> * n)
	{
		if (n->right())
			return GetMin(n->right());
		else
		{
			Node<Element> *child = n;
			Node<Element> *parent = n->parent();
			while (parent)
			{
				if (parent->left() == child)
					return parent;
				child = parent;
				parent = parent->parent();
			}
			return parent;
		}
	}

	// Update n's predecessor and successor in log(h) time
	void UpdatePredecessorAndSuccessor(Node<Element> *n)
	{
		Node<Element> *predecessor = FindPredecessor(n);
		if (predecessor)
		{
			// Insert n between predecessor and predecessor->successor()
			if (predecessor->successor())
			{
				predecessor->successor()->predecessor_ = n;
				n->successor_ = predecessor->successor();
			}
			predecessor->successor_ = n;
			n->predecessor_ = predecessor;
		}
		else
		{
			Node<Element> *successor = FindSuccessor(n);
			// Insert n between successor->predecessor() and successor
			if (successor)
			{
				if (successor->predecessor())
				{
					successor->predecessor()->successor_ = n;
					n->predecessor_ = successor->predecessor();
				}
				successor->predecessor_ = n;
				n->successor_ = successor;
			}
		}
	}

	void InternalDelete(Node<Element> *n)
	{
		Node<Element> *n_parent = n->parent();
		// Case 1: n has 2 children
		// Replace n with the minimum element from the subtree whose root is n->right()
		if (n->left() && n->right())
		{
			Node<Element> *min = GetMin(n->right());	// min can't be nullptr
			InternalDelete(min);	// min->left() is nullptr
			// Swap min with n
			min->parent_ = n_parent;
			min->left_ = n->left();
			min->right_ = n->right();
			// Update min's children
			if (n->left())
				n->left_->parent_ = min;
			if (n->right())
				n->right_->parent_ = min;

			if (n_parent)
			{
				if (n_parent->left() == n)
					n_parent->left_ = min;
				else
					n_parent->right_ = min;
			}
			// n is the root
			else
				root_ = min;
		}
		// Case 2: n only has a left child
		// Replace n with n->left()
		else if (n->left() && !n->right())
		{
			Node<Element> *n_left = n->left();
			// Update n_left
			n_left->parent_ = n_parent;
			if (n_parent)
			{
				if (n_parent->left() == n)
					n_parent->left_ = n_left;
				else
					n_parent->right_ = n_left;
			}
			else
				root_ = n_left;
		}
		// Case 3: n only has a right child
		// Replace n with n->right()
		else if (!n->left() && n->right())
		{
			Node<Element> *n_right = n->right();
			// Update n_right
			n_right->parent_ = n_parent;
			if (n_parent)
			{
				if (n_parent->left() == n)
					n_parent->left_ = n_right;
				else
					n_parent->right_ = n_right;
			}
			else
				root_ = n_right;
		}
		// Case 4: n has no children
		else
		{
			if (n_parent)
			{
				if (n_parent->left() == n)
					n_parent->left_ = nullptr;
				else
					n_parent->right_ = nullptr;
			}	
			else
				root_ = nullptr;
		}
			
		// Remove n from the successor/predecessor doubly linked list 
		if (n->predecessor_)
			n->predecessor_->successor_ = n->successor_;
		if (n->successor_)
			n->successor_->predecessor_ = n->predecessor_;
	}

public:
	void Insert(const Element &e)
	{
		Node<Element> **n = &root_;
		Node<Element> *parent = *n;
		// Go down the tree until we find an empty node while satisfying the following restriction:
		// - Any node value is larger than any node value in that node's left subtree
		// - Any node value is smaller than any node value in that node's right subtree
		while (*n)
		{
			parent = *n;
			if (e < (*n)->data())
				n = &((*n)->left_);
			else
				n = &((*n)->right_);
		}
		*n = new Node<Element>(e, parent);
		UpdatePredecessorAndSuccessor(*n);
		UpdateMinMax(*n);
	}

	void Insert(const Element &&e)
	{
		Node<Element> **n = &root_;
		Node<Element> *parent = *n;
		// Go down the tree until we find an empty node while satisfying the following restriction:
		// - Any node value is larger than any node value in that node's left subtree
		// - Any node value is smaller than any node value in that node's right subtree
		while (*n)
		{
			parent = *n;
			if (e < (*n)->data())
				n = &((*n)->left_);
			else
				n = &((*n)->right_);
		}
		*n = new Node<Element>(std::move(e), parent);
		UpdatePredecessorAndSuccessor(*n);
		UpdateMinMax(*n);
	}

	// Returns the node whose value is 'e', or nullptr if the node could not be found. Non-recursive. log(h) time complexity
	// where h is the height of the tree.
	Node<Element> *Find(const Element &e)
	{
		if (!root_)
			return nullptr;
		Node <Element> *n = root_;
		while (e != n->data())
		{
			if (e < n->data())
				n = n->left();
			else
				n = n->right();
			if (!n)
				break;
		}
		return n;
	}

	void Delete(Node<Element> *n)
	{
		InternalDelete(n);

		// Update min_ and max_
		if (min_->data() == n->data())
			min_ = n->successor();
		if (max_->data() == n->data())
			max_ = n->predecessor();
		delete n;
	}

	// Returns the node whose value is minimum in the subtree whose 'root' is passed as argument
	Node<Element> *GetMin(Node<Element> *root)
	{
		Node<Element> *n = root;
		while (n->left())
			n = n->left();
		return n;
	}

	// Returns the node whose value is maximum in the subtree whose 'root' is passed as argument
	Node<Element> *GetMax(Node<Element> *root)
	{
		Node<Element> *n = root;
		while (n->right())
			n = n->right();
		return n;
	}

	// Performs in order traversal of the BST and returns a sorted doubly linked list of the nodes.
	// Non recursive.
	void ConvertToList(std::list<Element> &list)
	{
		std::stack<Node<Element> *> stack;
		Node<Element> *current_node = root_;
		while (current_node || !stack.empty())
		{
			while (current_node)
			{
				stack.push(current_node);
				current_node = current_node->left();
			}
			Node<Element> *n = stack.top();
			stack.pop();
			list.push_back(n->data());
			current_node = n->right();
		}
	}

	Node<Element> *root()	{ return root_; }
	// Returns the min element in constant time
	Node<Element> *min()	{ return min_; }
	// Returns the max element in constant time
	Node<Element> *max()	{ return max_; }
};