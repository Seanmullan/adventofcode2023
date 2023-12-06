
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

using IntType = long long int;

struct Range
{
    IntType src_start;
    IntType src_end;
    IntType dst_start;
};

using Map = std::vector<Range>;
using Maps = std::vector<Map>;

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

IntType lookup(const Map& map, const IntType src)
{
    for (const auto& range : map)
    {
        if (src >= range.src_start && src <= range.src_end)
        {
            return range.dst_start + (src - range.src_start);
        }
    }
    return src;
}

void part1(const Maps& maps, const std::vector<IntType>& initial_seeds)
{
    IntType closest_seed = std::numeric_limits<IntType>::max();
    for (const auto& init_seed : initial_seeds)
    {
        auto src = init_seed;
        for (const auto& map : maps)
        {
            src = lookup(map, src);
        }
        if (src < closest_seed)
        {
            closest_seed = src;
        }
    }
    std::cout << "Part 1: " << closest_seed << std::endl;
}

void part2(const Maps& maps, const Map& init_ranges)
{
    IntType closest_seed = std::numeric_limits<IntType>::max();
    for (const auto& init_range : init_ranges)
    {
        for (auto init_seed = init_range.src_start; init_seed <= init_range.src_end; init_seed++)
        {
            auto src = init_seed;
            for (const auto& map : maps)
            {
                src = lookup(map, src);
            }
            if (src < closest_seed)
            {
                closest_seed = src;
            }
        }
    }
    std::cout << "Part 2: " << closest_seed << std::endl;
}

int main()
{
    std::vector<IntType> initial_seeds;

    std::string line;
    std::getline(std::cin, line);
    for (const auto& num : splitString(splitString(line, ':')[1], ' '))
    {
        initial_seeds.push_back(std::stoll(num));
    }
    std::getline(std::cin, line);

    Maps maps;
    while (std::getline(std::cin, line))
    {
        if (line.empty())
        {
            continue;
        }
        if (line.find("map") != std::string::npos)
        {
            maps.push_back({});
            continue;
        }

        const auto nums = splitString(line, ' ');
        const auto dst_start = std::stoll(nums[0]);
        const auto src_start = std::stoll(nums[1]);
        const auto src_end = src_start + std::stoll(nums[2]) - 1;
        maps.back().push_back(Range({ src_start, src_end, dst_start }));
    }

    part1(maps, initial_seeds);

    Map init_ranges;
    for (auto i = 0; i < initial_seeds.size(); i += 2)
    {
        const auto start = initial_seeds[i];
        const auto end = start + initial_seeds[i + 1] - 1;
        init_ranges.push_back(Range({ start, end, 0 }));
    }

    part2(maps, init_ranges);

    return 0;
}