#!/bin/bash

utilities_isomorphism_free_large_DIRS=(\
	"utilities/tree_isomorphism/free" "utilities/tree_isomorphism/free"
	)
utilities_isomorphism_free_small_DIRS=(\
	"utilities/tree_isomorphism/free/negative" "utilities/tree_isomorphism/free/negative"
	"utilities/tree_isomorphism/free/positive" "utilities/tree_isomorphism/free/positive"
	)
utilities_isomorphism_free_DIRS=(\
	"${utilities_isomorphism_free_large_DIRS[@]}" \
	"${utilities_isomorphism_free_small_DIRS[@]}" \
	)

utilities_isomorphism_rooted_LARGE_large_DIRS=(\
	"utilities/tree_isomorphism/rooted/large" "utilities/tree_isomorphism/rooted"
	)
utilities_isomorphism_rooted_LARGE_small_DIRS=(\
	"utilities/tree_isomorphism/rooted/large/negative" "utilities/tree_isomorphism/rooted/negative"
	"utilities/tree_isomorphism/rooted/large/positive" "utilities/tree_isomorphism/rooted/positive"
	)
utilities_isomorphism_rooted_LARGE_DIRS=(\
	"${utilities_isomorphism_rooted_LARGE_small_DIRS[@]}" \
	"${utilities_isomorphism_rooted_LARGE_large_DIRS[@]}" \
	)

utilities_isomorphism_rooted_SMALL_large_DIRS=(\
	"utilities/tree_isomorphism/rooted/small" "utilities/tree_isomorphism/rooted"
	)
utilities_isomorphism_rooted_SMALL_small_DIRS=(\
	"utilities/tree_isomorphism/rooted/small/negative" "utilities/tree_isomorphism/rooted/negative"
	"utilities/tree_isomorphism/rooted/small/positive" "utilities/tree_isomorphism/rooted/positive"
	)
utilities_isomorphism_rooted_SMALL_DIRS=(\
	"${utilities_isomorphism_rooted_SMALL_small_DIRS[@]}" \
	"${utilities_isomorphism_rooted_SMALL_large_DIRS[@]}" \
	)

utilities_isomorphism_rooted_DIRS=(\
	"${utilities_isomorphism_rooted_LARGE_DIRS[@]}" \
	"${utilities_isomorphism_rooted_SMALL_DIRS[@]}" \
	)

utilities_isomorphism_LARGE_DIRS=(\
	"${utilities_isomorphism_rooted_LARGE_DIRS[@]}" \
	)
utilities_isomorphism_SMALL_DIRS=(\
	"${utilities_isomorphism_rooted_SMALL_DIRS[@]}" \
	)

utilities_isomorphism_DIRS=(\
	"${utilities_isomorphism_rooted_DIRS[@]}" \
	"${utilities_isomorphism_free_DIRS[@]}" \
	)

# utilities tests
utilities_DIRS=(\
	"${utilities_isomorphism_DIRS[@]}" \
	)
