#!/bin/bash

# all groups
groups_list=(\
	"all" \
	"construction" \
	"numeric" \
	"exp_var_C" \
	"exp_var_D" \
	"linarr" \
	"linarr_C" \
	"Dmin" \
	"gen" \
	"gen_trees_alf" \
	"gen_trees_auf" \
	"gen_trees_aur" \
	"gen_trees_rlf" \
	"gen_trees_rlr" \
	"gen_trees_ruf" \
	"gen_trees_rur" \
	"gen_arr_rand_proj" \
	"gen_arr_all_proj" \
	"utils" \
	"utils_centre" \
	)

# all tests available
all_IN_DIRS=(\
	"construction" \
	"numeric/integer" \
	"numeric/rational" \
	"utils/sorting" \
	"utils/traversal" \
	"utils/centre" \
	"properties" \
	"properties/exp-var-C/formula-no-Q-reuse" \
	"properties/exp-var-C/formula-no-Q-no-reuse" \
	"properties/exp-var-C/formula-Q" \
	"properties/exp-var-C/trees" \
	"properties/exp-var-C/forests" \
	"properties/exp-var-D/general-formula" \
	"linarr/E_2Cd" \
	"linarr/headedness" \
	"linarr/MDD" \
	"linarr/D" \
	"linarr/Dmin" \
	"linarr/C/dyn-prog" \
	"linarr/C/dyn-prog-list" \
	"linarr/C/ladder" \
	"linarr/C/ladder-list" \
	"linarr/C/stack-based" \
	"linarr/C/stack-based-list" \
	"linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" \
	"generate/trees/all_lab_free" \
	"generate/trees/all_ulab_free" \
	"generate/trees/all_ulab_rooted" \
	"generate/trees/rand_lab_free" \
	"generate/trees/rand_lab_rooted" \
	"generate/trees/rand_ulab_free" \
	"generate/trees/rand_ulab_rooted" \
	"generate/arrangements/all_proj" \
	"generate/arrangements/rand_proj" \
	)
all_OUT_DIRS=(\
	"construction" \
	"numeric/integer" \
	"numeric/rational" \
	"utils/sorting" \
	"utils/traversal" \
	"utils/centre" \
	"properties" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-D" \
	"linarr/E_2Cd" \
	"linarr/headedness" \
	"linarr/MDD" \
	"linarr/D" \
	"linarr/Dmin" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" \
	"generate/trees/all_lab_free" \
	"generate/trees/all_ulab_free" \
	"generate/trees/all_ulab_rooted" \
	"generate/trees/rand_lab_free" \
	"generate/trees/rand_lab_rooted" \
	"generate/trees/rand_ulab_free" \
	"generate/trees/rand_ulab_rooted" \
	"generate/arrangements/all_proj" \
	"generate/arrangements/rand_proj" \
	)

# construction tests
construction_IN_DIRS=(\
	"construction" \
	)
construction_OUT_DIRS=(\
	"construction" \
	)

# numeric tests
numeric_IN_DIRS=(\
	"numeric/integer" \
	"numeric/rational" \
	)
numeric_OUT_DIRS=(\
	"numeric/integer" \
	"numeric/rational" \
	)

# variance C tests
exp_var_C_IN_DIRS=(\
	"properties/exp-var-C/formula-no-Q-reuse" \
	"properties/exp-var-C/formula-no-Q-no-reuse" \
	"properties/exp-var-C/formula-Q" \
	"properties/exp-var-C/trees" \
	"properties/exp-var-C/forests" \
	)
exp_var_C_OUT_DIRS=(\
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	)

# variance D tests
exp_var_D_IN_DIRS=(\
	"properties/exp-var-C/formula-no-Q-reuse" \
	"properties/exp-var-C/formula-no-Q-no-reuse" \
	"properties/exp-var-C/formula-Q" \
	"properties/exp-var-C/trees" \
	"properties/exp-var-C/forests" \
	)
exp_var_D_OUT_DIRS=(\
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	"properties/exp-var-C" \
	)

# linear arrangements tests
linarr_IN_DIRS=(\
	"linarr/E_2Cd" \
	"linarr/headedness" \
	"linarr/MDD" \
	"linarr/D" \
	"linarr/C/dyn-prog" \
	"linarr/C/dyn-prog-list" \
	"linarr/C/ladder" \
	"linarr/C/ladder-list" \
	"linarr/C/stack-based" \
	"linarr/C/stack-based-list" \
	"linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" \
	)
