#include "BinaryTree.h"
#include "Sort.h"
#include "RadixTree.h"
#include "RBTree.h"
#include "DamerauLevenshteinDistance.h"
#include "HanoiTower.h"
#include "EggDroppingPuzzle.h"
#include "QuickSelect.h"
#include "StringSearching.h"
#include "SegmentTree.h"
#include "LowestCommonAncestor.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <iterator>
#include <omp.h>
#include <chrono>

#define __NUM_ELEMENTS	10000000

class Test
{
private:
	int i_;
public:
	explicit Test(int i) : i_(i)	{}
	Test(const Test &t) : i_(t.i_)	{}
	Test(const Test &&t) : i_(std::move(t.i_))	{};
	Test &operator=(const Test &t)	{ i_ = t.i_; return *this; }
	bool operator<(const Test &t)	const	{ return i_ < t.i_; }
	bool operator>(const Test &t)	const	{ return i_ > t.i_; }
	bool operator!=(const Test &t)	const	{ return i_ != t.i_; }
	bool operator==(const Test &t)	const	{ return i_ == t.i_; }
};


int main()
{
#if 0
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
	cout << LCA.LCA(root, root)->data() << endl;
	cout << LCA.LCA(root, F)->data() << endl;
	cout << LCA.LCA(B, D)->data() << endl;
	cout << LCA.LCA(E, C)->data() << endl;
	cout << LCA.LCA(E, F)->data() << endl;

	auto min_cost_matrix_mul = MinCostMult({ { 10, 30 }, { 30, 5 }, { 5, 60 }, {60, 1} }, 0, 4);
	int idx = Yui::KMPSearch("ABABABC", "ABC");

	std::vector<int> test_st({ 2, 5, 1, 4, 9, 3 });
	Yui::SegmentTree<int> segment_tree(test_st);
	for (int i = 0; i < segment_tree.num_nodes(); ++i)
		if (segment_tree.nodes()[i].defined_)
			std::cout << segment_tree.nodes()[i].min_ << "[" << segment_tree.nodes()[i].start_ << "," << segment_tree.nodes()[i].end_ << "]\n";
	int r_min = segment_tree.GetMinInRange(3, 5);

	int *array1 = new int[100];
	for (int i = 0; i < 100; ++i)
		array1[i] = i+100;
	std::vector<int> test_v1(array1, array1 + 100);
	int *array2 = new int[200];
	for (int i = 0; i < 100; ++i)
		array2[i] = i;
	std::vector<int> test_v2(array2, array2 + 200);
	Yui::PMerge(test_v2.begin(), 100, test_v1.begin(), 100, Yui::IsLess<int>, 1);

	int *array = new int[__NUM_ELEMENTS];
	for (int i = 0; i < __NUM_ELEMENTS; ++i)
		array[i] = __NUM_ELEMENTS - i;
	std::random_shuffle(array, array + __NUM_ELEMENTS);

	std::vector<int> test_v(array, array + __NUM_ELEMENTS);

	//auto it = Yui::QuickSelectMax(test_v.begin(), test_v.end(), 1);

	auto t_start = std::chrono::high_resolution_clock::now();
	Yui::MergeSort(test_v.begin(), test_v.end(), 2);
	auto t_end = std::chrono::high_resolution_clock::now();

	std::cout << "Time elapsed for MergeSort " << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms\n";

	
	int num_tries = EggDroppingPuzzle::SolveDP(2, 20);
	HanoiTower hanoi(HanoiTower::Left, 6);
	int num_moves =	hanoi.Move(HanoiTower::Left, HanoiTower::Middle, 6);
	std::vector<Test> v;
	v.push_back(Test(6));
	v.push_back(Test(2));
	v.push_back(Test(1));
	v.push_back(Test(3));
	v.push_back(Test(4));
	v.push_back(Test(5));
	v.push_back(Test(7));
	Yui::HeapSort(v.begin(), v.end());

	Yui::Heap<Test> heap;
	heap.Push(Test(6));
	heap.Push(Test(2));
	heap.Push(Test(1));
	heap.Push(Test(3));
	heap.Push(Test(4));
	heap.Push(Test(0));
	Test t(0);
	while (!heap.IsEmpty())
		heap.Pop(t);

	Yui::BST<Test> test;
	test.Insert(Test(0));
	test.Insert(Test(-1));
	test.Insert(Test(4));
	test.Insert(Test(3));
	test.Insert(Test(1));
	test.Insert(Test(2));
	std::list<Test> list;
	test.ConvertToList(list);
	auto n = test.Find(Test(2));
	test.Delete(n);

	RBTree<int> RBtree;
	RBtree.Insert(1);
	RBtree.Insert(8);
	RBtree.Insert(13);
	RBtree.Insert(11);
	RBtree.Insert(6);
	RBtree.Insert(15);
	RBtree.Insert(17);
	RBtree.Insert(25);
	RBtree.Insert(22);
	RBtree.Insert(27);

	auto n_RBTree_min = RBtree.GetMin(RBtree.root());
	auto n_RBTree_max = RBtree.GetMax(RBtree.root());
	RBtree.Delete(RBtree.root());
#endif

	Yui::RadixTree radix_tree;
	std::wifstream file("..\\SCOWL\\english-words.10");
	std::wstring line;
	while (file >> line)
		radix_tree.Insert(line);
	file.close();
	file.open("..\\SCOWL\\english-words.20");
	while (file >> line)
		radix_tree.Insert(line);
	file.close();

	std::vector<Yui::RadixTree::String> matches;
	radix_tree.ApproximateMatching(L"collegue", matches);

	for (auto s : matches)
		std::wcout << s << std::endl;

#ifdef _DEBUG
	Yui::DamerauLevenshteinDistance distance(L"Tamqsd", L"Tamarin");
	distance.PrintDistance();
#endif
	system("pause");
}