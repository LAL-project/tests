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
	"graphs_tree_types" \
	"internal" \
	"internal_centre" \
	"internal_centroid" \
	"internal_sorting" \
	"internal_traversal" \
	"linarr" \
	"linarr_E_2Cd" \
	"linarr_C" \
	"linarr_C_brute_force" \
	"linarr_C_dyn_prog" \
	"linarr_C_ladder" \
	"linarr_C_stack_based" \
	"linarr_Dmin" \
	"linarr_Dmin_Comparison" \
	"linarr_Dmin_Projective" \
	"linarr_Dmin_Planar" \
	"linarr_Dmin_Unconstrained_YS" \
	"linarr_Dmin_Unconstrained_FC" \
	"linarr_syndepstruc_class" \
	"linarr_comp_depenendency_flux" \
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
