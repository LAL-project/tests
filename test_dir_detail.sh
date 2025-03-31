#!/bin/bash

# sorting tests
detail_sorting_insertion_DIRS=(\
	"detail/sorting/insertion_sort" "detail/sorting/insertion_sort" \
	)
detail_sorting_bit_DIRS=(\
	"detail/sorting/bit_sort" "detail/sorting/bit_sort" \
	)
detail_sorting_counting_DIRS=(\
	"detail/sorting/counting_sort" "detail/sorting/counting_sort" \
	)

detail_sorting_DIRS=(\
	"${detail_sorting_insertion_DIRS[@]}" \
	"${detail_sorting_bit_DIRS[@]}" \
	"${detail_sorting_counting_DIRS[@]}" \
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
