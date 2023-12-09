#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <vector>

using IntType = int;  // long long int;
using History = std::vector<IntType>;
using Histories = std::vector<History>;
using Tree = std::vector<std::vector<IntType>>;

std::vector<std::string> splitString(const std::string& str, const char separator)
{
    std::vector<std::string> result;
    std::size_t start_idx = 0, end_idx = 0;
    for (std::size_t i = 0; i <= str.size(); i++)
    {
        if (str[i] == separator || i == str.size())
        {
            // Handle case when start of string is separator
            if (str[i] == separator && i == 0)
            {
                start_idx = i + 1;
                continue;
            }
            end_idx = i;
            std::string temp;
            temp.append(str, start_idx, end_idx - start_idx);
            if (!temp.empty())
            {
                result.push_back(temp);
            }
            start_idx = end_idx + 1;
        }
    }
    return result;
}

void strip(std::string& str, const char c)
{
    str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

Tree computeTree(const History& history)
{
    Tree tree = { history };
    while (tree.back().back() != 0)
    {
        std::vector<IntType> diffs;
        diffs.resize(tree.back().size());
        std::adjacent_difference(tree.back().begin(), tree.back().end(), diffs.begin());
        std::vector<IntType>(std::next(diffs.begin()), diffs.end()).swap(diffs);
        tree.push_back(diffs);
    }
    return tree;
}

IntType predict_forward(const History& history)
{
    Tree tree = computeTree(history);
    tree.back().push_back(0);
    for (int i = tree.size() - 2; i >= 0; i--)
    {
        const auto new_num = tree[i + 1].back() + tree[i].back();
        tree[i].push_back(new_num);
    }
    return tree.front().back();
}

IntType predict_backward(const History& history)
{
    Tree tree = computeTree(history);
    tree.back().push_back(0);
    for (int i = tree.size() - 2; i >= 0; i--)
    {
        const auto new_num = tree[i].front() - tree[i + 1].back();
        tree[i].push_back(new_num);
    }
    return tree.front().back();
}

void part1(const Histories& histories)
{
    const auto res = std::transform_reduce(histories.begin(), histories.end(), 0, std::plus{}, predict_forward);
    std::cout << "Part 1 : " << res << std::endl;
}

void part2(const Histories& histories)
{
    const auto res = std::transform_reduce(histories.begin(), histories.end(), 0, std::plus{}, predict_backward);
    std::cout << "Part 2 : " << res << std::endl;
}

int main()
{
    Histories histories;
    for (std::string line; std::getline(std::cin, line);)
    {
        History history;
        for (const auto& num : splitString(line, ' '))
        {
            history.push_back(std::stoll(num));
        }
        histories.push_back(history);
    }

    part1(histories);
    part2(histories);
}