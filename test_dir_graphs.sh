#!/bin/bash

# construction tests
graphs_construction_DIRS=(\
	"graphs/construction" "graphs/construction" \
	)

# construction tests
graphs_tree_types_DIRS=(\
	"graphs/tree_types" "graphs/tree_types" \
	)

# graph tests
graphs_DIRS=(\
	"${graphs_construction_DIRS[@]}" \
	"${graphs_tree_types_DIRS[@]}" \
	)
