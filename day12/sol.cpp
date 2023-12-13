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

template <>
struct std::hash<SpringRow>
{
    std::size_t operator()(const SpringRow& s) const
    {
        std::string spring_str(s.springs.begin(), s.springs.end());
        std::string groups_str(s.groups.begin(), s.groups.end());
        std::hash<std::string> hasher;
        return hasher(spring_str + groups_str);
    }
};
using Cache = std::unordered_map<SpringRow, IntType>;

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

bool isValidUpToIndex(const SpringRow& spring_row, const size_t idx)
{
    std::vector<int> group_sizes;
    int curr_group_size = 0;
    for (size_t i = 0; i < idx; i++)
    {
        if (spring_row.springs[i] == Status::OPERATIONAL)
        {
            if (curr_group_size != 0)
            {
                group_sizes.push_back(curr_group_size);
                curr_group_size = 0;
            }
        }
        else if (spring_row.springs[i] == Status::BROKEN)
        {
            curr_group_size++;
        }
        else
        {
            std::cout << "Found UNKNOWN in spring row?" << std::endl;
            exit(1);
        }
    }

    bool ended_with_broken = false;
    if (curr_group_size > 0)
    {
        ended_with_broken = true;
        group_sizes.push_back(curr_group_size);
    }

    if (group_sizes.size() != spring_row.groups.size() && idx == spring_row.springs.size())
    {
        return false;
    }

    for (size_t i = 0; i < group_sizes.size(); i++)
    {
        if (ended_with_broken && i == group_sizes.size() - 1)
        {
            if (idx == spring_row.springs.size())
            {
                return group_sizes[i] == spring_row.groups[i];
            }
            return group_sizes[i] <= spring_row.groups[i];
        }
        if (spring_row.groups[i] != group_sizes[i])
        {
            return false;
        }
    }

    return true;
}

IntType findNumValidPermutations(const SpringRow& spring_row, Cache& cache)
{
    if (cache.contains(spring_row))
    {
        return cache[spring_row];
    }

    const auto unknown_it = std::find(spring_row.springs.begin(), spring_row.springs.end(), Status::UNKNOWN);
    if (unknown_it == spring_row.springs.end())
    {
        const auto num = isValidUpToIndex(spring_row, spring_row.springs.size()) ? 1 : 0;
        cache[spring_row] = num;
        return num;
    }

    const auto unknown_idx = std::distance(spring_row.springs.begin(), unknown_it);
    if (!isValidUpToIndex(spring_row, unknown_idx))
    {
        cache[spring_row] = 0;
        return 0;
    }

    auto spring_row_operational = spring_row;
    auto spring_row_broken = spring_row;
    spring_row_operational.springs[unknown_idx] = Status::OPERATIONAL;
    spring_row_broken.springs[unknown_idx] = Status::BROKEN;

    const auto num =
        findNumValidPermutations(spring_row_operational, cache) + findNumValidPermutations(spring_row_broken, cache);
    cache[spring_row] = num;
    return num;
}

void part1(const SpringRows& spring_rows)
{
    Cache cache;
    IntType total = 0;
    for (const auto& spring_row : spring_rows)
    {
        total += findNumValidPermutations(spring_row, cache);
    }
    std::cout << "Part 1: " << total << std::endl;
}

void part2(const SpringRows& spring_rows)
{
    Cache cache;
    IntType total = 0;
    size_t num_rows = spring_rows.size();
    size_t curr_row = 1;
    std::mutex mutex;

    for (const auto& spring_row : spring_rows)
    {
        std::cout << curr_row++ << " / " << num_rows << std::endl;
        total += findNumValidPermutations(spring_row, cache);
    }

    // std::for_each(std::execution::par, spring_rows.begin(), spring_rows.end(), [&](const auto& spring_row) {
    //     const auto num = findNumValidPermutations(spring_row);
    //     mutex.lock();
    //     std::cout << curr_row++ << " / " << num_rows << std::endl;
    //     total += num;
    //     mutex.unlock();
    // });

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