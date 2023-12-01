import sys

def main():
	s = 0
	for line in sys.stdin:
		first = ""
		second = ""
		for c in line:
			if c.isdigit():
				if not first:
					first = c
				second = c
		s += int(first + second)
	print(s)

if __name__ == "__main__":
    main()