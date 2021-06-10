#!/bin/bash

# generate (all labelled free) tests : alf
generate_trees_alf_DIRS=(\
	"generate/trees/all_lab_free" "generate/trees/all_lab_free" \
	)

# generate (all labelled rooted) tests : alr
generate_trees_alr_DIRS=(\
	"generate/trees/all_lab_rooted" "generate/trees/all_lab_rooted" \
	)

# generate (all unlabelled free) tests : auf
generate_trees_auf_DIRS=(\
	"generate/trees/all_ulab_free" "generate/trees/all_ulab_free" \
	)

# generate (all unlabelled rooted) tests : aur
generate_trees_aur_DIRS=(\
	"generate/trees/all_ulab_rooted" "generate/trees/all_ulab_rooted" \
	)

# generate (rand labelled free) tests : rlf
generate_trees_rlf_DIRS=(\
	"generate/trees/rand_lab_free" "generate/trees/rand_lab_free" \
	)

# generate (rand labelled rooted) tests : rlr
generate_trees_rlr_DIRS=(\
	"generate/trees/rand_lab_rooted" "generate/trees/rand_lab_rooted" \
	)

# generate (rand unlabelled rooted) tests : ruf
generate_trees_ruf_DIRS=(\
	"generate/trees/rand_ulab_free" "generate/trees/rand_ulab_free" \
	)

# generate (rand unlabelled rooted) tests : ruf
generate_trees_rur_DIRS=(\
	"generate/trees/rand_ulab_rooted" "generate/trees/rand_ulab_rooted" \
	)

# generate tests for trees
generate_trees_all_DIRS=(\
	"${generate_trees_alf_DIRS[@]}" \
	"${generate_trees_alr_DIRS[@]}" \
	"${generate_trees_auf_DIRS[@]}" \
	"${generate_trees_aur_DIRS[@]}" \
	)
generate_trees_rand_DIRS=(\
	"${generate_trees_rlf_DIRS[@]}" \
	"${generate_trees_rlr_DIRS[@]}" \
	"${generate_trees_ruf_DIRS[@]}" \
	"${generate_trees_rur_DIRS[@]}" \
	)
generate_trees_DIRS=(\
	"${generate_trees_all_DIRS[@]}" \
	"${generate_trees_rand_DIRS[@]}" \
	)

# generate (all unconstrained arrangements) tests
generate_arrangements_all_unc_DIRS=(\
	"generate/arrangements/all" "generate/arrangements/all" \
	)

# generate (rand unconstrained arrangements) tests
generate_arrangements_rand_unc_DIRS=(\
	"generate/arrangements/rand" "generate/arrangements/rand" \
	)

# generate (all projective arrangements) tests
generate_arrangements_all_projective_DIRS=(\
	"generate/arrangements/all_projective" "generate/arrangements/all_projective" \
	)

# generate (rand projective arrangements) tests
generate_arrangements_rand_projective_DIRS=(\
	"generate/arrangements/rand_projective" "generate/arrangements/rand_projective" \
	)

# generate (all planar arrangements) tests
generate_arrangements_all_planar_DIRS=(\
	"generate/arrangements/all_planar" "generate/arrangements/all_planar" \
	)

# generate (rand planar arrangements) tests
generate_arrangements_rand_planar_DIRS=(\
	"generate/arrangements/rand_planar" "generate/arrangements/rand_planar" \
	)

# generate tests for arrangements
generate_arrangements_all_DIRS=(\
	"${generate_arrangements_all_unc[@]}" \
	"${generate_arrangements_all_projective_DIRS[@]}" \
	"${generate_arrangements_all_planar_DIRS[@]}" \
	)
generate_arrangements_rand_DIRS=(\
	"${generate_arrangements_rand_unc[@]}" \
	"${generate_arrangements_rand_projective_DIRS[@]}" \
	"${generate_arrangements_rand_planar_DIRS[@]}" \
	)
generate_arrangements_DIRS=(\
	"${generate_arrangements_all_DIRS[@]}" \
	"${generate_arrangements_rand_DIRS[@]}" \
	)

# generate tests
generate_DIRS=(\
	"${generate_trees_DIRS[@]}" \
	"${generate_arrangements_DIRS[@]}" \
	)
