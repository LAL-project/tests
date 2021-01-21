#!/bin/bash

# linear arrangements tests
linarr_DIRS=(\
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
	"linarr/syntactic-dependency-structures" "linarr/syntactic-dependency-structures" \
	"linarr/headedness" "linarr/headedness" \
	"linarr/k-level/1-MDD" "linarr/k-level/1-MDD" \
	"linarr/k-level/2-MDD" "linarr/k-level/2-MDD" \
	"linarr/MDD" "linarr/MDD" \
	)

# approximation of the number of crossings
linarr_E_2Cd_DIRS=(\
	"linarr/E_2Cd" "linarr/E_2Cd" \
	)

# Number of crossings tests
linarr_C_DIRS=(\
	"linarr/C/dyn-prog" "linarr/C" \
	"linarr/C/dyn-prog-list" "linarr/C" \
	"linarr/C/ladder" "linarr/C" \
	"linarr/C/ladder-list" "linarr/C" \
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	)
linarr_C_dyn_prog_DIRS=(\
	"linarr/C/dyn-prog" "linarr/C" \
	"linarr/C/dyn-prog-list" "linarr/C" \
	)
linarr_C_ladder_DIRS=(\
	"linarr/C/ladder" "linarr/C" \
	"linarr/C/ladder-list" "linarr/C" \
	)
linarr_C_stack_based_DIRS=(\
	"linarr/C/stack-based" "linarr/C" \
	"linarr/C/stack-based-list" "linarr/C" \
	)

# Dmin tests
linarr_Dmin_DIRS=(\
	"linarr/D/Dmin/projective" "linarr/D/Dmin/projective" \
	"linarr/D/Dmin/planar" "linarr/D/Dmin/planar" \
	"linarr/D/Dmin/unconstrained/YS" "linarr/D/Dmin/unconstrained/YS" \
	"linarr/D/Dmin/unconstrained/FC" "linarr/D/Dmin/unconstrained/FC" \
	"linarr/D/Dmin/" "linarr/D/Dmin/" \
	)
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

# Classification of syntactic dependency structures
linarr_syndepstruc_class_DIRS=(\
	"linarr/syntactic-dependency-structures" "linarr/syntactic-dependency-structures" \
	)
