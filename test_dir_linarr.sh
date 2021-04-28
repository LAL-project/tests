#!/bin/bash

# approximation of the number of crossings
linarr_E_2Cd_DIRS=(\
	"linarr/E_2Cd" "linarr/E_2Cd" \
	)

# sum of edge lengths
linarr_D_DIRS=(\
	"linarr/D" "linarr/D" \
	)

# Number of crossings tests
linarr_C_brute_force_C_DIRS=(\
	"linarr/C/brute_force" "linarr/C" \
	)
linarr_C_brute_force_C_list_DIRS=(\
	"linarr/C/brute_force-list" "linarr/C" \
	)
linarr_C_brute_force_C_upper_bound_DIRS=(\
	"linarr/C/brute_force-upper_bound" "linarr/C" \
	)
linarr_C_brute_force_C_list_upper_bound_DIRS=(\
	"linarr/C/brute_force-list_upper_bound" "linarr/C" \
	)
linarr_C_brute_force_C_list_upper_bound_list_DIRS=(\
	"linarr/C/brute_force-list_upper_bound_list" "linarr/C" \
	)
linarr_C_brute_force_DIRS=(\
	"${linarr_C_brute_force_C_DIRS[@]}" \
	"${linarr_C_brute_force_C_list_DIRS[@]}" \
	"${linarr_C_brute_force_C_upper_bound_DIRS[@]}" \
	"${linarr_C_brute_force_C_list_upper_bound_DIRS[@]}" \
	"${linarr_C_brute_force_C_list_upper_bound_list_DIRS[@]}" \
	)

linarr_C_dyn_prog_C_DIRS=(\
	"linarr/C/dyn-prog" "linarr/C" \
	)
linarr_C_dyn_prog_C_list_DIRS=(\
	"linarr/C/dyn-prog-list" "linarr/C" \
	)
linarr_C_dyn_prog_C_upper_bound_DIRS=(\
	"linarr/C/dyn-prog-upper_bound" "linarr/C" \
	)
linarr_C_dyn_prog_C_list_upper_bound_DIRS=(\
	"linarr/C/dyn-prog-list_upper_bound" "linarr/C" \
	)
linarr_C_dyn_prog_C_list_upper_bound_list_DIRS=(\
	"linarr/C/dyn-prog-list_upper_bound_list" "linarr/C" \
	)
linarr_C_dyn_prog_DIRS=(\
	"${linarr_C_dyn_prog_C_DIRS[@]}" \
	"${linarr_C_dyn_prog_C_list_DIRS[@]}" \
	"${linarr_C_dyn_prog_C_upper_bound_DIRS[@]}" \
	"${linarr_C_dyn_prog_C_list_upper_bound_DIRS[@]}" \
	"${linarr_C_dyn_prog_C_list_upper_bound_list_DIRS[@]}" \
	)

linarr_C_ladder_C_DIRS=(\
	"linarr/C/ladder" "linarr/C" \
	)
linarr_C_ladder_C_list_DIRS=(\
	"linarr/C/ladder-list" "linarr/C" \
	)
linarr_C_ladder_C_upper_bound_DIRS=(\
	"linarr/C/ladder-upper_bound" "linarr/C" \
	)
linarr_C_ladder_C_list_upper_bound_DIRS=(\
	"linarr/C/ladder-list_upper_bound" "linarr/C" \
	)
linarr_C_ladder_C_list_upper_bound_list_DIRS=(\
	"linarr/C/ladder-list_upper_bound_list" "linarr/C" \
	)
linarr_C_ladder_DIRS=(\
	"${linarr_C_ladder_C_DIRS[@]}" \
	"${linarr_C_ladder_C_list_DIRS[@]}" \
	"${linarr_C_ladder_C_upper_bound_DIRS[@]}" \
	"${linarr_C_ladder_C_list_upper_bound_DIRS[@]}" \
	"${linarr_C_ladder_C_list_upper_bound_list_DIRS[@]}" \
	)

linarr_C_stack_based_C_DIRS=(\
	"linarr/C/stack-based" "linarr/C" \
	)
