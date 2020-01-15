"""
This file is used to test that the generation of unlabelled free trees is uniform.
"""

import networkx as nx
import sys
import time

import lal as lal

########################################################################
# GRAPH UTILS

# convert a lal's graph to a networkx's graph
def lal_to_nx(lal_g):
	nx_g = nx.Graph()
	nx_g.add_edges_from(list(lal_g.edges()))
	return nx_g

# build the degree sequence of a networkx's graph 't'
def get_degree_seq(g):
	return [g.degree(u) for u in range(0,g.number_of_nodes())]

# is a LAL's tree a caterpillar tree?
def is_caterpillar(t):
	def __larger1(d):
		if d > 1: return d
		return 0
	
	N = t.n_nodes()
	
	# degree of the internal vertices
	deg_internal = [__larger1(t.degree(u)) for u in range(0,N)]
	
	# reduce the degree of the internal vertices
	# as many times as leaves are connected to them
	for u in range(0,N):
		if t.degree(u) == 1:
			deg_internal[ t.get_neighbours(u)[0] ] -= 1
	
	# If we are left with 2, or 0, vertices with degree 1,
	# it means that after removing the leaves of the tree
	# all vertices are internal (degree 2), i.e., they are
	# part of a linear tree. Needless to say that these
	# two vertices of degree 1 are the endpoints of the
	# linear tree.
	n1 = 0
	for u in range(0,N):
		n1 += (deg_internal[u] == 1)
	return n1 == 2 or n1 == 0

def binsearch_first(v, x):
	l = 0
	r = len(v) - 1
	
	while l < r and abs(l - r) > 1:
		m = int((l + r)/2)
		if x == v[m]:
			r = m
		elif x < v[m]:
			r = m - 1
		else:
			l = m + 1
	
	if l > r: return -1;
	# l and r might be the same: if they are the following two
	# if statements are redundant. However, if |l-r|=1 then we
	# need to test for l first.
	if v[l] == x: return l
	if v[r] == x: return r
	return -1

def binsearch_last(v, x):
	l = 0
	r = len(v) - 1
	
	while l < r and abs(l - r) > 1:
		m = int((l + r)/2)
		if x == v[m]:
			l = m
		elif x < v[m]:
			r = m - 1
		else:
			l = m + 1
	
	if l > r: return -1;
	# l and r might be the same: if they are the following two
	# if statements are redundant. However, if |l-r|=1 then we
	# need to test for r first.
	if v[r] == x: return r
	if v[l] == x: return l
	return -1

"""
This class is used as a data structure to speed up locating
the equivalence class (under graph isomorphism) of a given graph.

Attributes:
* _n: number of vertices
* _deg_seqs: degree sequences of representative graphs
* _iso_graphs: representative graphs of their equivalence class
* _n_classes: number of equivalence classes
"""
class equivalence_classes_iso_graphs:
	def _build_equivalence_classes(self):
		# unique graphs under isomorphism
		_iso_graphs = []
		
		# generate the list of graphs
		TreeGen = lal.generation.all_ulab_free_trees(self._n)
		while TreeGen.has_next():
			TreeGen.next()
			tree = TreeGen.get_tree()
			
			g = lal_to_nx(tree)
			deg_seq = get_degree_seq(g)
			deg_seq = sorted(deg_seq)
			
			_iso_graphs.append((deg_seq, g))
		
		# sort trees by degree sequence
		_iso_graphs = sorted(_iso_graphs, key=lambda k: k[0])
		
		self._deg_seqs = list(map(lambda k: k[0], _iso_graphs))
		self._iso_graphs = list(map(lambda k: k[1], _iso_graphs))
	
	"""
	Initialises the class.
	n: number of vertices of the trees.
	"""
	def __init__(self, n):
		# number of vertices
		self._n = n
		
		# generate all "unique" trees and store them
		# separately into two lists:
		#	-> _deg_seqs
		#	-> iso_graphs
		self._build_equivalence_classes()
		
		# number of equivalence classes
		self._n_classes = len(self._iso_graphs)
	
	"""
	Returns a positive index of the equivalence class of tree 't'.
	Returns -1 if the class was not found.
	"""
	def find_class(self, t):
		deg_sq = self._deg_seqs
		iso_gs = self._iso_graphs
		k = self._n_classes
		
		t_deg_seq = sorted(get_degree_seq(t))
		
		# Fewer trees + each tree has a unique degree sequence
		if k <= 5:
			for i in range(0, k):
				if deg_sq[i] == t_deg_seq:
					return i
			return -1
		
		# Too many graphs: use binary search. Delimit the interval
		# of graphs against which actual isomorphism test is required
		first = binsearch_first(deg_sq, t_deg_seq)
		last  = binsearch_last(deg_sq, t_deg_seq)
		if first == last:
			# we have found the class
			return first
		# iterate through all candidates
		for i in range(first, last+1):
			if nx.is_isomorphic(t, iso_gs[i]):
				return i
		# we could not find the equivalence class of tree 't'
		return -1
		
		"""
		for i in range(0, k):
			if nx.is_isomorphic(t, iso_gs[i]):
				return i
		return -1
		"""

	def n_classes(self):
		return self._n_classes

def debug_n_verts(n):
	N = 100000
	
	EQUIV = equivalence_classes_iso_graphs(n)
	
	n_non_iso = EQUIV.n_classes()
	print("Amount of non-isomorphic graphs:", n_non_iso)
	
	# amount of trees generated for each equivalence class
	count = [0 for _ in range(0, n_non_iso)]
	# amount of caterpillar trees generated
	n_caterpillar = 0
	
	# "iteration" execution time
	start_it = time.time()
	cumul_time = 0
	first_time = True
	# full execution time
	begin = time.time()
	
	# generate trees
	TreeGen = lal.generation.rand_ulab_free_trees(n)
	for k in range(0,N):
		if k%500 == 0:
			now = time.time()
			if first_time:
				print("%d/%d" % (k, N))
				first_time = False
			else:
				cumul_time += now - start_it
				print("%d/%d" % (k, N), "-> it: %.4f s, cumul: %.4f s" % (now - start_it, cumul_time))
			start_it = now
		
		# generate tree
		lal_t = TreeGen.make_rand_tree()
		
		# find equivalence class
		i = EQUIV.find_class(lal_to_nx(lal_t))
		if i == -1:
			print("Error: could not find class for tree:")
			print(t)
			exit(1)
		
		# is this tree a caterpillar tree?
		n_caterpillar += is_caterpillar(lal_t)
		
		# keep track of the count
		count[i] += 1
	
	end = time.time()
	
	print("# trees per equivalence class:", list(map(lambda x: x/N, count)))
	print("--------------------------------------------------------")
	print("Number of replicas:", N)
	print("Number of equivalence classes:", n_non_iso)
	num_caterpillar = pow(2,n-4) + pow(2, int((n-4)/2))
	print("NUmber of caterpillar trees of", n, "vertices:", num_caterpillar)
	r = lal.numeric.rational(num_caterpillar, n_non_iso)
	print("Caterpillar trees:")
	print("    Theoretical proportion=", r, "~ %.8f" % (num_caterpillar/n_non_iso))
	print("    generated", n_caterpillar, "caterpillar trees ~ %.8f" % (n_caterpillar/N))
	print("Calculated in %.4f s" % (end - begin))

n = int(sys.argv[1])
debug_n_verts(n)

