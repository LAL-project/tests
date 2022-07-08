#!/bin/bash

# sorting tests
detail_sorting_DIRS=(\
	"detail/sorting" "detail/sorting" \
	)

# detail_traversal tests
detail_traversal_DIRS=(\
	"detail/traversal" "detail/traversal" \
	)

# detail_avl tests
detail_avl_DIRS=(\
	"detail/avl" "detail/avl" \
	)

# detail tests
detail_DIRS=(\
	"${detail_sorting_DIRS[@]}" \
	"${detail_traversal_DIRS[@]}" \
	"${detail_avl_DIRS[@]}" \
	)