linarr_C_stack_based_C_list_DIRS=(\
	"linarr/C/stack-based-list" "linarr/C" \
	)
linarr_C_stack_based_C_upper_bound_DIRS=(\
	"linarr/C/stack-based-upper_bound" "linarr/C" \
	)
linarr_C_stack_based_C_list_upper_bound_DIRS=(\
	"linarr/C/stack-based-list_upper_bound" "linarr/C" \
	)
linarr_C_stack_based_C_list_upper_bound_list_DIRS=(\
	"linarr/C/stack-based-list_upper_bound_list" "linarr/C" \
	)
linarr_C_stack_based_DIRS=(\
	"${linarr_C_stack_based_C_DIRS[@]}" \
	"${linarr_C_stack_based_C_list_DIRS[@]}" \
	"${linarr_C_stack_based_C_upper_bound_DIRS[@]}" \
	"${linarr_C_stack_based_C_list_upper_bound_DIRS[@]}" \
	"${linarr_C_stack_based_C_list_upper_bound_list_DIRS[@]}" \
	)

linarr_C_DIRS=(\
	"${linarr_C_dyn_prog_DIRS[@]}" \
	"${linarr_C_ladder_DIRS[@]}" \
	"${linarr_C_stack_based_DIRS[@]}" \
	)

# Dmin tests
linarr_Dmin_Projective_DIRS=(\
	"linarr/D/Dmin/projective" "linarr/D/Dmin/projective" \
	)
linarr_Dmin_Planar_DIRS=(\
	"linarr/D/Dmin/planar" "linarr/D/Dmin/planar" \
	)
linarr_Dmin_Comparison_DIRS=(\
	"linarr/D/Dmin/" "linarr/D/Dmin/" \
	)
linarr_Dmin_Unconstrained_YS_DIRS=(\
	"linarr/D/Dmin/unconstrained/YS" "linarr/D/Dmin/unconstrained/YS" \
	)
linarr_Dmin_Unconstrained_FC_DIRS=(\
	"linarr/D/Dmin/unconstrained/FC" "linarr/D/Dmin/unconstrained/FC" \
	)
linarr_Dmin_Unconstrained_DIRS=(\
	"${linarr_Dmin_Unconstrained_YS_DIRS[@]}" \
	"${linarr_Dmin_Unconstrained_FC_DIRS[@]}" \
	)
linarr_Dmin_DIRS=(\
	"${linarr_Dmin_Projective_DIRS[@]}" \
	"${linarr_Dmin_Planar_DIRS[@]}" \
	"${linarr_Dmin_Comparison_DIRS[@]}" \
	"${linarr_Dmin_Unconstrained_DIRS[@]}" \
	)

# Classification of syntactic dependency structures
linarr_syndepstruc_class_DIRS=(\
	"linarr/syntactic-dependency-structures" "linarr/syntactic-dependency-structures" \
	)

# Classification of dependency fluxes
linarr_comp_dependency_flux_DIRS=(\
	"linarr/dependency-flux" "linarr/dependency-flux" \
	)

# headedness
linarr_headedness_DIRS=(\
	"linarr/headedness" "linarr/headedness" \
	)

# k-level
linarr_k_level_DIRS=(\
	"linarr/k-level/1-MDD" "linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" "linarr/k-level/2-MDD" \
	)

# MDD
linarr_MDD_DIRS=(\
	"linarr/MDD" "linarr/MDD" \
	)

# linear arrangements tests
linarr_DIRS=(\
	"${linarr_E_2Cd_DIRS[@]}" \
	"${linarr_D_DIRS[@]}" \
	"${linarr_Dmin_DIRS[@]}" \
	"${linarr_C_DIRS[@]}" \
	"${linarr_syndepstruc_class_DIRS[@]}" \
	"${linarr_comp_dependency_flux_DIRS[@]}" \
	"${linarr_headedness_DIRS[@]}" \
	"${linarr_k_level_DIRS[@]}" \
	"${linarr_MDD_DIRS[@]}" \
	)
