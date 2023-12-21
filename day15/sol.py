import sys
from dataclasses import dataclass

@dataclass
class Lens:
    label: str
    focal_length: int

def compute_hash(s):
    val = 0
    for c in s:
        val += ord(c)
        val *= 17
        val %= 256
    return val

def part1(strings):
    total = 0
    for s in strings: 
        total += compute_hash(s)

    print("Part 1: " + str(total))

def part2(lenses):
    boxes = {}
    for lens in lenses:
        key = compute_hash(lens.label)
        if lens.focal_length == 0:
            if key in boxes:
                l = boxes[key]
                idx = None
                for i, v in enumerate(l):
                    if v.label == lens.label:
                        idx = i
                        break
                if idx != None:
                    del boxes[key][idx]
        else:
            if key not in boxes or len(boxes[key]) == 0:
                boxes[key] = [lens]
            else:
                found = False
                for v in boxes[key]:
                    if v.label == lens.label:
                        v.focal_length = lens.focal_length
                        found = True
                        break
                if not found:
                    boxes[key].append(lens)

    total = 0
    for box_num, box in boxes.items():
        for slot, lens in enumerate(box):
            total += (box_num + 1) * (slot + 1) * lens.focal_length

    print("Part 2: " + str(total))


def main():
    strings = sys.stdin.readline().split(',')
    part1(strings)
    
    lenses = []
    for s in strings:
        if '=' in s:
            l = s.split('=')
            lenses.append(Lens(l[0], int(l[1])))
        else:
            l = s.split('-')
            lenses.append(Lens(l[0], 0))
    part2(lenses)

if __name__ == "__main__":
    main()