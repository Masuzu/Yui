#include <string>
#include <unordered_set>
#include <vector>
#include <queue>

// Hash function for strings
namespace std {
	template <>
	struct hash<std::string>
	{
		typedef std::string argument_type;
		typedef std::size_t result_type;

		result_type operator()(const std::string &s) const
		{
			result_type hash = 0;
			for (size_t i = 0, e = s.size(); i < e; ++i)
				hash = ((hash << 5) + hash) + s[i];
			return hash;
		}
	};
};

// Given a source word and a target word, find the shortest sequence of transformations from source to target where each transformation
// consists in changing a character. The intermediate words must be in dictionary. Example :
// std::unordered_set<std::string> dictionary;
// dictionary.insert("hot");
// dictionary.insert("dot");
// dictionary.insert("dog");
// dictionary.insert("lot");
// dictionary.insert("log");
// WordLadder("hit", "cog", dictionary) will return {"hit", "hot", "dot", "dog", "cog"}
std::vector<std::string> WordLadder(const std::string &source, const std::string &target, std::unordered_set<std::string> &dictionary)
{
	std::queue<std::pair<std::string, std::vector<std::string>>> queue;
	queue.push({ source, std::vector<std::string>() });
	while (!queue.empty())
	{
		auto word_node = queue.front();
		for (size_t i = 0, e = word_node.first.size(); i < e; ++i)
		{
			for (char c = 'a'; c <= 'z'; ++c)
			{
				std::string new_word = word_node.first;
				new_word[i] = c;
				if (new_word == target)
					return word_node.second;
				if (dictionary.find(new_word) != dictionary.end())
				{
					word_node.second.push_back(new_word);
					queue.push({ new_word, word_node.second });
					dictionary.erase(new_word);
				}
			}
		}
		queue.pop();
	}
	return std::vector<std::string>();
}