linarr_OUT_DIRS=(\
	"linarr/E_2Cd" \
	"linarr/headedness" \
	"linarr/MDD" \
	"linarr/D" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" \
	)

# linear arrangements (crossings) tests
linarr_C_IN_DIRS=(\
	"linarr/C/dyn-prog" \
	"linarr/C/dyn-prog-list" \
	"linarr/C/ladder" \
	"linarr/C/ladder-list" \
	"linarr/C/stack-based" \
	"linarr/C/stack-based-list" \
	)
linarr_C_OUT_DIRS=(\
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	"linarr/C" \
	)

# Dmin (MLA) tests
Dmin_IN_DIRS=(\
	"linarr/Dmin"\
	)
Dmin_OUT_DIRS=(\
	"linarr/Dmin"\
	)

# generate tests
gen_IN_DIRS=(\
	"generate/trees/all_lab_free" \
	"generate/trees/all_ulab_free" \
	"generate/trees/rand_lab_free" \
	"generate/trees/rand_lab_rooted" \
	"generate/trees/rand_ulab_free" \
	"generate/trees/rand_ulab_rooted" \
	"generate/arrangements/all_proj" \
	"generate/arrangements/rand_proj" \
	)
gen_OUT_DIRS=(\
	"generate/trees/all_lab_free" \
	"generate/trees/all_ulab_free" \
	"generate/trees/rand_lab_free" \
	"generate/trees/rand_lab_rooted" \
	"generate/trees/rand_ulab_free" \
	"generate/trees/rand_ulab_rooted" \
	"generate/arrangements/all_proj" \
	"generate/arrangements/rand_proj" \
	)
# generate (all labelled free) tests : alf
gen_trees_alf_IN_DIRS=(\
	"generate/trees/all_lab_free" \
	)
gen_trees_alf_OUT_DIRS=(\
	"generate/trees/all_lab_free" \
	)
# generate (all unlabelled free) tests : auf
gen_trees_auf_IN_DIRS=(\
	"generate/trees/all_ulab_free" \
	)
gen_trees_auf_OUT_DIRS=(\
	"generate/trees/all_ulab_free" \
	)
# generate (all unlabelled rooted) tests : aur
gen_trees_aur_IN_DIRS=(\
	"generate/trees/all_ulab_rooted" \
	)
gen_trees_aur_OUT_DIRS=(\
	"generate/trees/all_ulab_rooted" \
	)
# generate (rand labelled free) tests : rlf
gen_trees_rlf_IN_DIRS=(\
	"generate/trees/rand_lab_free" \
	)
gen_trees_rlf_OUT_DIRS=(\
	"generate/trees/rand_lab_free" \
	)
# generate (rand labelled rooted) tests : rlr
gen_trees_rlr_IN_DIRS=(\
	"generate/trees/rand_lab_rooted" \
	)
gen_trees_rlr_OUT_DIRS=(\
	"generate/trees/rand_lab_rooted" \
	)
# generate (rand unlabelled rooted) tests : ruf
gen_trees_ruf_IN_DIRS=(\
	"generate/trees/rand_ulab_free" \
	)
gen_trees_ruf_OUT_DIRS=(\
	"generate/trees/rand_ulab_free" \
	)
# generate (rand unlabelled rooted) tests : ruf
gen_trees_rur_IN_DIRS=(\
	"generate/trees/rand_ulab_rooted" \
	)
gen_trees_rur_OUT_DIRS=(\
	"generate/trees/rand_ulab_rooted" \
	)
# generate (all projective arrangements) tests
gen_arr_all_proj_IN_DIRS=(\
	"generate/arrangements/all_proj" \
	)
gen_arr_all_proj_OUT_DIRS=(\
	"generate/arrangements/all_proj" \
	)
# generate (rand projective arrangements) tests
gen_arr_rand_proj_IN_DIRS=(\
	"generate/arrangements/rand_proj" \
	)
gen_arr_rand_proj_OUT_DIRS=(\
	"generate/arrangements/rand_proj" \
	)

# utils tests
utils_IN_DIRS=(\
	"utils/sorting" \
	"utils/traversal" \
	"utils/centre" \
	)
utils_OUT_DIRS=(\
	"utils/sorting" \
	"utils/traversal" \
	"utils/centre" \
	)

# utils_centre tests
utils_centre_IN_DIRS=(\
	"utils/centre" \
	)
utils_centre_OUT_DIRS=(\
	"utils/centre" \
	)
