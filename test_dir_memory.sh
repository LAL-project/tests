#!/bin/bash

# numeric tests
memory_numeric_DIRS=(\
	"memory/numeric" "memory/numeric" \
	)

# graphs tests
memory_graphs_DIRS=(\
	"memory/graphs" "memory/graphs" \
	)

# internal tests
memory_internal_DIRS=(\
	"memory/internal" "memory/internal" \
	)

# memory tests
memory_DIRS=(\
	"${memory_numeric_DIRS[@]}" \
	"${memory_graphs_DIRS[@]}" \
	"${memory_internal_DIRS[@]}" \
	)
