#include "HanoiTower.h"

HanoiTower::HanoiTower(Position source, unsigned int num_disks)
{
	for (int i = num_disks; i > 0; --i)
		spikes_[source].push(i);
}

int HanoiTower::Move(Position source, Position target, unsigned num_disks)
{
	if (source == target)
		return 0;
	if (num_disks == 1)
	{
		int disk = spikes_[source].top();
		spikes_[source].pop();
		spikes_[target].push(disk);
		return 1;
	}
	else
	{
		Position intermediary;
		if (source + target == 1)
			intermediary = Right;
		else if (source + target == 2)
			intermediary = Middle;
		else if (source + target == 3)
			intermediary = Left;

		int num_moves = Move(source, intermediary, num_disks - 1) + 1;
		int disk = spikes_[source].top();
		spikes_[source].pop();
		spikes_[target].push(disk);
		num_moves += Move(intermediary, target, num_disks - 1);
		return num_moves;
	}
}