class Islands {
	int h;
	int w;
	vector<vector<bool>> copy;
public:
	void expandFrom(int i, int j, vector<vector<char>>& grid)
	{
		cout << "expandFrom" << i << " " << j << endl;
		if (i - 1 >= 0)
		{
			if (grid[i - 1][j] == '1' && !copy[i - 1][j])
			{
				copy[i - 1][j] = copy[i][j];
				expandFrom(i - 1, j, grid);
			}
		}
		if (j - 1 >= 0)
		{
			if (grid[i][j - 1] == '1' && !copy[i][j - 1])
			{
				copy[i][j - 1] = copy[i][j];
				expandFrom(i, j - 1, grid);
			}
		}
		if (i + 1<h)
		{
			if (grid[i + 1][j] == '1' && !copy[i + 1][j])
			{
				copy[i + 1][j] = copy[i][j];
				expandFrom(i + 1, j, grid);
			}
		}
		if (j + 1<w)
		{
			if (grid[i][j + 1] == '1' && !copy[i][j + 1])
			{
				copy[i][j + 1] = copy[i][j];
				expandFrom(i, j + 1, grid);
			}
		}
	}

	int numIslands(vector<vector<char>>& grid) {
		h = grid.size();
		if (h == 0)
			return 0;
		w = grid[0].size();

		for (int i = 0; i<h; ++i)
		{
			copy.push_back(vector<bool>());
			for (int j = 0; j<w; ++j)
				copy[i].push_back(false);
		}
		int num_islands = 0;
		for (int i = 0; i<h; ++i)
		{
			for (int j = 0; j<w; ++j)
			{
				if (grid[i][j] == '1' && !copy[i][j])
				{
					++num_islands;
					copy[i][j] = true;
					expandFrom(i, j, grid);
				}

			}
		}
		return num_islands;
	}
};
