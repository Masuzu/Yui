#include <vector>

// Counts the number of islands in a 2D grid
template<class T>
class Islands 
{
private:
	int height_;
	int width_;
	std::vector<std::vector<bool>> visited_;
	T empty_symbol_;

	void ExpandFrom(int i, int j, const std::vector<std::vector<T>>& grid)
	{
		if (i - 1 >= 0)
		{
			if (grid[i - 1][j] != empty_symbol_ && !visited_[i - 1][j])
			{
				visited_[i - 1][j] = visited_[i][j];
				ExpandFrom(i - 1, j, grid);
			}
		}
		if (j - 1 >= 0)
		{
			if (grid[i][j - 1] != empty_symbol_ && !visited_[i][j - 1])
			{
				visited_[i][j - 1] = visited_[i][j];
				ExpandFrom(i, j - 1, grid);
			}
		}
		if (i + 1 < height_)
		{
			if (grid[i + 1][j] != empty_symbol_ && !visited_[i + 1][j])
			{
				visited_[i + 1][j] = visited_[i][j];
				ExpandFrom(i + 1, j, grid);
			}
		}
		if (j + 1 < width_)
		{
			if (grid[i][j + 1] != empty_symbol_ && !visited_[i][j + 1])
			{
				visited_[i][j + 1] = visited_[i][j];
				ExpandFrom(i, j + 1, grid);
			}
		}
	}

public:
	int NumIslands(const std::vector<std::vector<T>>& grid) {
		height_ = grid.size();
		if (height_ == 0)
			return 0;
		width_ = grid[0].size();

		for (int i = 0; i < height_; ++i)
		{
			visited_.push_back(std::vector<bool>());
			for (int j = 0; j < width_; ++j)
				visited_[i].push_back(false);
		}
		int num_islands = 0;
		for (int i = 0; i < height_; ++i)
		{
			for (int j = 0; j<width_; ++j)
			{
				if (grid[i][j] == '1' && !visited_[i][j])
				{
					++num_islands;
					visited_[i][j] = true;
					ExpandFrom(i, j, grid);
				}

			}
		}
		return num_islands;
	}
};