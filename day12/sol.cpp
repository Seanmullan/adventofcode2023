#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <optional>
#include <unordered_map>
#include <vector>

using IntType = long long int;
enum Status
{
    OPERATIONAL,
    BROKEN,
    UNKNOWN,
};
struct SpringRow
{
    std::vector<Status> springs;
    std::vector<int> groups;

    bool operator==(const SpringRow& o) const
    {
        if (springs.size() != o.springs.size() || groups.size() != o.groups.size())
        {
            return false;
        }
        for (auto i = 0; i < springs.size(); i++)
        {
            if (springs[i] != o.springs[i])
            {
                return false;
            }
        }
        for (auto i = 0; i < groups.size(); i++)
        {
            if (groups[i] != o.groups[i])
            {
                return false;
            }
        }
        return true;
    }
};
using SpringRows = std::vector<SpringRow>;
struct SpringRowCacheKey
{
    SpringRow spring_row;
    int curr_group_size;

    bool operator==(const SpringRowCacheKey& o) const
    {
        return spring_row == o.spring_row && curr_group_size == o.curr_group_size;
    }
};

template <>
struct std::hash<SpringRowCacheKey>
{
    std::size_t operator()(const SpringRowCacheKey& s) const
    {
        std::string spring_str(s.spring_row.springs.begin(), s.spring_row.springs.end());
        std::string groups_str(s.spring_row.groups.begin(), s.spring_row.groups.end());
        std::string count_str = std::to_string(s.curr_group_size);
        std::hash<std::string> hasher;
        return hasher(spring_str + groups_str + count_str);
    }
};
using Cache = std::unordered_map<SpringRowCacheKey, IntType>;

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

IntType findNumValidPermutations(const SpringRow& spring_row, Cache& cache, int curr_group_size)
{
    SpringRowCacheKey key = { spring_row, curr_group_size };
    if (cache.contains(key))
    {
        return cache[key];
    }

    if (spring_row.springs.empty())
    {
        return spring_row.groups.empty() && curr_group_size == 0;
    }

    IntType total = 0;

    const auto curr_spring = spring_row.springs.front();
    const std::vector<Status> next_springs(std::next(spring_row.springs.begin()), spring_row.springs.end());

    std::vector<Status> to_search;
    if (curr_spring == Status::UNKNOWN)
    {
        to_search.push_back(Status::BROKEN);
        to_search.push_back(Status::OPERATIONAL);
    }
    else
    {
        to_search.push_back(curr_spring);
    }

    for (const auto curr_status : to_search)
    {
        if (curr_status == Status::BROKEN)
        {
            // If group is still valid
            if (!spring_row.groups.empty() && curr_group_size < spring_row.groups.front())
            {
                SpringRow new_row = { next_springs, spring_row.groups };
                total += findNumValidPermutations(new_row, cache, curr_group_size + 1);
            }
        }
        // If curr_status == OPERATIONAL and still in damaged group
        else if (curr_group_size > 0)
        {
            // and the damaged group matches the current group in the provided patterns
            if (!spring_row.groups.empty() && curr_group_size == spring_row.groups.front())
            {
                const std::vector<int> next_groups(std::next(spring_row.groups.begin()), spring_row.groups.end());
                SpringRow new_row = { next_springs, next_groups };
                total += findNumValidPermutations(new_row, cache, 0);
            }
        }
        else
        {
            SpringRow new_row = { next_springs, spring_row.groups };
            total += findNumValidPermutations(new_row, cache, 0);
        }
    }

    cache[key] = total;
    return total;
}

void part1(const SpringRows& spring_rows)
{
    Cache cache;
    IntType total = 0;
    for (auto spring_row : spring_rows)
    {
        spring_row.springs.push_back(Status::OPERATIONAL);
        total += findNumValidPermutations(spring_row, cache, 0);
    }
    std::cout << "Part 1: " << total << std::endl;
}

void part2(const SpringRows& spring_rows)
{
    Cache cache;
    IntType total = 0;
    for (auto spring_row : spring_rows)
    {
        spring_row.springs.push_back(Status::OPERATIONAL);
        total += findNumValidPermutations(spring_row, cache, 0);
    }
    std::cout << "Part 2: " << total << std::endl;
}

int main()
{
    SpringRows spring_rows;
    for (std::string line; std::getline(std::cin, line);)
    {
        const auto str = splitString(line, ' ');
        SpringRow spring_row;
        for (const auto c : str[0])
        {
            switch (c)
            {
                case '.':
                    spring_row.springs.push_back(Status::OPERATIONAL);
                    break;
                case '#':
                    spring_row.springs.push_back(Status::BROKEN);
                    break;
                case '?':
                    spring_row.springs.push_back(Status::UNKNOWN);
                    break;
                default:
                    std::cout << "Unknown char: " << c << std::endl;
                    exit(1);
            }
        }
        for (const auto c : splitString(str[1], ','))
        {
            spring_row.groups.push_back(std::stoi(c));
        }
        spring_rows.push_back(spring_row);
    }

    part1(spring_rows);

    SpringRows spring_rows_2;
    for (const auto& spring_row : spring_rows)
    {
        SpringRow spring_row_2;
        for (int i = 0; i < 5; i++)
        {
            spring_row_2.springs.insert(spring_row_2.springs.end(), spring_row.springs.begin(),
                                        spring_row.springs.end());
            if (i != 4)
            {
                spring_row_2.springs.push_back(Status::UNKNOWN);
            }
            spring_row_2.groups.insert(spring_row_2.groups.end(), spring_row.groups.begin(), spring_row.groups.end());
        }
        spring_rows_2.push_back(spring_row_2);
    }

    part2(spring_rows_2);
}