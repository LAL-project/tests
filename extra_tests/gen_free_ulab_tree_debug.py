import functools
import networkx as nx
import sys

"""
This file is used to test that the generation of unlabelled free trees is uniform.
"""

import lal

def is_caterpillar(t):
	def __larger1(d):
		if d > 1: return d
		return 0
	
	N = t.n_nodes()
	
	# degree of the internal vertices
	deg_internal = [__larger1(t.degree(u)) for u in range(0,N)]
	n_internal = functools.reduce(lambda acc,x: acc + (x > 1), deg_internal, 0)
	
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

def lal_graph_to_nx_graph(lal_g):
	nx_g = nx.Graph()
	nx_g.add_edges_from(list(lal_g.edges()))
	return nx_g

def get_degree_seq(t):
	return [t.degree(u) for u in range(0,t.n_nodes())]

def list_iso_graphs(n):
	# unique graphs under isomorphism
	graphs = []
	
	# generate the list of graphs
	TreeGen = lal.generation.all_ulab_free_trees()
	TreeGen.init(n)
	while TreeGen.has_next():
		TreeGen.next()
		tree = TreeGen.get_tree()
		graphs.append(lal_graph_to_nx_graph(tree))
	
	return graphs

def find_isomorph(nx_g, iso_graphs):
	n = len(iso_graphs)
	for i in range(0, n):
		if nx.is_isomorphic(nx_g, iso_graphs[i]):
			return i
	return -1

def debug_n_verts(n):
	N = 50000
	
	iso_gs = list_iso_graphs(n)
	n_non_iso = len(iso_gs)
	print("Amount of non-isomorphic graphs:", n_non_iso)
	
	# amount of trees generated for each equivalence class
	count = [0 for _ in range(0, n_non_iso)]
	# amount of caterpillar trees generated
	n_caterpillar = 0
	
	# generate trees
	TreeGen = lal.generation.rand_ulab_free_trees(n)
	for k in range(0,N):
		if k%500 == 0:
			print(k/N*100, "%")
		
		# generate tree
		t = TreeGen.make_rand_tree()
		
		# find equivalence class
		i = find_isomorph(lal_graph_to_nx_graph(t), iso_gs)
		
		# is this tree a caterpillar tree?
		n_caterpillar += is_caterpillar(t)
		
		# keep track of the count
		if i != -1: count[i] += 1
		else:
			print(t)
			exit(1)
	
	print("# trees per equivalence class:", list(map(lambda x: x/N, count)))
	print("--------------------------------------------------------")
	print("Number of equivalence classes:", n_non_iso)
	num_caterpillar = pow(2,n-4) + pow(2, int((n-4)/2))
	print("NUmber of caterpillar trees of", n, "vertices:", num_caterpillar)
	print("Theoretical proportion:", num_caterpillar/n_non_iso)
	print("    generated", n_caterpillar, "caterpillar trees")
	print("        ~", n_caterpillar/N)
	

n = int(sys.argv[1])
debug_n_verts(n)

