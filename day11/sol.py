import sys
import itertools
from collections import deque

def get_neighbours(grid, node):
    neighbours = []
    row, col = node

    # Check the top neighbour
    if row > 0:
        neighbours.append((row - 1, col))

    # Check the bottom neighbour
    if row < len(grid) - 1:
        neighbours.append((row + 1, col))

    # Check the left neighbour
    if col > 0:
        neighbours.append((row, col - 1))

    # Check the right neighbour
    if col < len(grid[0]) - 1:
        neighbours.append((row, col + 1))

    return neighbours

def get_path(prev, start, end):
    path = []
    node = end

    while node != start:
        path.append(node)
        node = prev[node]

    path.append(start)
    path.reverse()
    return path

def get_shortest_path(grid, start, end):
    queue = deque()
    visited = set()
    distance = {start: 0}
    prev = {}

    queue.append(start)
    visited.add(start)

    while queue:
        node = queue.popleft()

        # Explore the neighbouring nodes
        for neighbour in get_neighbours(grid, node):
            if neighbour not in visited:
                visited.add(neighbour)
                distance[neighbour] = distance[node] + 1
                prev[neighbour] = node
                queue.append(neighbour)

            if neighbour == end:
                return get_path(prev, start, end)

    return None

def part1(grid):

    galaxy_coords = []
    for row in range(len(grid)):
        for col in range(len(grid[0])):
            if grid[row][col] == '#':
                galaxy_coords.append((row, col))

    total = 0
    for pair in itertools.combinations(galaxy_coords, 2):
        path = get_shortest_path(grid, pair[0], pair[1])
        total += len(path) - 1
    print("Part 1: " + str(total))


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
        grid.append(row)
        if row == ['.'] * len(row):
            grid.append(row)

    new_grid = grid
    offset = 0
    for idx in range(len(grid[0])):
        col = [row[idx] for row in grid]
        if col == ['.'] * len(col):
            new_grid = add_col(new_grid, idx + offset)
            offset += 1

    part1(grid)


if __name__ == "__main__":
    main()