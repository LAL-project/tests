#!/bin/bash

# numeric tests
memory_numeric_DIRS=(\
	"memory/numeric" "memory/numeric" \
	)

# graphs tests
memory_graphs_DIRS=(\
	"memory/graphs" "memory/graphs" \
	)

# detail tests
memory_detail_DIRS=(\
	"memory/detail" "memory/detail" \
	)

# memory tests
memory_DIRS=(\
	"${memory_numeric_DIRS[@]}" \
	"${memory_graphs_DIRS[@]}" \
	"${memory_detail_DIRS[@]}" \
	)
