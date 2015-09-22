#include <boost/heap/fibonacci_heap.hpp>
#include <vector>
#include <unordered_map>

struct Edge
{
	bool start;
	double position;
	double height;
};

std::vector<std::vector<double>> SkylineTestCase1()
{
	std::vector<std::vector<double>> buildings;
	buildings.push_back({ 2, 9, 10 });
	buildings.push_back({ 3, 7, 15 });
	buildings.push_back({ 5, 12, 12 });
	buildings.push_back({ 15, 20, 10 });
	buildings.push_back({ 15, 18, 5 });
	buildings.push_back({ 19, 24, 8 });
	return buildings;
}

// https://leetcode.com/problems/the-skyline-problem/
// Input : building list where each building is modelled as a triplet {left_edge.position, right_edge.position, height}
// Runtime : O(B*log(B)) where B is the number of buildings
// Expected output { { 2, 10 }, { 3, 15 }, { 7, 12 }, { 12, 0 }, { 15, 10 }, { 20, 8 }, { 24, 0 } }
std::vector<std::pair<int, int>> GetSkyline(std::vector<std::vector<double>> &buildings)
{
	std::vector<std::pair<int, int>> keypoints;
	std::vector<Edge> edges;
	// Create two edges for each building and sort them by position, height and starting edge
	for (auto const &building : buildings)
	{
		edges.push_back({ true, building[0], building[2] });
		edges.push_back({ false, building[1], building[2] });
	}
	std::sort(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2) -> bool
	{
		if (e1.position != e2.position)
			return e1.position < e2.position;
		if (e1.start && e2.start)
			return e2.height < e1.start;
		if (!(e1.start || e2.start))
			return e1.height < e2.height;
		return e2.start;
	});

	typedef boost::heap::fibonacci_heap<double> FibonacciHeap;	// Height priority queue
	FibonacciHeap heap;
	std::unordered_multimap<double, FibonacciHeap::handle_type> handles;
	for (auto const &edge : edges)
	{
		if (edge.start)
		{
			double height = edge.height;
			if (heap.empty() || height > heap.top())
				keypoints.push_back({ edge.position, height });
			handles.insert(std::make_pair(height, heap.push(height)));
		}
		else
		{
			// Erase from the height priority queue the starting edge of the building and add a keypoint if the current edge height is greater than the remaining edges in the priority queue 
			double height = edge.height;
			auto handle_it = handles.find(height);
			heap.erase(handle_it->second);
			handles.erase(handle_it);
			if (heap.empty())
				keypoints.push_back({ edge.position, 0 });
			else if(height > heap.top())
				keypoints.push_back({ edge.position, heap.top() });
		}
	}
	return keypoints;
}
