#!/bin/bash

# basic properties (size of Q, ...)
properties_basic_DIRS=(\
	"properties" "properties" \
	)

# variance C tests -- without Q
properties_exp_var_C_no_Q_DIRS=(\
	"properties/exp-var-C/formula-no_Q-reuse" "properties/exp-var-C" \
	"properties/exp-var-C/formula-no_Q-no-reuse" "properties/exp-var-C" \
	)

# variance C tests -- with Q
properties_exp_var_C_Q_DIRS=(\
	"properties/exp-var-C/formula-Q" "properties/exp-var-C" \
	)
	
# variance C tests -- trees
properties_exp_var_C_trees_DIRS=(\
	"properties/exp-var-C/trees" "properties/exp-var-C" \
	)

# variance C tests -- forests
properties_exp_var_C_forests_DIRS=(\
	"properties/exp-var-C/forests" "properties/exp-var-C" \
	)

# variance C tests
properties_exp_var_C_DIRS=(\
	"${properties_exp_var_C_no_Q_DIRS[@]}" \
	"${properties_exp_var_C_Q_DIRS[@]}" \
	"${properties_exp_var_C_trees_DIRS[@]}" \
	"${properties_exp_var_C_forests_DIRS[@]}" \
	)

# variance D tests
properties_exp_var_D_DIRS=(\
	"properties/exp-var-D/" "properties/exp-var-D/" \
	)

# properties
properties_DIRS=(\
	"${properties_basic_DIRS[@]}" \
	"${properties_exp_var_D_DIRS[@]}" \
	"${properties_exp_var_C_DIRS[@]}" \
	)
