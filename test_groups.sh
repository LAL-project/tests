#!/bin/bash

# all groups -- used only as a means to test the correctness of
# the value of the parameter --exe-group=*.
groups_list=(\
	"all" \
	"small-tests" \
	\
	"generate" \
		"generate_trees_all" \
			"generate_trees_alf" \
			"generate_trees_alr" \
			"generate_trees_auf" \
			"generate_trees_aur" \
		"generate_trees_rand" \
			"generate_trees_rlf" \
			"generate_trees_rlr" \
			"generate_trees_ruf" \
			"generate_trees_rur" \
		"generate_arrangements_all" \
			"generate_arrangements_all_unc" \
			"generate_arrangements_all_bipartite" \
			"generate_arrangements_all_projective" \
			"generate_arrangements_all_planar" \
		"generate_arrangements_rand" \
			"generate_arrangements_rand_unc" \
			"generate_arrangements_rand_bipartite" \
			"generate_arrangements_rand_projective" \
			"generate_arrangements_rand_planar" \
	\
	"graphs" \
		"graphs_construction" \
		"graphs_tree_types" \
		"graphs_graph_literals" \
	\
	"detail" \
		"detail_sorting" \
		"detail_traversal" \
		"detail_avl" \
	\
	"io"
	"io_correctness"
	\
	"linarr" \
		"linarr_E_2Cd" \
		"linarr_D" \
		"linarr_chunking" \
			"linarr_chunking_Anderson" \
			"linarr_chunking_Macutek" \
		"linarr_C" \
			"linarr_C_bruteforce" \
				"linarr_C_bruteforce_C" \
				"linarr_C_bruteforce_C_list" \
				"linarr_C_bruteforce_C_upper_bound" \
				"linarr_C_bruteforce_C_list_upper_bound" \
				"linarr_C_bruteforce_C_list_upper_bound_list" \
			"linarr_C_dyn_prog" \
				"linarr_C_dyn_prog_C" \
				"linarr_C_dyn_prog_C_list" \
				"linarr_C_dyn_prog_C_upper_bound" \
				"linarr_C_dyn_prog_C_list_upper_bound" \
				"linarr_C_dyn_prog_C_list_upper_bound_list" \
			"linarr_C_ladder" \
				"linarr_C_ladder_C" \
				"linarr_C_ladder_C_list" \
				"linarr_C_ladder_C_upper_bound" \
				"linarr_C_ladder_C_list_upper_bound" \
				"linarr_C_ladder_C_list_upper_bound_list" \
			"linarr_C_stack_based" \
				"linarr_C_stack_based_C" \
				"linarr_C_stack_based_C_list" \
				"linarr_C_stack_based_C_upper_bound" \
				"linarr_C_stack_based_C_list_upper_bound" \
				"linarr_C_stack_based_C_list_upper_bound_list" \
		"linarr_Dmin" \
			"linarr_Dmin_Comparison" \
				"linarr_Dmin_Comparison_Plan_vs_Plan" \
				"linarr_Dmin_Comparison_Plan_vs_Unc" \
				"linarr_Dmin_Comparison_Unc_vs_Unc" \
			"linarr_Dmin_Projective" \
				"linarr_Dmin_Projective_AEF" \
				"linarr_Dmin_Projective_HS" \
			"linarr_Dmin_Planar" \
				"linarr_Dmin_Planar_AEF" \
				"linarr_Dmin_Planar_HS" \
			"linarr_Dmin_Unconstrained" \
				"linarr_Dmin_Unconstrained_YS" \
				"linarr_Dmin_Unconstrained_FC" \
		"linarr_DMax" \
			"linarr_DMax_Projective" \
				"linarr_DMax_Projective_AEF" \
			"linarr_DMax_Planar" \
				"linarr_DMax_Planar_AEF" \
			"linarr_DMax_Bipartite" \
				"linarr_DMax_Bipartite_AEF" \
			"linarr_DMax_1_eq_thistle" \
				"linarr_DMax_1_eq_thistle_AEF" \
			"linarr_DMax_1_le_thistle" \
				"linarr_DMax_1_le_thistle_AEF" \
		"linarr_syndepstruc_class" \
		"linarr_comp_dependency_flux" \
		"linarr_headedness" \
		"linarr_k_level" \
		"linarr_MDD" \
		"linarr_arrangement_validity" \
		"linarr_linear_arrangement" \
	\
	"memory" \
		"memory_graphs" \
		"memory_numeric" \
		"memory_detail" \
	\
	"numeric" \
	\
	"properties" \
		"properties_basic" \
		"properties_exp_var_C" \
			"properties_exp_var_C_no_Q" \
			"properties_exp_var_C_Q" \
			"properties_exp_var_C_trees" \
			"properties_exp_var_C_forests" \
		"properties_exp_var_D" \
			"properties_exp_D_projective" \
			"properties_exp_D_planar" \
			"properties_exp_var_D_unconstrained" \
		"properties_coloring" \
		"properties_centre" \
		"properties_centroid" \
		"properties_diameter" \
		"properties_maximum_subtrees" \
			"properties_maximum_subtrees_caterpillar" \
	\
	"utilities" \
		"utilities_isomorphism" \
		"utilities_isomorphism_small" \
		"utilities_isomorphism_big" \
	)
