#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <stack>
#include <vector>

enum Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};
struct Pipe
{
    char value;
    std::vector<Direction> valid_directions;
};
struct Coord
{
    size_t row;
    size_t col;

    bool operator==(const Coord& rhs) const
    {
        return row == rhs.row && col == rhs.col;
    }

    bool operator!=(const Coord& rhs) const
    {
        return row != rhs.row || col != rhs.col;
    }
};
using Grid = std::vector<std::vector<Pipe>>;
using Coords = std::vector<Coord>;

bool operator<(const Coord& lhs, const Coord& rhs)
{
    return (lhs.row < rhs.row || (lhs.row == rhs.row && lhs.col < rhs.col));
}

bool withinBounds(const Grid& grid, const Coord& coord)
{
    return coord.row >= 0 && coord.col >= 0 && coord.row < grid.size() && coord.col < grid.front().size();
}

bool isValidConnection(const Grid& grid, const Coord& from_coord, const Coord& to_coord, const Direction from_direction,
                       const Direction to_direction)
{
    if (!withinBounds(grid, to_coord))
    {
        return false;
    }

    // Check that from pipe allows travelling in desired direction
    const auto& from_directions = grid[from_coord.row][from_coord.col].valid_directions;
    if (std::find(from_directions.begin(), from_directions.end(), to_direction) == from_directions.end())
    {
        return false;
    }

    // Check that to pipe accepts travelling from this direction
    const auto& to_directions = grid[to_coord.row][to_coord.col].valid_directions;
    if (std::find(to_directions.begin(), to_directions.end(), from_direction) == to_directions.end())
    {
        return false;
    }

    return true;
}

Coords getNeighbours(const Grid& grid, const Coord& coord)
{
    Coords neighbours;

    // Head from south to north
    Coord to_coord = { coord.row - 1, coord.col };
    if (isValidConnection(grid, coord, to_coord, Direction::SOUTH, Direction::NORTH))
    {
        neighbours.push_back(to_coord);
    }

    // Head from north to south
    to_coord = { coord.row + 1, coord.col };
    if (isValidConnection(grid, coord, to_coord, Direction::NORTH, Direction::SOUTH))
    {
        neighbours.push_back(to_coord);
    }

    // Head from west to east
    to_coord = { coord.row, coord.col + 1 };
    if (isValidConnection(grid, coord, to_coord, Direction::WEST, Direction::EAST))
    {
        neighbours.push_back(to_coord);
    }

    // Head from east to west
    to_coord = { coord.row, coord.col - 1 };
    if (isValidConnection(grid, coord, to_coord, Direction::EAST, Direction::WEST))
    {
        neighbours.push_back(to_coord);
    }

    return neighbours;
}

Coords dfs(const Grid& grid, const Coord& start)
{
    std::vector<bool> visited_row(grid.front().size(), false);
    std::vector<std::vector<bool>> visited(grid.size(), visited_row);
    std::stack<Coord> stack;
    std::vector<Coord> traversal;
    std::map<Coord, std::optional<Coord>> parent_map;
    parent_map[start] = std::nullopt;

    visited[start.row][start.col] = true;
    stack.push(start);

    while (!stack.empty())
    {
        const auto current = stack.top();
        stack.pop();
        traversal.push_back(current);

        for (const auto neighbour : getNeighbours(grid, current))
        {
            for (const auto neighbour_neighbour : getNeighbours(grid, neighbour))
            {
                if (neighbour_neighbour == start)
                {
                    traversal.clear();
                    traversal.push_back(neighbour);
                    Coord curr = current;
                    while (parent_map[curr] != std::nullopt)
                    {
                        if (traversal.size() <= 3 &&
                            std::find(traversal.begin(), traversal.end(), curr) != traversal.end())
                        {
                            break;
                        }
                        traversal.push_back(curr);
                        curr = *parent_map[curr];
                    }

                    if (traversal.size() > 2)
                    {
                        traversal.push_back(start);
                        std::reverse(traversal.begin(), traversal.end());
                        return traversal;
                    }
                }
            }

            if (!visited[neighbour.row][neighbour.col])
            {
                visited[neighbour.row][neighbour.col] = true;
                stack.push(neighbour);
                parent_map[neighbour] = current;
            }
        }
    }

    std::cout << "No cycle found!" << std::endl;
    exit(1);
}

bool isPointWithinPolygon(const Coords& polygon, const Coord& point)
{
    int i = 0;
    int j = 0;
    bool within = false;

    for (i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
    {
        const auto poly_x_i = polygon[i].col;
        const auto poly_y_i = polygon[i].row;
        const auto poly_x_j = polygon[j].col;
        const auto poly_y_j = polygon[j].row;
        const auto test_x = point.col;
        const auto test_y = point.row;

        if (((poly_y_i > test_y) != (poly_y_j > test_y)) &&
            (test_x < (poly_x_j - poly_x_i) * (test_y - poly_y_i) / (poly_y_j - poly_y_i) + poly_x_i))
        {
            within = !within;
        }
    }

    return within;
}

void part1(const Grid& grid, const Coord& start)
{
    const auto path = dfs(grid, start);
    std::cout << "Part 1: " << std::floor(path.size() / 2) << std::endl;
}

void part2(const Grid& grid, const Coord& start)
{
    const auto path = dfs(grid, start);
    size_t num_within = 0;

    for (size_t row = 0; row < grid.size(); row++)
    {
        for (size_t col = 0; col < grid.front().size(); col++)
        {
            const Coord coord = { row, col };
            if (std::find(path.begin(), path.end(), coord) != path.end())
            {
                continue;
            }
            if (isPointWithinPolygon(path, coord))
            {
                num_within++;
            }
        }
    }

    std::cout << "Part 2: " << num_within << std::endl;
}

int main()
{
    Grid grid;
    Coord starting_coord;
    size_t curr_row = 0;
    for (std::string line; std::getline(std::cin, line);)
    {
        std::vector<Pipe> row;
        for (size_t col = 0; col < line.size(); col++)
        {
            Pipe pipe;
            pipe.value = line[col];

            switch (pipe.value)
            {
                case 'S':
                    pipe.valid_directions = { Direction::NORTH, Direction::SOUTH, Direction::EAST, Direction::WEST };
                    break;
                case '|':
                    pipe.valid_directions = { Direction::NORTH, Direction::SOUTH };
                    break;
                case '-':
                    pipe.valid_directions = { Direction::EAST, Direction::WEST };
                    break;
                case 'L':
                    pipe.valid_directions = { Direction::NORTH, Direction::EAST };
                    break;
                case 'J':
                    pipe.valid_directions = { Direction::NORTH, Direction::WEST };
                    break;
                case '7':
                    pipe.valid_directions = { Direction::SOUTH, Direction::WEST };
                    break;
                case 'F':
                    pipe.valid_directions = { Direction::SOUTH, Direction::EAST };
                    break;
                case '.':
                    pipe.valid_directions = {};
                    break;
                default:
                    std::cout << "Unhandled char: " << pipe.value << std::endl;
                    exit(1);
            }

            row.push_back(pipe);
            if (line[col] == 'S')
            {
                starting_coord = { curr_row, col };
            }
        }
        grid.push_back(row);
        curr_row++;
    }
    part1(grid, starting_coord);
    part2(grid, starting_coord);
}