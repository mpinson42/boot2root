
import sys
def sort_it(t):
	offset = t.find("//file")
	comp = int(t[offset + 6:len(t)])
	return comp
t = []
i = 1
max = len(sys.argv)
while i < max:
	f = open(sys.argv[i], "r")
	t.append(f.read())
	t.sort(key= sort_it)
	i += 1
i = 0
while i < max - 1:
	print(t[i])
	i += 1
