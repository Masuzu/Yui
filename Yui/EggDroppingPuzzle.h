#ifndef __EGG_DROPPING_PUZZLE
#define __EGG_DROPPING_PUZZLE

// From http://en.wikipedia.org/wiki/Dynamic_programming#Egg_dropping_puzzle
// Suppose that we wish to know which storeys in a 36-storey building are safe to drop eggs from, and which will cause the eggs to break on landing, under the following assumptions:
// An egg that survives a fall can be used again.
// A broken egg must be discarded.
// The effect of a fall is the same for all eggs.
// If an egg breaks when dropped, then it would break if dropped from a higher window.
// If an egg survives a fall, then it would survive a shorter fall.
// It is not ruled out that the first - floor windows break eggs, nor is it ruled out that eggs can survive the 36th - floor windows.
// If only one egg is available and we wish to be sure of obtaining the right result, the experiment can be carried out in only one way.Drop the egg from the first - floor window; if it survives, drop it from the second - floor window.Continue upward until it breaks.In the worst case, this method may require 36 droppings.Suppose 2 eggs are available.What is the least number of egg - droppings that is guaranteed to work in all cases ?
// To derive a dynamic programming functional equation for this puzzle, let the state of the dynamic programming model be a pair s = (n, k), where
// n = number of test eggs available
// k = number of(consecutive) floors yet to be tested
// Let T(n,k) be the minimum number of trials required to identify the value of the critical floor under the worst-case scenario
// It can be shown that T(n,k) = 1 + min{max(T(n − 1, x − 1), T(n,k − x)): x = 1, 2, ..., k}:
// Case 1: the egg breaks after dropping from the xth floor, then the (x-1) floors below must be checked with the n-1 remaining eggs
// Case 2: the egg does not break after dropping from the xth floor, then the (k-x) floor above must be checked with the remaining n eggs
namespace EggDroppingPuzzle
{
	// Returns the minimum number of tries for the egg dropping puzzle for the state (n, k) where
	// n = number of test eggs available
	// k = number of(consecutive) floors yet to be tested
	unsigned int SolveRecursive(unsigned int n, unsigned int k);

	// O(nk^2) time complexity, O(nk) space complexity dynamic programming solution
	unsigned int SolveDP(unsigned int n, unsigned int k);
};

#endif