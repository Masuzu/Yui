#include "BinaryTree.h"
#include "Sort.h"
#include "RadixTree.h"
#include "RBTree.h"
#include "DamerauLevenshteinDistance.h"
#include "HanoiTower.h"
#include "EggDroppingPuzzle.h"
#include "QuickSelect.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <iterator>
#include <omp.h>
#include <chrono>

#define __NUM_ELEMENTS	50

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

bool IntComparator(int a, int b)
{
	return a > b;
}

int main()
{
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
	Yui::MergeSort(test_v.begin(), test_v.end(), 1);
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

	Yui::RadixTree radix_tree;
	std::ifstream file("..\\SCOWL\\english-words.10");
	std::string line;
	while (file >> line)
		radix_tree.Insert(line);
	file.close();
	file.open("..\\SCOWL\\english-words.20");
	while (file >> line)
		radix_tree.Insert(line);
	file.close();

	std::vector<Yui::RadixTree::String> matches;
	radix_tree.ApproximateMatching("teach", matches);

	for (auto s : matches)
		std::cout << s << std::endl;

#ifdef _DEBUG
	Yui::DamerauLevenshteinDistance distance("Tamqsd", "Tamarin");
	distance.PrintDistance();
#endif
	system("pause");
}