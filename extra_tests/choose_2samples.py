import random

"""
Test whether the selection of a pair of two values (v1,v2) from a collection of n values v1,v2,...,vn is uniform.
"""

def irange(a,b): return range(a,b+1)

def choose_2sample(n):
	# uniformly random number in [0,1]
	r = random.random()
	if r <= 1/(n+1):
		# uniformly random integer in [1,n]
		i = random.randint(1,n)
		return (i,i)
	
	# two uniformly random integers in [1,n]
	i = random.randint(1,n)
	j = random.randint(1,n)
	return tuple(sorted( (i,j) ))

random.seed(1234)
N = 10
all_samples = {}
for i in irange(1,N):
	for j in irange(i,N):
		all_samples[ (i,j) ] = 0

R = 1000000
for k in range(0,R):
	all_samples[ choose_2sample(N) ] += 1

for i in irange(1,N):
	for j in irange(i,N):
		print( "(%d,%d) -> %d " % (i,j, all_samples[(i,j)]))
