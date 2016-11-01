#pragma once

namespace Yui
{
	namespace Euler67
	{
		// Euler problem 67: https://projecteuler.net/problem=67
		class Node
		{
		private:
			int weight_;
			Node *left_;
			Node *right_;
			Node *left_parent_;
			Node *right_parent_;
			int max_sum_so_far_;

			void UpdateMaxPathSumSoFar();

		public:
			Node(int weight);
			virtual ~Node();

			void AttachLeftNode(Node *n);
			void AttachRightNode(Node *n);
			int MaxPathSum();


			__forceinline int weight() { return weight_; }
			__forceinline Node *left() { return left_; }
			__forceinline Node *right() { return right_; }
		};
	}
}