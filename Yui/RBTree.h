#ifndef __RB_TREE_H__
#define __RB_TREE_H__

// Red-black trees have to comply with the following properties:
// 1. A node is either red or black.
// 2. The root is black.
// 3. All leaves are black. Every newly inserted node has two nil children (left and right), both of them defining leaf nodes.
// 4. Every red node must have two black child nodes.
// 5. Every path from a given node to any of its descendant leaves contains the same number of black nodes.
template<typename Element> class RBTree
{
private:
	template<typename Element>
	class Node
	{
		friend class RBTree<Element>;
		
	public:
		typedef bool Color;
		const Color kRed = true;
		const Color kBlack = false;

	private:
		Color color_ = kBlack;
		Element data_;
		Node *parent_;
		Node *left_ = nullptr;
		Node *right_ = nullptr;

		void RotateLeft()
		{
			Node<Element> *saved_right = right_;
			Node<Element> *saved_parent = parent_;
			right_ = saved_right->left_;
			if (right_)
				right_->parent_ = this;		
			parent_ = saved_right;
			if (saved_parent)
			{
				if (saved_parent->left_ == this)
					saved_parent->left_ = saved_right;
				else
					saved_parent->right_ = saved_right;
			}
			if (saved_right)
			{
				saved_right->left_ = this;
				saved_right->parent_ = saved_parent;
			}
		}

		void RotateRight()
		{
			Node<Element> *saved_left = left_;
			Node<Element> *saved_parent = parent_;
			left_ = saved_left->right_;
			if (left_)
				left_->parent_ = this;
			parent_ = saved_left;
			if (saved_parent)
			{
				if (saved_parent->left_ == this)
					saved_parent->left_ = saved_left;
				else
					saved_parent->right_ = saved_left;
			}
			if (saved_left)
			{
				saved_left->right_ = this;
				saved_left->parent_ = saved_parent;
			}
		}

		// Nil node
		Node(Node *parent) : parent_(parent)	{}
	public:
		Node(const Element &e, Node *parent = nullptr) : data_(e), parent_(parent)
		{
			left_ = new Node<Element>(this);
			right_ = new Node<Element>(this);
		}
		Node(const Element &&e, Node *parent = nullptr) : data_(std::move(e)), parent_(parent)
		{
			left_ = new Node<Element>(this);
			right_ = new Node<Element>(this);
		}

		~Node()
		{
			if (left_)
			{
				if (left_->IsLeaf())
					delete left_;
			}
			if (right_)
			{
				if (right_->IsLeaf())
					delete right_;
			}
		}

		inline const Element &data()	const	{ return data_; }
		inline bool IsBlack()	const	{ return color_ == kBlack; }
		inline bool IsRed()	const	{ return color_ == kRed; }
		inline void PaintInBlack()	{ color_ = kBlack; }
		inline void PaintInRed()	{ color_ = kRed; }
		inline bool IsLeaf() const	{ return !(left_ || right_); }
	};
	
	Node<Element> *root_ = nullptr;

	// Returns the sibling of n's parent node
	Node<Element> *FindUncle(Node<Element> *n)
	{
		if (n->parent_)
		{
			Node<Element> *grand_parent = n->parent_->parent_;
			if (grand_parent)
			{
				if (grand_parent->left_ == n->parent_)
					return grand_parent->right_;
				else
					return grand_parent->left_;
			}
			else
				return nullptr;
		}
		else
			return nullptr;
	}

	// Action taken if n is the root of the tree
	void InternalInsertAtRoot(Node<Element> *n)
	{
		if (!n->parent_)
			n->PaintInBlack();
		else
			InternalInsertBlackParent(n);
	}

	// Action taken if n's parent is black
	void InternalInsertBlackParent(Node<Element> *n)
	{
		if (n->parent_->IsBlack())
			// Properties 4 and 5 still hold
			return;
		else
			InternalInsertRedParentAndUncle(n);
	}

	// Action taken if both n's parent and uncle are red
	void InternalInsertRedParentAndUncle(Node<Element> *n)
	{
		Node<Element> *uncle = FindUncle(n);
		if (uncle)
		{
			if (uncle->IsRed())
			{
				// Paint the grand parent in red and repaint the parent and uncle in black
				uncle->parent_->PaintInRed();
				n->parent_->PaintInBlack();
				uncle->PaintInBlack();
				// However, the grand parent may now violate property 2 or 4 (the grand grand parent may be red)
				InternalInsertAtRoot(uncle->parent_);
			}
			else
				InternalInsertRedParentAndBlackUncle(n);
		}
	}

	// Action taken if n's parent is red but n's uncle is black
	void InternalInsertRedParentAndBlackUncle(Node<Element> *n)
	{
		Node<Element> *parent = n->parent_;
		Node<Element> *grand_parent = parent->parent_;
		// If n is the right child of parent and parent in turn is the left child of its parent grand_parent, rotate parent left
		// then deal with the former parent
		if (parent->right_ == n && grand_parent->left_ == parent)
		{
			parent->RotateLeft();
			if (parent == root_)
				root_ = n;
			InternalInsertRedParentAndBlackUncleLast(parent);
		}
		else if (parent->left_ == n && grand_parent->right_ == parent)
		{
			parent->RotateRight();
			if (parent == root_)
				root_ = n;
			InternalInsertRedParentAndBlackUncleLast(parent);
		}
		else
			InternalInsertRedParentAndBlackUncleLast(n);
	}

	void InternalInsertRedParentAndBlackUncleLast(Node<Element> *n)
	{
		Node<Element> *parent = n->parent_;
		Node<Element> *grand_parent = parent->parent_;
		parent->PaintInBlack();
		grand_parent->PaintInRed();
		if (parent->right_ == n)
			grand_parent->RotateLeft();
		else if (parent->left_ == n)
			grand_parent->RotateRight();
		// Fix the root node
		if (root_ == grand_parent)
			root_ = grand_parent->parent_;
	}

public:
	// O(log n) worst case time complexity
	void Insert(const Element &e)
	{
		Node<Element> **n = &root_;
		// n's parent
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
			if ((*n)->IsLeaf())
				break;
		}
		delete (*n);
		*n = new Node<Element>(e, parent);
		(*n)->PaintInRed();
		
		InternalInsertAtRoot(*n);
	}

	// O(log n) worst case time complexity
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
			if ((*n)->IsLeaf())
				break;
		}
		delete (*n);
		*n = new Node<Element>(std::move(e), parent);
		(*n)->PaintInRed();

		InternalInsertAtRoot(*n);
	}

	// Returns the node whose value is 'e', or nullptr if the node could not be found. log(n) time complexity.
	Node<Element> *Find(const Element &e)
	{
		if (!root_)
			return nullptr;
		Node <Element> *n = root_;
		while (e != n->data())
		{
			if (e < n->data())
				n = n->left_;
			else
				n = n->right_;
			if (!n)
				break;
		}
		if (n)
		{
			if (n->IsLeaf())
				return nullptr;
		}
		return n;
	}
};

#endif