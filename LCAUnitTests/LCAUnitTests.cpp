#include <LowestCommonAncestor.h>

#include <gtest/gtest.h>

TEST(Queries, LowestCommonAncestor)
{
	Yui::LCANode<char> *root = new Yui::LCANode<char>('A');
	Yui::LCANode<char> *B = new Yui::LCANode<char>('B');
	Yui::LCANode<char> *C = new Yui::LCANode<char>('C');
	Yui::LCANode<char> *D = new Yui::LCANode<char>('D');
	Yui::LCANode<char> *E = new Yui::LCANode<char>('E');
	Yui::LCANode<char> *F = new Yui::LCANode<char>('F');
	root->AddChild(B);
	root->AddChild(C);
	B->AddChild(D);
	D->AddChild(E);
	D->AddChild(F);

	Yui::LowestCommonAncestor<char> LCA(root);
	EXPECT_EQ('A', LCA.LCA(root, root)->data());
	EXPECT_EQ('B', LCA.LCA(B, B)->data());
	EXPECT_EQ('A', LCA.LCA(root, F)->data());
	EXPECT_EQ('B', LCA.LCA(B, D)->data());
	EXPECT_EQ('A', LCA.LCA(E, C)->data());
	EXPECT_EQ('D', LCA.LCA(E, F)->data());
	EXPECT_EQ('B', LCA.LCA(B, F)->data());
}