import lal

def output_decorated(g):
    print(g)
    print("|   " + g)
    print(g + "   |")
    print("|   " + g + "   |")

ug = lal.graphs.undirected_graph(10)
output_decorated(ug)

dg = lal.graphs.directed_graph(10)
output_decorated(dg)

ft = lal.graphs.free_tree(10)
output_decorated(ft)

rt = lal.graphs.rooted_tree(10)
output_decorated(rt)

print("If you see this message then the tests succeeded.")
