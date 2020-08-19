#!/bin/bash

# all groups -- used only as a means to test the correctness of
# the value of the parameter --exe-group=*.
groups_list=(\
	"all" \
	"construction" \
	"numeric" \
	"utilities" \
	"utilities_isomorphism" \
	"exp_var_C" \
	"exp_var_D" \
	"linarr" \
	"linarr_E_2Cd" \
	"linarr_C" \
	"linarr_C_stack_based" \
	"Dmin" \
	"Dmin_Projective" \
	"Dmin_Planar" \
	"Dmin_Unconstrained" \
	"Dmin_Unconstrained_YS" \
	"Dmin_Unconstrained_FC" \
	"gen" \
	"gen_trees_alf" \
	"gen_trees_alr" \
	"gen_trees_auf" \
	"gen_trees_aur" \
	"gen_trees_rlf" \
	"gen_trees_rlr" \
	"gen_trees_ruf" \
	"gen_trees_rur" \
	"gen_arr_rand_proj" \
	"gen_arr_all_proj" \
	"properties" \
	"internal" \
	"internal_centre" \
	"internal_centroid" \
	"internal_sorting" \
	"internal_traversal" \
	)

# all tests available
all_DIRS=(\
	"construction" "construction" \
	"numeric/integer" "numeric/integer" \
	"numeric/rational" "numeric/rational" \
	"utilities/tree_isomorphism" "utilities/tree_isomorphism" \
	"internal/sorting" "internal/sorting" \
	"internal/traversal" "internal/traversal" \
	"internal/centre" "internal/centre" \
	"internal/centroid" "internal/centroid" \
	"properties" "properties" \
	"properties/exp-var-C/formula-no-Q-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-no-Q-no-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-Q" "properties/exp-var-C" \
	"properties/exp-var-C/trees" "properties/exp-var-C" \
	"properties/exp-var-C/forests" "properties/exp-var-C" \
	"properties/exp-var-D/general-formula" "properties/exp-var-D" \
	"linarr/E_2Cd" "linarr/E_2Cd" \
	"linarr/headedness" "linarr/headedness" \
	"linarr/MDD" "linarr/MDD" \
	"linarr/D" "linarr/D" \
	"linarr/D/Dmin/projective" "linarr/D/Dmin/projective" \
	"linarr/D/Dmin/planar" "linarr/D/Dmin/planar" \
	"linarr/D/Dmin/unconstrained" "linarr/D/Dmin/unconstrained" \
	"linarr/D/Dmin/unconstrained/YS" "linarr/D/Dmin/unconstrained/YS" \
	"linarr/D/Dmin/unconstrained/FC" "linarr/D/Dmin/unconstrained/FC" \
	"linarr/C/dyn-prog" "linarr/C" \
	"linarr/C/dyn-prog-list" "linarr/C" \
	"linarr/C/ladder" "linarr/C" \
	"linarr/C/ladder-list" "linarr/C" \
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	"linarr/k-level/1-MDD" "linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" "linarr/k-level/2-MDD" \
	"generate/trees/all_lab_free" "generate/trees/all_lab_free" \
	"generate/trees/all_lab_rooted" "generate/trees/all_lab_rooted" \
	"generate/trees/all_ulab_free" "generate/trees/all_ulab_free" \
	"generate/trees/all_ulab_rooted" "generate/trees/all_ulab_rooted" \
	"generate/trees/rand_lab_free" "generate/trees/rand_lab_free" \
	"generate/trees/rand_lab_rooted" "generate/trees/rand_lab_rooted" \
	"generate/trees/rand_ulab_free" "generate/trees/rand_ulab_free" \
	"generate/trees/rand_ulab_rooted" "generate/trees/rand_ulab_rooted" \
	"generate/arrangements/all_proj" "generate/arrangements/all_proj" \
	"generate/arrangements/rand_proj" "generate/arrangements/rand_proj" \
	)

# construction tests
construction_DIRS=(\
	"construction" "construction" \
	)

