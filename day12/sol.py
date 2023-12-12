import sys
import itertools

def is_valid_to(spring, groups, index = -1):
    return False

def part1(spring_rows):


def main():
    spring_rows = []
    for line in sys.stdin:
        row = line.split()
        springs = list(row[0])
        groups = list(row[1])
        spring_rows.append((springs, groups))

    part1(spring_rows)


if __name__ == "__main__":
    main()