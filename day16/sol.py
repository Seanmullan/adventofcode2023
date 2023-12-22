import sys
from enum import Enum
from copy import deepcopy

class Direction(Enum):
    UP = 0
    DOWN = 1
    LEFT = 2
    RIGHT = 3

def print_grid(grid):
    for r in grid:
        print(" ".join(r))
    print("")

def panic():
    print("Panic!")
    exit(1)

def go_up(grid, curr):
    i, j, _ = curr
    return [(i - 1, j, Direction.UP)] if i > 0 else []

def go_down(grid, curr):
    i, j, _ = curr
    return [(i + 1, j, Direction.DOWN)] if i < len(grid) - 1 else []

def go_left(grid, curr):
    i, j, _ = curr
    return [(i, j - 1, Direction.LEFT)] if j > 0 else []

def go_right(grid, curr):
    i, j, _ = curr
    return [(i, j + 1, Direction.RIGHT)] if j < len(grid[0]) - 1 else []

def get_neighbours(grid, curr):
    i, j, direction = curr
    char = grid[i][j]
    if direction == Direction.UP:
        if char == '.' or char == '|':
            return go_up(grid, curr)
        elif char == '/':
            return go_right(grid, curr)
        elif char == '\\':
            return go_left(grid, curr)
        elif char == '-':
            return go_left(grid, curr) + go_right(grid, curr)
        else:
            panic()

    elif direction == Direction.DOWN:
        if char == '.' or char == '|':
            return go_down(grid, curr)
        elif char == '/':
            return go_left(grid, curr)
        elif char == '\\':
            return go_right(grid, curr)
        elif char == '-':
            return go_left(grid, curr) + go_right(grid, curr)
        else:
            panic()

    elif direction == Direction.LEFT:
        if char == '.' or char == '-':
            return go_left(grid, curr)
        elif char == '/':
            return go_down(grid, curr)
        elif char == '\\':
            return go_up(grid, curr)
        elif char == '|':
            return go_up(grid, curr) + go_down(grid, curr)
        else:
            panic()

    else:
        if char == '.' or char == '-':
            return go_right(grid, curr)
        elif char == '/':
            return go_up(grid, curr)
        elif char == '\\':
            return go_down(grid, curr)
        elif char == '|':
            return go_up(grid, curr) + go_down(grid, curr)
        else:
            panic()

    return []

def bfs(grid, start):
    visited = set()
    queue = []
    queue.append(start)

    while queue:
        curr = queue.pop(0)
        for neighbour in get_neighbours(grid, curr):
            if neighbour not in visited:
                queue.append(neighbour)
                visited.add(neighbour)

    out_grid = deepcopy(grid)
    out_grid[start[0]][start[1]] = '#'
    for i, j, _ in visited:
        out_grid[i][j] = '#'
    return out_grid

def part1(grid):
    energised_grid = bfs(grid, (0, 0, Direction.RIGHT))
    print_grid(energised_grid)
    total = sum(["".join(r).count("#") for r in energised_grid])
    print("Part 1: " + str(total))

def part2(grid):
    top = [(0, j, Direction.DOWN) for j in range(1, len(grid[0]) - 2)]
    bottom = [(len(grid) - 1, j, Direction.UP) for j in range(1, len(grid[0]) - 2)]
    left = [(i, 0, Direction.RIGHT) for i in range(1, len(grid) - 2)]
    right = [(i, len(grid) - 1, Direction.LEFT) for i in range(1, len(grid) - 2)]
    corners = [(0, 0, Direction.DOWN), (0, 0, Direction.RIGHT),
               (0, len(grid[0]) - 1, Direction.DOWN), (0, len(grid[0]) - 1, Direction.LEFT),
               (len(grid) - 1, 0, Direction.UP), (len(grid) - 1, 0, Direction.RIGHT),
               (len(grid) - 1, len(grid[0]) - 1, Direction.UP), (len(grid) - 1, len(grid[0]) - 1, Direction.LEFT)]
    search = corners + top + bottom + left + right

    max_energised = 0
    max_energised_grid = []
    for start in search:
        energised_grid = bfs(grid, start)
        total = sum(["".join(r).count("#") for r in energised_grid])
        if total > max_energised:
            max_energised = total
            max_energised_grid = energised_grid

    print_grid(max_energised_grid)
    print("Part 2: " + str(max_energised))

def main():
    grid = []
    for line in sys.stdin:
        grid.append([c for c in line.rstrip()])
    part1(grid)
    print("")
    part2(grid)

if __name__ == "__main__":
    main()