# numeric tests
numeric_DIRS=(\
	"numeric/integer" "numeric/integer" \
	"numeric/rational" "numeric/rational" \
	)

# utilities tests
utilities_DIRS=(\
	"utilities/tree_isomorphism" "utilities/tree_isomorphism" \
	)

utilities_isomorphism_DIRS=(\
	"utilities/tree_isomorphism" "utilities/tree_isomorphism" \
	)

# variance C tests
exp_var_C_DIRS=(\
	"properties/exp-var-C/formula-no-Q-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-no-Q-no-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-Q" "properties/exp-var-C" \
	"properties/exp-var-C/trees" "properties/exp-var-C" \
	"properties/exp-var-C/forests" "properties/exp-var-C" \
	)

# variance D tests
exp_var_D_DIRS=(\
	"properties/exp-var-C/formula-no-Q-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-no-Q-no-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-Q" "properties/exp-var-C" \
	"properties/exp-var-C/trees" "properties/exp-var-C" \
	"properties/exp-var-C/forests" "properties/exp-var-C" \
	)

# linear arrangements tests
linarr_DIRS=(\
	"linarr/E_2Cd" "linarr/E_2Cd" \
	"linarr/headedness" "linarr/headedness" \
	"linarr/MDD" "linarr/MDD" \
	"linarr/D" "linarr/D" \
	"linarr/D/Dmin/projective" "linarr/D/Dmin/projective" \
	"linarr/D/Dmin/planar" "linarr/D/Dmin/planar" \
	"linarr/D/Dmin/unconstrained" "linarr/D/Dmin/unconstrained" \
	"linarr/C/dyn-prog" "linarr/C" \
	"linarr/C/dyn-prog-list" "linarr/C" \
	"linarr/C/ladder" "linarr/C" \
	"linarr/C/ladder-list" "linarr/C" \
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	"linarr/k-level/1-MDD" "linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" "linarr/k-level/2-MDD" \
	)

# approximation of the number of crossings
linarr_E_2Cd_DIRS=(\
	"linarr/E_2Cd" "linarr/E_2Cd" \
	)

# linear arrangements (crossings) tests
linarr_C_DIRS=(\
	"linarr/C/dyn-prog" "linarr/C" \
	"linarr/C/dyn-prog-list" "linarr/C" \
	"linarr/C/ladder" "linarr/C" \
	"linarr/C/ladder-list" "linarr/C" \
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	)

linarr_C_stack_based_DIRS=(\
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	)

# Dmin (MLA) tests
Dmin_DIRS=(\
	"linarr/D/Dmin/projective" "linarr/D/Dmin/projective" \
	"linarr/D/Dmin/planar" "linarr/D/Dmin/planar" \
	"linarr/D/Dmin/unconstrained" "linarr/D/Dmin/unconstrained" \
	"linarr/D/Dmin/unconstrained/YS" "linarr/D/Dmin/unconstrained/YS" \
	"linarr/D/Dmin/unconstrained/FC" "linarr/D/Dmin/unconstrained/FC" \
	)

# Dmin Projective tests
Dmin_Projective_DIRS=(\
	"linarr/D/Dmin/projective" "linarr/D/Dmin/projective" \
	)

# Dmin Planar tests
Dmin_Planar_DIRS=(\
	"linarr/Dmin/planar" "linarr/Dmin/planar" \
	)

# Dmin Unconstrained tests -- all
Dmin_Unconstrained_DIRS=(\
	"linarr/Dmin/unconstrained" "linarr/Dmin/unconstrained" \
	)
# Dmin Unconstrained tests -- Yossi Shiloach
Dmin_Unconstrained_YS_DIRS=(\
	"linarr/Dmin/unconstrained/YS" "linarr/Dmin/unconstrained/YS" \
	)
# Dmin Unconstrained tests -- Fan Chung
Dmin_Unconstrained_FC_DIRS=(\
	"linarr/Dmin/unconstrained/FC" "linarr/Dmin/unconstrained/FC" \
	)

