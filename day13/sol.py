import sys
from copy import deepcopy

def differs_by_one(line1, line2):
    num_diff = 0
    diff_idx = None
    for idx, (lhs, rhs) in enumerate(zip(line1, line2)):
        if lhs != rhs:
            num_diff += 1
            diff_idx = idx
    return num_diff == 1, diff_idx

def find_reflection_idx_candidates(lines):
    candidates = []
    for i, (cur, nxt) in enumerate(zip(lines[:-1], lines[1:])):
        if (cur == nxt):
            candidates.append(i + 1)

    return candidates

def is_reflection(lines, idx, is_part_2 = False):
    i = idx - 1
    j = idx
    while(i >= 0 and j < len(lines)):
        if lines[i] != lines[j]:
            return False
        i -= 1
        j += 1
    return True

def part1(grids):
    row_sum = 0
    col_sum = 0
    for grid in grids:
        rows = grid[0]
        cols = grid[1]
        row_refl_idx_candidates = find_reflection_idx_candidates(rows)
        col_refl_idx_candidates = find_reflection_idx_candidates(cols)

        for row_refl_idx in row_refl_idx_candidates:
            if is_reflection(rows, row_refl_idx):
                row_sum += row_refl_idx

        for col_refl_idx in col_refl_idx_candidates:
            if is_reflection(cols, col_refl_idx):
                col_sum += col_refl_idx

    total = col_sum + 100 * row_sum
    print("Part 1: " + str(total))

def find_smudges(lines):
    smudges = []
    i = 0
    while (i < len(lines) - 1):
        j = i + 1
        while(j < len(lines)):
            diff_by_one, idx = differs_by_one(lines[i], lines[j])
            if diff_by_one:
                smudges.append((i, j, idx))
            j += 1
        i += 1

    return smudges

def part2(grids):
    row_sum = 0
    col_sum = 0
    for i, grid in enumerate(grids):
        rows = grid[0]
        cols = grid[1]
        row_smudges = find_smudges(rows)
        col_smudges = find_smudges(cols)
        row_refl_idx_candidates = find_reflection_idx_candidates(rows)
        col_refl_idx_candidates = find_reflection_idx_candidates(cols)

        orig_row_refl_idx = None
        for row_refl_idx in row_refl_idx_candidates:
            if is_reflection(rows, row_refl_idx):
                orig_row_refl_idx = row_refl_idx

        orig_col_refl_idx = None
        for col_refl_idx in col_refl_idx_candidates:
            if is_reflection(cols, col_refl_idx):
                orig_col_refl_idx = col_refl_idx

        num_found = 0
        for row_i, row_j, idx in row_smudges:

            rows_copy = deepcopy(rows)
            rows_copy[row_i] = rows_copy[row_i][:idx] + '?' + rows_copy[row_i][idx + 1:]
            rows_copy[row_j] = rows_copy[row_j][:idx] + '?' + rows_copy[row_j][idx + 1:]

            row_refl_idx_candidates = find_reflection_idx_candidates(rows_copy)
            for row_refl_idx in row_refl_idx_candidates:
                if is_reflection(rows_copy, row_refl_idx) and row_refl_idx != orig_row_refl_idx:
                    num_found += 1
                    row_sum += row_refl_idx

        for col_i, col_j, idx in col_smudges:
            cols_copy = deepcopy(cols)
            cols_copy[col_i] = cols_copy[col_i][:idx] + '?' + cols_copy[col_i][idx + 1:]
            cols_copy[col_j] = cols_copy[col_j][:idx] + '?' + cols_copy[col_j][idx + 1:]

            col_refl_idx_candidates = find_reflection_idx_candidates(cols_copy)
            for col_refl_idx in col_refl_idx_candidates:
                if is_reflection(cols_copy, col_refl_idx) and col_refl_idx != orig_col_refl_idx:
                    num_found += 1
                    col_sum += col_refl_idx

    total = col_sum + 100 * row_sum
    print("Part 2: " + str(total))

def main():
    line_grids = []
    line_grid = []
    for line in sys.stdin:
        if len(line) < 2:
            n = deepcopy(line_grid)
            line_grids.append(n)
            line_grid = []
            continue
        line_grid.append(line.rstrip())

    grids = []
    for grid in line_grids:
        rows = [row for row in grid]
        cols = []
        for col in range(len(grid[0])):
            cols.append("".join([row[col] for row in grid]))

        grids.append([rows, cols])


    part1(grids)
    part2(grids)

if __name__ == "__main__":
    main()