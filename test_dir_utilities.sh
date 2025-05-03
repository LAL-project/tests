#!/bin/bash

utilities_isomorphism_free_tuple_manual_positive_rootless_DIRS=(\
	"utilities/tree_isomorphism/free/tuple/manual/positive/rootless" "utilities/tree_isomorphism/free/tuple/manual/positive/rootless"
	)
utilities_isomorphism_free_tuple_manual_positive_rooted_DIRS=(\
	"utilities/tree_isomorphism/free/tuple/manual/positive/rooted" "utilities/tree_isomorphism/free/tuple/manual/positive/rooted"
	)
utilities_isomorphism_free_tuple_manual_positive_DIRS=(\
	"${utilities_isomorphism_free_tuple_manual_positive_rootless_DIRS[@]}" \
	"${utilities_isomorphism_free_tuple_manual_positive_rooted_DIRS[@]}" \
	)

utilities_isomorphism_free_tuple_manual_negative_rootless_DIRS=(\
	"utilities/tree_isomorphism/free/tuple/manual/negative/rootless" "utilities/tree_isomorphism/free/tuple/manual/negative/rootless"
	)
utilities_isomorphism_free_tuple_manual_negative_rooted_DIRS=(\
	"utilities/tree_isomorphism/free/tuple/manual/negative/rooted" "utilities/tree_isomorphism/free/tuple/manual/negative/rooted"
	)
utilities_isomorphism_free_tuple_manual_negative_DIRS=(\
	"${utilities_isomorphism_free_tuple_manual_negative_rootless_DIRS[@]}" \
	"${utilities_isomorphism_free_tuple_manual_negative_rooted_DIRS[@]}" \
	)

utilities_isomorphism_free_tuple_manual_DIRS=(\
	"${utilities_isomorphism_free_tuple_manual_negative_DIRS[@]}" \
	"${utilities_isomorphism_free_tuple_manual_positive_DIRS[@]}" \
	)

utilities_isomorphism_free_tuple_automatic_rootless_DIRS=(\
	"utilities/tree_isomorphism/free/tuple/automatic/rootless" "utilities/tree_isomorphism/free/tuple/automatic/rootless"
	)
utilities_isomorphism_free_tuple_automatic_rooted_DIRS=(\
	"utilities/tree_isomorphism/free/tuple/automatic/rooted" "utilities/tree_isomorphism/free/tuple/automatic/rooted"
	)
utilities_isomorphism_free_tuple_automatic_DIRS=(\
	"${utilities_isomorphism_free_tuple_automatic_rootless_DIRS[@]}" \
	"${utilities_isomorphism_free_tuple_automatic_rooted_DIRS[@]}" \
	)

utilities_isomorphism_free_tuple_DIRS=(\
	"${utilities_isomorphism_free_tuple_automatic_DIRS[@]}" \
	"${utilities_isomorphism_free_tuple_manual_DIRS[@]}" \
	)

utilities_isomorphism_free_string_manual_positive_rootless_DIRS=(\
	"utilities/tree_isomorphism/free/string/manual/positive/rootless" "utilities/tree_isomorphism/free/string/manual/positive/rootless"
	)
utilities_isomorphism_free_string_manual_positive_rooted_DIRS=(\
	"utilities/tree_isomorphism/free/string/manual/positive/rooted" "utilities/tree_isomorphism/free/string/manual/positive/rooted"
	)
utilities_isomorphism_free_string_manual_positive_DIRS=(\
	"${utilities_isomorphism_free_string_manual_positive_rootless_DIRS[@]}" \
	"${utilities_isomorphism_free_string_manual_positive_rooted_DIRS[@]}" \
	)

utilities_isomorphism_free_string_manual_negative_rootless_DIRS=(\
	"utilities/tree_isomorphism/free/string/manual/negative/rootless" "utilities/tree_isomorphism/free/string/manual/negative/rootless"
	)
utilities_isomorphism_free_string_manual_negative_rooted_DIRS=(\
	"utilities/tree_isomorphism/free/string/manual/negative/rooted" "utilities/tree_isomorphism/free/string/manual/negative/rooted"
	)
utilities_isomorphism_free_string_manual_negative_DIRS=(\
	"${utilities_isomorphism_free_string_manual_negative_rootless_DIRS[@]}" \
	"${utilities_isomorphism_free_string_manual_negative_rooted_DIRS[@]}" \
	)

