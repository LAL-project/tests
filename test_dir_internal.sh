#!/bin/bash

# sorting tests
internal_sorting_DIRS=(\
	"internal/sorting" "internal/sorting" \
	)

# internal_traversal tests
internal_traversal_DIRS=(\
	"internal/traversal" "internal/traversal" \
	)

# internal_centre tests
internal_centre_DIRS=(\
	"internal/centre" "internal/centre" \
	)

# internal_centre tests
internal_centroid_DIRS=(\
	"internal/centroid" "internal/centroid" \
	)

# internal tests
internal_DIRS=(\
	"${internal_sorting_DIRS[@]}" \
	"${internal_traversal_DIRS[@]}" \
	"${internal_centre_DIRS[@]}" \
	"${internal_centroid_DIRS[@]}" \
	)
