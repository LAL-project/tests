#!/bin/bash

utilities_isomorphism_small_DIRS=(\
	"utilities/tree_isomorphism/free/positive" "utilities/tree_isomorphism/free/positive" \
	"utilities/tree_isomorphism/free/negative" "utilities/tree_isomorphism/free/negative" \
	"utilities/tree_isomorphism/rooted/positive" "utilities/tree_isomorphism/rooted/positive" \
	"utilities/tree_isomorphism/rooted/negative" "utilities/tree_isomorphism/rooted/negative" \
	)
utilities_isomorphism_big_DIRS=(\
	"utilities/tree_isomorphism/" "utilities/tree_isomorphism" \
	)
utilities_isomorphism_DIRS=(\
	"${utilities_isomorphism_small_DIRS[@]}" \
	"${utilities_isomorphism_big_DIRS[@]}" \
	)

# utilities tests
utilities_DIRS=(\
	"${utilities_isomorphism_DIRS[@]}" \
	)
