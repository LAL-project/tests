#!/bin/bash

# all groups -- used only as a means to test the correctness of
# the value of the parameter --exe-group=*.
groups_list=(\
	"all" \
	"construction" \
	"numeric" \
	"exp_var_C" \
	"exp_var_D" \
	"linarr" \
	"linarr_C" \
	"Dmin" \
	"Dmin_Projective" \
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
	"utils" \
	"utils_centre" \
	"utils_sorting" \
	)

# all tests available
all_DIRS=(\
	"construction" "construction" \
	"numeric/integer" "numeric/integer" \
	"numeric/rational" "numeric/rational" \
	"utils/sorting" "utils/sorting" \
	"utils/traversal" "utils/traversal" \
	"utils/centre" "utils/centre" \
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
	"linarr/Dmin/projective" "linarr/Dmin/projective" \
	"linarr/Dmin/unconstrained/YS" "linarr/Dmin/unconstrained" \
	"linarr/Dmin/unconstrained/FC" "linarr/Dmin/unconstrained" \
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
	"linarr/C/dyn-prog" "linarr/C" \
	"linarr/C/dyn-prog-list" "linarr/C" \
	"linarr/C/ladder" "linarr/C" \
	"linarr/C/ladder-list" "linarr/C" \
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	"linarr/k-level/1-MDD" "linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" "linarr/k-level/2-MDD" \
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

# Dmin (MLA) tests
Dmin_DIRS=(\
	"linarr/Dmin/projective" "linarr/Dmin/projective" \
	)

# Dmin Projective tests
Dmin_Projective_DIRS=(\
	"linarr/Dmin/projective" "linarr/Dmin/projective" \
	)

# Dmin Unconstrained tests -- all
Dmin_Unconstrained_DIRS=(\
	"linarr/Dmin/unconstrained/YS" "linarr/Dmin/unconstrained/YS" \
	)
# Dmin Unconstrained tests -- Yossi Shiloach
Dmin_Unconstrained_YS_DIRS=(\
	"linarr/Dmin/unconstrained/YS" "linarr/Dmin/unconstrained/" \
	)
# Dmin Unconstrained tests -- Fan Chung
Dmin_Unconstrained_YS_DIRS=(\
	"linarr/Dmin/unconstrained/FC" "linarr/Dmin/unconstrained/" \
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

# utils tests
utils_DIRS=(\
	"utils/sorting" "utils/sorting" \
	"utils/traversal" "utils/traversal" \
	"utils/centre" "utils/centre" \
	)

# sorting tests
utils_sorting_DIRS=(\
	"utils/sorting" "utils/sorting" \
	)

# utils_centre tests
utils_centre_DIRS=(\
	"utils/centre" "utils/centre" \
	)
