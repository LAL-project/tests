#!/bin/bash

# all groups -- used only as a means to test the correctness of
# the value of the parameter --exe-group=*.
groups_list=(\
	"all" \
	"generate" \
	"generate_trees_alf" \
	"generate_trees_alr" \
	"generate_trees_auf" \
	"generate_trees_aur" \
	"generate_trees_rlf" \
	"generate_trees_rlr" \
	"generate_trees_ruf" \
	"generate_trees_rur" \
	"generate_arr_rand_proj" \
	"generate_arr_all_proj" \
	"graphs" \
	"graphs_construction" \
	"graphs_tree_type" \
	"internal" \
	"internal_centre" \
	"internal_centroid" \
	"internal_sorting" \
	"internal_traversal" \
	"linarr" \
	"linarr_E_2Cd" \
	"linarr_C" \
	"linarr_C_dyn_prog" \
	"linarr_C_ladder" \
	"linarr_C_stack_based" \
	"linarr_Dmin" \
	"linarr_Dmin_Comparison" \
	"linarr_Dmin_Projective" \
	"linarr_Dmin_Planar" \
	"linarr_Dmin_Unconstrained_YS" \
	"linarr_Dmin_Unconstrained_FC" \
	"memory" \
	"numeric" \
	"properties" \
	"properties_basic" \
	"properties_exp_var_C" \
	"properties_exp_var_C_no_Q" \
	"properties_exp_var_D" \
	"utilities" \
	"utilities_isomorphism" \
	"utilities_isomorphism_small" \
	"utilities_isomorphism_big" \
	)

# all tests available
all_DIRS=(\
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
	"graphs/construction" "graphs/construction" \
	"graphs/tree_type" "graphs/tree_type" \
	"internal/sorting" "internal/sorting" \
	"internal/traversal" "internal/traversal" \
	"internal/centre" "internal/centre" \
	"internal/centroid" "internal/centroid" \
	"linarr/E_2Cd" "linarr/E_2Cd" \
	"linarr/D" "linarr/D" \
	"linarr/D/Dmin/projective" "linarr/D/Dmin/projective" \
	"linarr/D/Dmin/planar" "linarr/D/Dmin/planar" \
	"linarr/D/Dmin/unconstrained/YS" "linarr/D/Dmin/unconstrained/YS" \
	"linarr/D/Dmin/unconstrained/FC" "linarr/D/Dmin/unconstrained/FC" \
	"linarr/D/Dmin/" "linarr/D/Dmin/" \
	"linarr/C/dyn-prog" "linarr/C" \
	"linarr/C/dyn-prog-list" "linarr/C" \
	"linarr/C/ladder" "linarr/C" \
	"linarr/C/ladder-list" "linarr/C" \
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	"linarr/headedness" "linarr/headedness" \
	"linarr/k-level/1-MDD" "linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" "linarr/k-level/2-MDD" \
	"linarr/MDD" "linarr/MDD" \
	"numeric/integer" "numeric/integer" \
	"numeric/rational" "numeric/rational" \
	"properties" "properties" \
	"properties/exp-var-C/formula-no-Q-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-no-Q-no-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-Q" "properties/exp-var-C" \
	"properties/exp-var-C/trees" "properties/exp-var-C" \
	"properties/exp-var-C/forests" "properties/exp-var-C" \
	"properties/exp-var-D/general-formula" "properties/exp-var-D" \
	"utilities/tree_isomorphism" "utilities/tree_isomorphism" \
	)

source test_dir_generate.sh
source test_dir_graphs.sh
source test_dir_internal.sh
source test_dir_linarr.sh
source test_dir_memory.sh
source test_dir_numeric.sh
source test_dir_properties.sh
source test_dir_utilities.sh
