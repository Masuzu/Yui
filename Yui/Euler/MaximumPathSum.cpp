#include "MaximumPathSum.h"

#include <queue>

#define __MIN(a, b) ((a) < (b) ? (a) : (b))
#define __MAX(a, b) ((a) > (b) ? (a) : (b))

namespace Yui
{
	namespace Euler67
	{
		Node::Node(int weight) :
			weight_(weight),
			left_(nullptr),
			right_(nullptr),
			max_sum_so_far_(0),
			left_parent_(nullptr),
			right_parent_(nullptr)
		{ }

		Node::~Node()
		{
			if (left_);
				delete left_;
			if (right_)
				delete right_;
		}

		void Node::UpdateMaxPathSumSoFar()
		{
			if (!left_parent_ && !right_parent_)
			{
				max_sum_so_far_ = weight_;
				return;
			}

			int left_parent_weight = INT_MIN;
			int right_parent_weight = INT_MIN;
			if (left_parent_)
				left_parent_weight = left_parent_->max_sum_so_far_;
			if (right_parent_)
				right_parent_weight = right_parent_->max_sum_so_far_;
			max_sum_so_far_ = __MAX(left_parent_weight, right_parent_weight) + weight_;
		}

		int Node::MaxPathSum()
		{
			std::queue<Node*> queue;
			queue.push(this);
			int max_path_sum = INT_MIN;
			while (!queue.empty())
			{
				Node *node = queue.front();
				queue.pop();
				node->UpdateMaxPathSumSoFar();
				if (node->left())
					queue.push(node->left());
				if (node->right())
					queue.push(node->right());
				if (!node->left() && !node->right())
					max_path_sum = __MAX(max_path_sum, node->max_sum_so_far_);
			}
			return max_path_sum;
		}

		void Node::AttachLeftNode(Node *n)
		{
			left_ = n;
			n->right_parent_ = this;
		}

		void Node::AttachRightNode(Node *n)
		{
			right_ = n;
			n->left_parent_ = this;
		}
	}
}