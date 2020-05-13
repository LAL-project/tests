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
	"gen_alf" \
	"gen_ulf" \
	"gen_alf" \
	"gen_auf" \
	"gen_rlf" \
	"gen_rlr" \
	"gen_ruf" \
	"gen_rur" \
	"gen_rand_arr" \
	"utils" \
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
	"generate/all_lab_free" \
	"generate/all_ulab_free" \
	"generate/all_ulab_rooted" \
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
	"generate/rand_ulab_rooted" \
	"generate/rand_arrangements" \
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
	"generate/all_lab_free" \
	"generate/all_ulab_free" \
	"generate/all_ulab_rooted" \
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
	"generate/rand_ulab_rooted" \
	"generate/rand_arrangements" \
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
	"generate/all_lab_free" \
	"generate/all_ulab_free" \
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
	"generate/rand_ulab_rooted" \
	"generate/rand_arrangements" \
	)
gen_OUT_DIRS=(\
	"generate/all_lab_free" \
	"generate/all_ulab_free" \
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
	"generate/rand_ulab_rooted" \
	"generate/rand_arrangements" \
	)
# generate (all labelled free) tests : alf
gen_alf_IN_DIRS=(\
	"generate/all_lab_free" \
	)
gen_alf_OUT_DIRS=(\
	"generate/all_lab_free" \
	)
# generate (all unlabelled free) tests : auf
gen_auf_IN_DIRS=(\
	"generate/all_ulab_free" \
	)
gen_auf_OUT_DIRS=(\
	"generate/all_ulab_free" \
	)
# generate (rand labelled free) tests : rlf
gen_rlf_IN_DIRS=(\
	"generate/rand_lab_free" \
	)
gen_rlf_OUT_DIRS=(\
	"generate/rand_lab_free" \
	)
# generate (rand labelled rooted) tests : rlr
gen_rlr_IN_DIRS=(\
	"generate/rand_lab_rooted" \
	)
gen_rlr_OUT_DIRS=(\
	"generate/rand_lab_rooted" \
	)
# generate (rand unlabelled rooted) tests : ruf
gen_ruf_IN_DIRS=(\
	"generate/rand_ulab_free" \
	)
gen_ruf_OUT_DIRS=(\
	"generate/rand_ulab_free" \
	)
# generate (rand unlabelled rooted) tests : ruf
gen_rur_IN_DIRS=(\
	"generate/rand_ulab_rooted" \
	)
gen_rur_OUT_DIRS=(\
	"generate/rand_ulab_rooted" \
	)
# generate (rand unlabelled rooted) tests : ruf
gen_rand_arr_IN_DIRS=(\
	"generate/rand_arrangements" \
	)
gen_rand_arr_OUT_DIRS=(\
	"generate/rand_arrangements" \
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
