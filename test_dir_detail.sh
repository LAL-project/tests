#!/bin/bash

# sorting tests
detail_sorting_DIRS=(\
	"detail/sorting" "detail/sorting" \
	)

# detail_traversal tests
detail_traversal_DIRS=(\
	"detail/traversal" "detail/traversal" \
	)

# detail_centre tests
detail_centre_DIRS=(\
	"detail/centre" "detail/centre" \
	)

# detail_centre tests
detail_centroid_DIRS=(\
	"detail/centroid" "detail/centroid" \
	)

# detail tests
detail_DIRS=(\
	"${detail_sorting_DIRS[@]}" \
	"${detail_traversal_DIRS[@]}" \
	"${detail_centre_DIRS[@]}" \
	"${detail_centroid_DIRS[@]}" \
	)
