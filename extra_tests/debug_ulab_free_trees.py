"""
This file is used to test that the generation of unlabelled free trees is uniform.
"""

import networkx as nx
import sys
import time

import sys
sys.path.append("/home/lluis/Documents/projects/linear-arrangement-library/python-interface/linux/64")
#sys.path.append("/home/usuaris/lalemany/installations/lib/python3.6")

import lal as lal

########################################################################
# GRAPH UTILS

# convert a lal's graph to a networkx's graph
def lal_to_nx(lal_g):
	nx_g = nx.Graph()
	nx_g.add_edges_from(list(lal_g.edges()))
	return nx_g

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

def binsearch_first(__l, __r, v, x):
	l = __l
	r = __r
	
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

def binsearch_last(__l, __r, v, x):
	l = __l
	r = __r
	
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
* _mmt_deg2s: second moment of degree of each graph
* _var_Cs: variance of C (rla) of each graph
* _iso_graphs: representative graph of their equivalence class
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
			mmt_deg = lal.properties.mmt_degree_rational(tree, 2)
			var_C_rla = lal.properties.variance_C_tree_rational(tree)
			
			_iso_graphs.append((mmt_deg, var_C_rla, g))
		
		# sort trees by degree sequence
		_iso_graphs = sorted(_iso_graphs, key=lambda k: (k[0], k[1]))
		
		self._mmt_deg2s = list(map(lambda k: k[0], _iso_graphs))
		self._var_Cs = list(map(lambda k: k[1], _iso_graphs))
		self._iso_graphs = list(map(lambda k: k[2], _iso_graphs))
	
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
	def find_class(self, lal_t, nx_t):
		mmt_deg2s = self._mmt_deg2s
		var_Cs = self._var_Cs
		iso_gs = self._iso_graphs
		k = self._n_classes
		
		t_mmt_deg2 = lal.properties.mmt_degree_rational(lal_t, 2)
		
		# Each tree has a unique degree sequence ==>
		# has a unique second moment of degree
		if k <= 5:
			for i in range(0, k):
				if mmt_deg2s[i] == t_mmt_deg2:
					return i
			return -1
		
		t_var_C = lal.properties.variance_C_tree_rational(lal_t)
		
		# Each tree has a unique value of the variance of C (rla)
		if k <= 8:
			for i in range(0, k):
				if var_Cs[i] == t_var_C:
					return i
			return -1
		
		# Too many graphs
		
		# Use binary search on the list of second moment of degree values
		# to delimit the interval of graphs against which actual
		# isomorphism test is required
		first_mmt = binsearch_first(0, k-1, mmt_deg2s, t_mmt_deg2)
		last_mmt  = binsearch_last(0, k-1, mmt_deg2s, t_mmt_deg2)
		
		# Use binary search on the list of variance of C values to
		# delimit the interval of graphs against which actual
		# isomorphism test is required
		first_var = binsearch_first(first_mmt, last_mmt, var_Cs, t_var_C)
		last_var  = binsearch_last(first_mmt, last_mmt, var_Cs, t_var_C)
		
		if first_var == last_var:
			# we have found the class
			return first_var
		
		# iterate through all candidates
		for i in range(first_var, last_var+1):
			if nx.is_isomorphic(nx_t, iso_gs[i]):
				return i
		# we could not find the equivalence class of tree 't'
		return -1
		
		"""
		for i in range(0, k):
			if nx.is_isomorphic(nx_t, iso_gs[i]):
				return i
		return -1
		"""

	def n_classes(self):
		return self._n_classes

def debug_n_verts(n):
	N = 1000000
	
	EQUIV = equivalence_classes_iso_graphs(n)
	
	n_classes = EQUIV.n_classes()
	print("Amount of non-isomorphic graphs:", n_classes)
	
	# amount of trees generated for each equivalence class
	count = [0 for _ in range(0, n_classes)]
	# amount of caterpillar trees generated
	n_caterpillar = 0
	
	# "iteration" execution time
	start_it = time.time()
	cumul_time = 0
	# full execution time
	begin = time.time()
	
	# generate trees
	TreeGen = lal.generation.rand_ulab_free_trees(n)
	for k in range(0,N):
		if k > 0 and k%500 == 0:
			now = time.time()
			cumul_time += now - start_it
			print("%d/%d" % (k, N), "-> it: %.4f s, cumul: %.4f s" % (now - start_it, cumul_time))
			start_it = now
		
		# generate tree
		lal_t = TreeGen.make_rand_tree()
		
		# find equivalence class
		i = EQUIV.find_class(lal_t, lal_to_nx(lal_t))
		if i == -1:
			print("Error: could not find class for tree:")
			print(lal_t)
			exit(1)
		
		# is this tree a caterpillar tree?
		n_caterpillar += is_caterpillar(lal_t)
		
		# keep track of the count
		count[i] += 1
	
	# print last iteration
	now = time.time()
	cumul_time += now - start_it
	print("%d/%d" % (N, N), "-> it: %.4f s, cumul: %.4f s" % (now - start_it, cumul_time))
	start_it = now

	end = time.time()
	
	prop_count = list(map(lambda x: x/N, count))
	print("# trees per equivalence class:")
	print("    c(%.8f" % prop_count[0], end='')
	for d in range(1,n_classes):
		print(", %.8f" % prop_count[d], end='')
	print(")")
	print("--------------------------------------------------------")
	print("Number of replicas:", N)
	print("Number of equivalence classes:", n_classes)
	num_caterpillar = pow(2,n-4) + pow(2, int((n-4)/2))
	print("NUmber of caterpillar trees of", n, "vertices:", num_caterpillar)
	r = lal.numeric.rational(0)
	r.init_ui(num_caterpillar, n_classes)
	print("Caterpillar trees:")
	print("    Theoretical proportion=", r, "~ %.8f" % (num_caterpillar/n_classes))
	print("    generated", n_caterpillar, "caterpillar trees ~ %.8f" % (n_caterpillar/N))
	print("Calculated in %.4f s" % (end - begin))

n = int(sys.argv[1])
debug_n_verts(n)

