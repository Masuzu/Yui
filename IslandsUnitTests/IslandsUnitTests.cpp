#include <Islands.h>

#include <gtest/gtest.h>

static const char kEmptyCellSymbol = '0';
static const char kNonEmptyCellSymbol = '1';

TEST(UnidimensionalSingleIsland, Islands)
{
	const int kIslandSize = 5;
	Yui::Islands<char> islands(kEmptyCellSymbol);

	std::vector<std::vector<char>> map;
	map.push_back(std::vector<char>());
	for (int i = 0; i < kIslandSize; ++i)
		map[0].push_back(kNonEmptyCellSymbol);

	EXPECT_EQ(1u, islands.NumIslands(map));
}

TEST(UnidimensionalDisjointIslands, Islands)
{
	const int kNumIslands = 3;
	const int kIslandSize = 5;
	Yui::Islands<char> islands(kEmptyCellSymbol);

	std::vector<std::vector<char>> map;
	map.push_back(std::vector<char>());
	for (int island = 0; island < kNumIslands; ++island)
	{
		map[0].push_back(kEmptyCellSymbol);
		for (int i = 0; i < kIslandSize; ++i)
			map[0].push_back(kNonEmptyCellSymbol);
	}

	EXPECT_EQ(kNumIslands, islands.NumIslands(map));
}

TEST(TwoDimensionalSingleIsland, Islands)
{
	Yui::Islands<char> islands(kEmptyCellSymbol);

	std::vector<std::vector<char>> map;
	map.push_back({ '1', '0', '0', '0', '0' });
	map.push_back({ '1', '1', '1', '1', '0' });
	map.push_back({ '0', '1', '0', '1', '0' });
	map.push_back({ '0', '0', '1', '1', '0' });
	map.push_back({ '0', '1', '1', '1', '0' });

	EXPECT_EQ(1u, islands.NumIslands(map));
}

TEST(TwoDimensionalDisjointIslands, Islands)
{
	Yui::Islands<char> islands(kEmptyCellSymbol);

	std::vector<std::vector<char>> map;
	map.push_back({ '1', '0', '0', '0', '1' });
	map.push_back({ '1', '1', '1', '1', '0' });
	map.push_back({ '0', '1', '0', '0', '1' });
	map.push_back({ '0', '0', '1', '0', '1' });
	map.push_back({ '0', '1', '1', '0', '0' });

	EXPECT_EQ(4u, islands.NumIslands(map));
}