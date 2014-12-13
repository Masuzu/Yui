#include "BinaryTree.h"
#include "Sort.h"
#include "RadixTree.h"
#include "DamerauLevenshteinDistance.h"

#include <iostream>
#include <vector>

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
	std::vector<Test> v;
	v.push_back(Test(6));
	v.push_back(Test(2));
	v.push_back(Test(1));
	v.push_back(Test(3));
	v.push_back(Test(4));
	v.push_back(Test(5));
	v.push_back(Test(7));
	HeapSort(v.begin(), v.end());

	Heap<Test> heap;
	heap.Push(Test(6));
	heap.Push(Test(2));
	heap.Push(Test(1));
	heap.Push(Test(3));
	heap.Push(Test(4));
	heap.Push(Test(0));
	Test t(0);
	while (!heap.IsEmpty())
		heap.Pop(t);

	BST<Test> test;
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

	RadixTree radix_tree;
	radix_tree.Insert("Tama");
	radix_tree.Insert("Tamaru");
	radix_tree.Insert("Tamarin");
	std::vector<RadixTree::String> matches;
	radix_tree.ExactMatching("Tama", matches);
	bool b = radix_tree.Find("Tam");
	b = radix_tree.Find("Tamar");
	radix_tree.Delete("Tamaru");
	radix_tree.Delete("Tamarin");
	radix_tree.Delete("Tama");

	DamerauLevenshteinDistance distance("timng", "timing");
	distance.PrintDistance();
	distance.UpdateDistance("ming");
	distance.PrintDistance();
	system("pause");
}