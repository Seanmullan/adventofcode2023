
#include <iostream>
#include <map>
#include <optional>
#include <vector>

using Schematic = std::vector<std::vector<int>>;
const auto DOT = -1;
const auto SYMBOL = -2;
const auto GEAR = -3;

struct RowNumber
{
    int number;
    int min_col;
    int max_col;
};
using RowNumbers = std::vector<RowNumber>;
using Coord = std::tuple<int, int>;

bool hasAdjacentSymbolOrGear(const Schematic& schematic, const int row, const int min_col, const int max_col)
{
    const auto row_start = row == 0 ? 0 : row - 1;
    const auto row_end = row == schematic.size() - 1 ? row : row + 1;
    const auto col_start = min_col == 0 ? 0 : min_col - 1;
    const auto col_end = max_col == schematic[row].size() - 1 ? max_col : max_col + 1;

    for (auto i = row_start; i <= row_end; i++)
    {
        for (auto j = col_start; j <= col_end; j++)
        {
            if (schematic[i][j] == SYMBOL || schematic[i][j] == GEAR)
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<Coord> findAdjacentGears(const Schematic& schematic, const int row, const int min_col, const int max_col)
{
    const auto row_start = row == 0 ? 0 : row - 1;
    const auto row_end = row == schematic.size() - 1 ? row : row + 1;
    const auto col_start = min_col == 0 ? 0 : min_col - 1;
    const auto col_end = max_col == schematic[row].size() - 1 ? max_col : max_col + 1;

    std::vector<Coord> result;
    for (auto i = row_start; i <= row_end; i++)
    {
        for (auto j = col_start; j <= col_end; j++)
        {
            if (schematic[i][j] == GEAR)
            {
                result.push_back(std::make_tuple(i, j));
            }
        }
    }
    return result;
}

RowNumbers findRowNumbers(const std::vector<int> row)
{
    RowNumbers row_numbers;
    int col = 0;
    while (col < row.size())
    {
        if (row[col] >= 0)
        {
            const auto min_col = col;
            std::string str_num = "";
            str_num.push_back((char)row[col]);
            col++;
            while (row[col] >= 0)
            {
                str_num.push_back((char)row[col]);
                col++;
            }
            const RowNumber row_number = { std::stoi(str_num), min_col, col - 1 };
            row_numbers.push_back(row_number);
            continue;
        }
        col++;
    }
    return row_numbers;
}

void part1(const Schematic& schematic)
{
    int sum = 0;
    for (auto row = 0; row < schematic.size(); row++)
    {
        const auto row_numbers = findRowNumbers(schematic[row]);
        for (const auto row_number : row_numbers)
        {
            if (hasAdjacentSymbolOrGear(schematic, row, row_number.min_col, row_number.max_col))
            {
                sum += row_number.number;
            }
        }
    }
    std::cout << "Part 1 sum: " << sum << std::endl;
}

void part2(const Schematic& schematic)
{
    std::map<Coord, std::vector<int>> gear_numbers;
    for (auto row = 0; row < schematic.size(); row++)
    {
        const auto row_numbers = findRowNumbers(schematic[row]);
        for (const auto row_number : row_numbers)
        {
            const auto adjacent_gears = findAdjacentGears(schematic, row, row_number.min_col, row_number.max_col);
            for (const auto gear_coord : adjacent_gears)
            {
                gear_numbers[gear_coord].push_back(row_number.number);
            }
        }
    }

    int sum = 0;
    for (const auto& [_, numbers] : gear_numbers)
    {
        if (numbers.size() == 2)
        {
            sum += numbers[0] * numbers[1];
        }
    }
    std::cout << "Part 2 sum: " << sum << std::endl;
}

int main()
{
    Schematic schematic;
    for (std::string line; std::getline(std::cin, line);)
    {
        std::vector<int> row;
        for (const auto& c : line)
        {
            if (std::isdigit(c))
            {
                row.push_back((int)c);
            }
            else if (c == '.')
            {
                row.push_back(DOT);
            }
            else if (c == '*')
            {
                row.push_back(GEAR);
            }
            else
            {
                row.push_back(SYMBOL);
            }
        }
        schematic.push_back(row);
    }

    part1(schematic);
    part2(schematic);

    return 0;
}