#include <vector>
#include <queue>

namespace Yui
{
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
			// BFS traversal, queue of the next coordinates to visit
			std::queue<std::pair<int, int>> queue;
			queue.push({ i, j });
			while (!queue.empty())
			{
				auto coordinates = queue.front();
				queue.pop();
				if (coordinates.first - 1 >= 0)
				{
					if (grid[coordinates.first - 1][coordinates.second] != empty_symbol_ && !visited_[coordinates.first - 1][coordinates.second])
					{
						visited_[coordinates.first - 1][coordinates.second] = visited_[coordinates.first][coordinates.second];
						queue.push({ coordinates.first - 1, coordinates.second });
					}
				}
				if (coordinates.second - 1 >= 0)
				{
					if (grid[coordinates.first][coordinates.second - 1] != empty_symbol_ && !visited_[coordinates.first][coordinates.second - 1])
					{
						visited_[coordinates.first][coordinates.second - 1] = visited_[coordinates.first][coordinates.second];
						queue.push({ coordinates.first, coordinates.second - 1 });
					}
				}
				if (coordinates.first + 1 < height_)
				{
					if (grid[coordinates.first + 1][coordinates.second] != empty_symbol_ && !visited_[coordinates.first + 1][coordinates.second])
					{
						visited_[coordinates.first + 1][coordinates.second] = visited_[coordinates.first][coordinates.second];
						queue.push({ coordinates.first + 1, coordinates.second });
					}
				}
				if (coordinates.second + 1 < width_)
				{
					if (grid[coordinates.first][coordinates.second + 1] != empty_symbol_ && !visited_[coordinates.first][coordinates.second + 1])
					{
						visited_[coordinates.first][coordinates.second + 1] = visited_[coordinates.first][coordinates.second];
						queue.push({ coordinates.first, coordinates.second + 1 });
					}
				}
			}


		}

	public:
		Islands(T empty_symbol) : empty_symbol_(empty_symbol)
		{

		}

		unsigned int NumIslands(const std::vector<std::vector<T>>& grid) {
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
			unsigned int num_islands = 0;
			for (int i = 0; i < height_; ++i)
			{
				for (int j = 0; j < width_; ++j)
				{
					if (grid[i][j] != empty_symbol_ && !visited_[i][j])
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
}