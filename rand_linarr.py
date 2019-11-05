import functools
import random
import sys

f = sys.argv[1]
R = int(sys.argv[2])
n = int(sys.argv[3])

print("TYPE linear_arrangements-compute_headedness")
print("INPUT 1 %s edge-list" % f)
print("BODY")
print("    %d" % R)

for r in range(0,R):
	linarr = [x for x in range(0,n)]
	random.shuffle(linarr)
	S = functools.reduce(lambda acc,k: acc + str(k) + " ", linarr, "")
	print("    " + S[:-1])
