import sys
from copy import deepcopy
from tqdm import tqdm

def can_move_north(grid, i, j):
    return i > 0 and grid[i-1][j] == '.'

def move_north(grid, i, j):
    char = grid[i][j]
    orig_i = i
    while(can_move_north(grid, i, j)):
        i -= 1
    if i != orig_i:
        grid[orig_i][j] = '.'
        grid[i][j] = char

def tilt(grid):
    list_grid = [list(r) for r in grid]
    for i in range(len(list_grid)):
        for j in range(len(list_grid[0])):
            if(list_grid[i][j] == 'O'):
                move_north(list_grid, i, j)
    return tuple(tuple(r) for r in list_grid)

def rotate_clockwise(grid):
    return tuple(zip(*grid[::-1]))

def cycle(grid):
    # North
    grid = tilt(grid)

    # West
    grid = tilt(rotate_clockwise(grid))
    
    # South
    grid = tilt(rotate_clockwise(grid))

    # East
    grid = tilt(rotate_clockwise(grid))

    # Correct grid
    return rotate_clockwise(grid)

def part1(grid):
    grid = tilt(grid)
    total = 0
    for row_idx, row in enumerate(reversed(list(grid))):
        for char in row:
            if char == 'O':
                total += row_idx + 1

    print("Part 1: " + str(total))

def part2(grid):
    grid_cache = {}
    index_cache = {}
    cycles = 1000000000
    for i in range(cycles):
        if grid in index_cache:
            break
        index_cache[grid] = i
        grid_cache[i] = grid
        grid = cycle(grid)

    cycle_length = i - index_cache[grid]
    remaining = (cycles - i) % cycle_length
    target_idx = index_cache[grid] + remaining
    final_grid = grid_cache[target_idx]

    total = 0
    for row_idx, row in enumerate(reversed(list(final_grid))):
        for char in row:
            if char == 'O':
                total += row_idx + 1

    print("Part 2: " + str(total))

def main():
    grid = []
    for line in sys.stdin:
        row = []
        for c in line.rstrip():
            row.append(c)
        grid.append(tuple(row))

    part1(deepcopy(tuple(grid)))
    part2(deepcopy(tuple(grid)))

if __name__ == "__main__":
    main()