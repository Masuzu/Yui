#include <Euler/MaximumPathSum.h>

#include <gtest/gtest.h>

using namespace Yui::Euler67;

TEST(SingleNode, MaximumPathSum)
{
	const int kRootWeight = 612321;
	Node *root = new Node(kRootWeight);
	EXPECT_EQ(kRootWeight, root->MaxPathSum());
}

TEST(SimpleTree, MaximumPathSum)
{
	/*
	                5
			-3            3
		  5    10     -7     1
	*/
	Node *root = new Node(5);
	Node *root_left = new Node(-3);
	Node *root_right = new Node(3);
	root->AttachLeftNode(root_left);
	root->AttachRightNode(root_right);
	root_left->AttachLeftNode(new Node(5));
	root_left->AttachRightNode(new Node(10));
	root_right->AttachLeftNode(new Node(-7));
	root_right->AttachRightNode(new Node(1));

	EXPECT_EQ(12, root->MaxPathSum());
}