# generate tests
gen_DIRS=(\
	"generate/trees/all_lab_free" "generate/trees/all_lab_free" \
	"generate/trees/all_lab_rooted" "generate/trees/all_lab_rooted" \
	"generate/trees/all_ulab_free" "generate/trees/all_ulab_free" \
	"generate/trees/all_ulab_rooted" "generate/trees/all_ulab_rooted" \
	"generate/trees/rand_lab_free" "generate/trees/rand_lab_free" \
	"generate/trees/rand_lab_rooted" "generate/trees/rand_lab_rooted" \
	"generate/trees/rand_ulab_free" "generate/trees/rand_ulab_free" \
	"generate/trees/rand_ulab_rooted" "generate/trees/rand_ulab_rooted" \
	"generate/arrangements/all_proj" "generate/arrangements/all_proj" \
	"generate/arrangements/rand_proj" "generate/arrangements/rand_proj" \
	)

# generate (all labelled free) tests : alf
gen_trees_alf_DIRS=(\
	"generate/trees/all_lab_free" "generate/trees/all_lab_free" \
	)

# generate (all labelled rooted) tests : alr
gen_trees_alr_DIRS=(\
	"generate/trees/all_lab_rooted" "generate/trees/all_lab_rooted" \
	)

# generate (all unlabelled free) tests : auf
gen_trees_auf_DIRS=(\
	"generate/trees/all_ulab_free" "generate/trees/all_ulab_free" \
	)

# generate (all unlabelled rooted) tests : aur
gen_trees_aur_DIRS=(\
	"generate/trees/all_ulab_rooted" "generate/trees/all_ulab_rooted" \
	)

# generate (rand labelled free) tests : rlf
gen_trees_rlf_DIRS=(\
	"generate/trees/rand_lab_free" "generate/trees/rand_lab_free" \
	)

# generate (rand labelled rooted) tests : rlr
gen_trees_rlr_DIRS=(\
	"generate/trees/rand_lab_rooted" "generate/trees/rand_lab_rooted" \
	)

# generate (rand unlabelled rooted) tests : ruf
gen_trees_ruf_DIRS=(\
	"generate/trees/rand_ulab_free" "generate/trees/rand_ulab_free" \
	)

# generate (rand unlabelled rooted) tests : ruf
gen_trees_rur_DIRS=(\
	"generate/trees/rand_ulab_rooted" "generate/trees/rand_ulab_rooted" \
	)

# generate (all projective arrangements) tests
gen_arr_all_proj_DIRS=(\
	"generate/arrangements/all_proj" "generate/arrangements/all_proj" \
	)

# generate (rand projective arrangements) tests
gen_arr_rand_proj_DIRS=(\
	"generate/arrangements/rand_proj" "generate/arrangements/rand_proj" \
	)

# properties
properties_DIRS=(\
	"properties" "properties" \
	"properties/exp-var-C/formula-no-Q-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-no-Q-no-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-Q" "properties/exp-var-C" \
	"properties/exp-var-C/trees" "properties/exp-var-C" \
	"properties/exp-var-C/forests" "properties/exp-var-C" \
	"properties/exp-var-D/general-formula" "properties/exp-var-D" \
	)

# internal tests
internal_DIRS=(\
	"internal/sorting" "internal/sorting" \
	"internal/traversal" "internal/traversal" \
	"internal/centre" "internal/centre" \
	"internal/centroid" "internal/centroid" \
	)

# sorting tests
internal_sorting_DIRS=(\
	"internal/sorting" "internal/sorting" \
	)

# internal_centre tests
internal_centre_DIRS=(\
	"internal/centre" "internal/centre" \
	)

# internal_centre tests
internal_centroid_DIRS=(\
	"internal/centroid" "internal/centroid" \
	)

# internal_traversal tests
internal_traversal_DIRS=(\
	"internal/traversal" "internal/traversal" \
	)
