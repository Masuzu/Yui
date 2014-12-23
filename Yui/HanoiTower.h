#ifndef __HANOI_TOWER_H__
#define __HANOI_TOWER_H__

#include <stack>

class HanoiTower
{
private:
	std::stack<int> spikes_[3];
	HanoiTower()	{}

public:
	enum Position { Left, Middle, Right };

	HanoiTower(Position source, unsigned int num_disks);
	// Returns the number of moves made
	int Move(Position source, Position target, unsigned num_disks);
};

#endif