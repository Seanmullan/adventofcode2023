#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <vector>

using Id = std::string;
using Ids = std::vector<std::string>;
struct Node
{
    Id id;
    Id left;
    Id right;
};
using Graph = std::map<Id, Node>;
using IntType = long long int;

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

IntType part1(const Graph& graph, const std::string& instructions, Id curr_id = "AAA", IntType steps = 0,
              bool is_part1 = true)
{
    for (const auto c : instructions)
    {
        const auto& node = graph.find(curr_id)->second;
        curr_id = c == 'L' ? node.left : node.right;
        steps++;
    }

    if ((is_part1 && curr_id == "ZZZ") || (!is_part1 && curr_id.back() == 'Z'))
    {
        if (is_part1)
        {
            std::cout << "Part 1: " << steps << std::endl;
        }
        return steps;
    }
    else
    {
        return part1(graph, instructions, curr_id, steps, is_part1);
    }
}

void part2(const Graph& graph, const std::string& instructions, const Ids& start_ids)
{
    std::vector<IntType> all_steps;
    for (const auto& id : start_ids)
    {
        const auto steps = part1(graph, instructions, id, 0, false);
        all_steps.push_back(steps);
    }

    const auto res = std::accumulate(all_steps.begin(), all_steps.end(), all_steps.front(), std::lcm<IntType, IntType>);

    std::cout << "Part 2: " << res << std::endl;
}

int main()
{
    std::string line;
    std::getline(std::cin, line);
    const auto instructions = line;
    std::getline(std::cin, line);

    Graph graph;
    while (std::getline(std::cin, line))
    {
        strip(line, ' ');
        auto str = splitString(line, '=');
        const auto id = str[0];
        strip(str[1], '(');
        strip(str[1], ')');
        const auto nodes = splitString(str[1], ',');

        Node node;
        node.id = id;
        node.left = nodes[0];
        node.right = nodes[1];
        graph[id] = node;
    }

    part1(graph, instructions);

    Ids curr_ids;
    for (const auto& [id, _] : graph)
    {
        if (id.back() == 'A')
        {
            curr_ids.push_back(id);
        }
    }

    part2(graph, instructions, curr_ids);
}