utilities_isomorphism_free_string_manual_DIRS=(\
	"${utilities_isomorphism_free_string_manual_negative_DIRS[@]}" \
	"${utilities_isomorphism_free_string_manual_positive_DIRS[@]}" \
	)

utilities_isomorphism_free_string_automatic_rootless_DIRS=(\
	"utilities/tree_isomorphism/free/string/automatic/rootless" "utilities/tree_isomorphism/free/string/automatic/rootless"
	)
utilities_isomorphism_free_string_automatic_rooted_DIRS=(\
	"utilities/tree_isomorphism/free/string/automatic/rooted" "utilities/tree_isomorphism/free/string/automatic/rooted"
	)
utilities_isomorphism_free_string_automatic_DIRS=(\
	"${utilities_isomorphism_free_string_automatic_rootless_DIRS[@]}" \
	"${utilities_isomorphism_free_string_automatic_rooted_DIRS[@]}" \
	)

utilities_isomorphism_free_string_DIRS=(\
	"${utilities_isomorphism_free_string_automatic_DIRS[@]}" \
	"${utilities_isomorphism_free_string_manual_DIRS[@]}" \
	)

utilities_isomorphism_free_DIRS=(\
	"${utilities_isomorphism_free_string_DIRS[@]}" \
	"${utilities_isomorphism_free_tuple_DIRS[@]}" \
	)

utilities_isomorphism_rooted_string_manual_positive_DIRS=(\
	"utilities/tree_isomorphism/rooted/string/manual/positive" "utilities/tree_isomorphism/rooted/string/manual/positive"
	)

utilities_isomorphism_rooted_string_manual_negative_DIRS=(\
	"utilities/tree_isomorphism/rooted/string/manual/negative" "utilities/tree_isomorphism/rooted/string/manual/negative"
	)

utilities_isomorphism_rooted_string_manual_DIRS=(\
	"${utilities_isomorphism_rooted_string_manual_positive_DIRS[@]}" \
	"${utilities_isomorphism_rooted_string_manual_negative_DIRS[@]}" \
	)

utilities_isomorphism_rooted_string_automatic_DIRS=(\
	"utilities/tree_isomorphism/rooted/string/automatic" "utilities/tree_isomorphism/rooted/string/automatic"
	)

utilities_isomorphism_rooted_string_DIRS=(\
	"${utilities_isomorphism_rooted_string_manual_DIRS[@]}" \
	"${utilities_isomorphism_rooted_string_automatic_DIRS[@]}" \
	)

utilities_isomorphism_rooted_tuple_manual_positive_DIRS=(\
	"utilities/tree_isomorphism/rooted/tuple/manual/positive" "utilities/tree_isomorphism/rooted/tuple/manual/positive"
	)

utilities_isomorphism_rooted_tuple_manual_negative_DIRS=(\
	"utilities/tree_isomorphism/rooted/tuple/manual/negative" "utilities/tree_isomorphism/rooted/tuple/manual/negative"
	)

utilities_isomorphism_rooted_tuple_manual_DIRS=(\
	"${utilities_isomorphism_rooted_tuple_manual_positive_DIRS[@]}" \
	"${utilities_isomorphism_rooted_tuple_manual_negative_DIRS[@]}" \
	)

utilities_isomorphism_rooted_tuple_automatic_DIRS=(\
	"utilities/tree_isomorphism/rooted/tuple/automatic" "utilities/tree_isomorphism/rooted/tuple/automatic"
	)

utilities_isomorphism_rooted_tuple_DIRS=(\
	"${utilities_isomorphism_rooted_tuple_manual_DIRS[@]}" \
	"${utilities_isomorphism_rooted_tuple_automatic_DIRS[@]}" \
	)

utilities_isomorphism_rooted_DIRS=(\
	"${utilities_isomorphism_rooted_string_DIRS[@]}" \
	"${utilities_isomorphism_rooted_tuple_DIRS[@]}" \
	)

utilities_isomorphism_DIRS=(\
	"${utilities_isomorphism_free_DIRS[@]}" \
	"${utilities_isomorphism_rooted_DIRS[@]}" \
	)

# utilities tests
utilities_DIRS=(\
	"${utilities_isomorphism_DIRS[@]}" \
	)
