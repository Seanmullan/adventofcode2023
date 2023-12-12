#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <vector>

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
};
using SpringRows = std::vector<SpringRow>;
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
    if (curr_group_size != 0)
    {
        group_sizes.push_back(curr_group_size);
    }

    if (spring_row.groups.size() != group_sizes.size())
    {
        return false;
    }

    for (size_t i = 0; i < group_sizes.size(); i++)
    {
        if (spring_row.groups[i] != group_sizes[i])
        {
            return false;
        }
    }

    return true;
}

IntType findNumValidPermutations(SpringRow spring_row)
{
    const auto unknown_it = std::find(spring_row.springs.begin(), spring_row.springs.end(), Status::UNKNOWN);
    if (unknown_it == spring_row.springs.end())
    {
        return isValidUpToIndex(spring_row, spring_row.springs.size()) ? 1 : 0;
    }

    const auto unknown_idx = std::distance(spring_row.springs.begin(), unknown_it);

    spring_row.springs[unknown_idx] = Status::OPERATIONAL;
    const auto spring_row_operational = spring_row;
    spring_row.springs[unknown_idx] = Status::BROKEN;
    const auto spring_row_broken = spring_row;

    return findNumValidPermutations(spring_row_operational) + findNumValidPermutations(spring_row_broken);
}

void part1(const SpringRows& spring_rows)
{
    IntType total = 0;
    for (const auto spring_row : spring_rows)
    {
        total += findNumValidPermutations(spring_row);
    }
    std::cout << "Part 1: " << total << std::endl;
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
}