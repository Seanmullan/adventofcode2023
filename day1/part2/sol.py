import sys
from word2number import w2n

NUMBERS = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]

def main():
 	
	s = 0
	for line in sys.stdin:
		first = ""
		second = ""

		for i in range(len(line)):
			sub = line[i:]
			for n in NUMBERS:
				if sub.find(n) == 0:
					first = n
					break
			if first:
				break

		for i in range(len(line)):
			rev = line[::-1]
			sub = rev[i:]
			for n in NUMBERS:
				rev_n = n[::-1]
				if sub.find(rev_n) == 0:
					second = n
					break
			if second:
				break
		
		num = str(w2n.word_to_num(first)) + str(w2n.word_to_num(second))
		s += int(num)
	
	print(s)

if __name__ == "__main__":
    main()