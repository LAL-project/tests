#!/bin/bash

# all groups
groups_list=(\
	"all" \
	"numeric" \
	"exp_var_C" \
	"exp_var_D" \
	"linarr" \
	"linarr_C" \
	"generate" \
	"generate_alf" \
	"generate_ulf" \
	"generate_alf" \
	"generate_auf" \
	"generate_rlf" \
	"generate_rlr" \
	"generate_ruf" \
	"utils" \
	)

# all tests available
all_IN_DIRS=(\
	"numeric/integer" \
	"numeric/rational" \
	"construction" \
	"utils/sorting" \
	"utils/traversal" \
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
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
	)
all_OUT_DIRS=(\
	"numeric/integer" \
	"numeric/rational" \
	"construction" \
	"utils/sorting" \
	"utils/traversal" \
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
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
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

# generate tests
generate_IN_DIRS=(\
	"generate/all_lab_free" \
	"generate/all_ulab_free" \
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
	)
generate_OUT_DIRS=(\
	"generate/all_lab_free" \
	"generate/all_ulab_free" \
	"generate/rand_lab_free" \
	"generate/rand_lab_rooted" \
	"generate/rand_ulab_free" \
	)
# generate (all labelled free) tests : alf
generate_alf_IN_DIRS=(\
	"generate/all_lab_free" \
	)
generate_alf_OUT_DIRS=(\
	"generate/all_lab_free" \
	)
# generate (all unlabelled free) tests : auf
generate_auf_IN_DIRS=(\
	"generate/all_ulab_free" \
	)
generate_auf_OUT_DIRS=(\
	"generate/all_ulab_free" \
	)
# generate (rand labelled free) tests : rlf
generate_rlf_IN_DIRS=(\
	"generate/rand_lab_free" \
	)
generate_rlf_OUT_DIRS=(\
	"generate/rand_lab_free" \
	)
# generate (rand labelled rooted) tests : rlr
generate_rlr_IN_DIRS=(\
	"generate/rand_lab_rooted" \
	)
generate_rlr_OUT_DIRS=(\
	"generate/rand_lab_rooted" \
	)
# generate (rand labelled rooted) tests : ruf
generate_ruf_IN_DIRS=(\
	"generate/rand_lab_free" \
	)
generate_ruf_OUT_DIRS=(\
	"generate/rand_lab_free" \
	)

# utils tests
utils_IN_DIRS=(\
	"utils/sorting" \
	"utils/traversal" \
	)
utils_OUT_DIRS=(\
	"utils/sorting" \
	"utils/traversal" \
	)
