import laldebug as lal

"""
This file is used to test that the generation of unlabelled rooted trees is uniform.

In order to use this file, you have to modify LAL's source code to make the function lal::generate::rand_ulab_rooted_trees::make_rand_tree() to return a vector of ints, instead of an instance of lal::graphs::urtree.
"""

def test3():
	gen = lal.generation.rand_ulab_rooted_trees()
	gen.init(3, 1234)
	num_trees = [0 for _ in range(0,3)]
	for i in range(0,10000):
		rT = gen.make_rand_tree()
		if rT == (0,0,1):   num_trees[0] += 1
		elif rT == (0,0,0): num_trees[1] += 1
		else:
			print(rT)
			exit(1)
	print(num_trees)

def test4():
	gen = lal.generation.rand_ulab_rooted_trees()
	gen.init(4, 1234)
	num_trees = [0 for _ in range(0,420)]
	for i in range(0,1000000):
		rT = gen.make_rand_tree()
		if rT == (0,0,1,2):   num_trees[0] += 1
		elif rT == (0,0,0,2): num_trees[1] += 1
		elif rT == (0,0,1,0): num_trees[1] += 1
		elif rT == (0,0,1,1): num_trees[2] += 1
		elif rT == (0,0,0,0): num_trees[3] += 1
		else:
			print(rT)
			exit(1)
	print(num_trees)

def test5():
	gen = lal.generation.rand_ulab_rooted_trees()
	gen.init(5, 1234)
	num_trees = [0 for _ in range(0,9)]
	for i in range(0,1000000):
		rT = gen.make_rand_tree()
		if rT == (0,0,1,2,3):   num_trees[0] += 1
		elif rT == (0,0,0,2,3): num_trees[1] += 1
		elif rT == (0,0,1,2,0): num_trees[1] += 1
		elif rT == (0,0,1,0,3): num_trees[2] += 1
		elif rT == (0,0,1,1,1): num_trees[3] += 1
		elif rT == (0,0,0,0,0): num_trees[4] += 1
		elif rT == (0,0,1,2,2): num_trees[5] += 1
		elif rT == (0,0,0,2,2): num_trees[6] += 1
		elif rT == (0,0,1,1,0): num_trees[6] += 1
		elif rT == (0,0,0,0,3): num_trees[7] += 1
		elif rT == (0,0,0,2,0): num_trees[7] += 1
		elif rT == (0,0,1,0,0): num_trees[7] += 1
		elif rT == (0,0,1,2,1): num_trees[8] += 1
		elif rT == (0,0,1,1,3): num_trees[8] += 1
		else:
			print(rT)
			exit(1)
	print(num_trees)

def test6():
	gen = lal.generation.rand_ulab_rooted_trees()
	gen.init(6, 1234)
	num_trees = [0 for _ in range(0,20)]
	for i in range(0,1000000):
		rT = gen.make_rand_tree()
		if rT == (0,0,1,2,3,4):   num_trees[0] += 1
		elif rT == (0,0,1,2,3,0): num_trees[1] += 1
		elif rT == (0,0,0,2,3,4): num_trees[1] += 1
		elif rT == (0,0,1,2,0,4): num_trees[2] += 1
		elif rT == (0,0,1,0,3,4): num_trees[2] += 1
		elif rT == (0,0,1,2,3,3): num_trees[3] += 1
		elif rT == (0,0,0,2,3,3): num_trees[4] += 1
		elif rT == (0,0,1,2,2,0): num_trees[4] += 1
		elif rT == (0,0,1,0,3,3): num_trees[5] += 1
		elif rT == (0,0,1,1,0,4): num_trees[5] += 1
		elif rT == (0,0,0,2,3,0): num_trees[6] += 1
		elif rT == (0,0,0,0,3,4): num_trees[6] += 1
		elif rT == (0,0,1,2,0,0): num_trees[6] += 1
		elif rT == (0,0,1,2,3,1): num_trees[7] += 1
		elif rT == (0,0,1,1,3,4): num_trees[7] += 1
		elif rT == (0,0,1,2,2,1): num_trees[8] += 1
		elif rT == (0,0,1,1,3,3): num_trees[8] += 1
		elif rT == (0,0,1,1,0,0): num_trees[9] += 1
		elif rT == (0,0,0,2,2,0): num_trees[9] += 1
		elif rT == (0,0,0,0,3,3): num_trees[9] += 1
		elif rT == (0,0,1,2,1,4): num_trees[10] += 1
		elif rT == (0,0,1,0,0,4): num_trees[11] += 1
		elif rT == (0,0,1,0,3,0): num_trees[11] += 1
		elif rT == (0,0,0,2,0,4): num_trees[11] += 1
		elif rT == (0,0,1,1,3,0): num_trees[12] += 1
		elif rT == (0,0,0,2,3,2): num_trees[12] += 1
		elif rT == (0,0,1,2,1,0): num_trees[12] += 1
		elif rT == (0,0,0,2,2,4): num_trees[12] += 1
		elif rT == (0,0,1,2,2,4): num_trees[13] += 1
		elif rT == (0,0,1,2,3,2): num_trees[13] += 1
		elif rT == (0,0,0,0,0,4): num_trees[14] += 1
		elif rT == (0,0,0,0,3,0): num_trees[14] += 1
		elif rT == (0,0,0,2,0,0): num_trees[14] += 1
		elif rT == (0,0,1,0,0,0): num_trees[14] += 1
		elif rT == (0,0,1,1,1,4): num_trees[15] += 1
		elif rT == (0,0,1,2,1,1): num_trees[15] += 1
		elif rT == (0,0,1,1,3,1): num_trees[15] += 1
		elif rT == (0,0,1,1,1,0): num_trees[16] += 1
		elif rT == (0,0,0,2,2,2): num_trees[16] += 1
		elif rT == (0,0,1,2,2,2): num_trees[17] += 1
		elif rT == (0,0,0,0,0,0): num_trees[18] += 1
		elif rT == (0,0,1,1,1,1): num_trees[19] += 1
		else:
			print(rT)
			exit(1)
		
	print(num_trees)


test6()
