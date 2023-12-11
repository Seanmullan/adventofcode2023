import sys
import itertools
from collections import deque

part_1 = False

def manhattan(a, b):
    return sum(abs(val1-val2) for val1, val2 in zip(a,b))

def part1(grid):
    galaxy_coords = []
    for row in range(len(grid)):
        for col in range(len(grid[0])):
            if grid[row][col] == '#':
                galaxy_coords.append((row, col))

    total = 0
    for pair in itertools.combinations(galaxy_coords, 2):
        path_len = manhattan(pair[0], pair[1])
        total += path_len

    print("Part 1: " + str(total))


def part2(grid):
    galaxy_coords = []
    for row in range(len(grid)):
        for col in range(len(grid[0])):
            if grid[row][col] == '#':
                galaxy_coords.append((row, col))

    total = 0
    for pair in itertools.combinations(galaxy_coords, 2):
        path_len = manhattan(pair[0], pair[1])

        # Find all empty rows between galaxies
        rows = sorted((pair[0][0], pair[1][0]))
        for row_idx in range(rows[0] + 1, rows[1]):
            if grid[row_idx][0] == '0':
                path_len += 999999

        # Find all empty cols between galaxies
        cols = sorted((pair[0][1], pair[1][1]))
        for col_idx in range(cols[0] + 1, cols[1]):
            if grid[0][col_idx] == '0':
                path_len += 999999

        total += path_len

    print("Part 2: " + str(total))


def add_col(grid, index):
    new_grid = []
    l = len(grid[0])
    for row in grid:
        new_row = row[:index] + ['.'] + row[index:]
        new_grid.append(new_row)
    return new_grid

def main():
    grid = []
    for line in sys.stdin:
        row = list(line.split()[0])
        if part_1:
            grid.append(row)
            if row == ['.'] * len(row):
                grid.append(row)
        else:
            if row == ['.'] * len(row):
                grid.append(['0'] * len(row))
            else:
                grid.append(row)

    new_grid = grid
    offset = 0
    for idx in range(len(grid[0])):
        empty = [row[idx] == '.' or row[idx] == '0' for row in grid]
        if all(empty):
            if part_1:
                new_grid = add_col(new_grid, idx + offset)
                offset += 1
            else:
                for row in new_grid:
                    row[idx] = '0'

    if part_1:
        part1(new_grid)
    else:
        part2(new_grid)


if __name__ == "__main__":
    main()