#!/bin/bash

# properties_centre tests
properties_branchless_paths_DIRS=(\
	"properties/branchless_paths" "properties/branchless_paths" \
	)

# properties_coloring tests
properties_coloring_DIRS=(\
	"properties/coloring" "properties/coloring" \
	)

# properties_orbits tests
properties_orbits_DIRS=(\
	"properties/orbits" "properties/orbits" \
	)

# properties_centre tests
properties_centre_DIRS=(\
	"properties/centre" "properties/centre" \
	)

# maximum subtrees
properties_connected_components_DIRS=(\
	"properties/connected_components" "properties/connected_components" \
	)

# properties_centre tests
properties_centroid_DIRS=(\
	"properties/centroid" "properties/centroid" \
	)

# properties_centre tests
properties_diameter_DIRS=(\
	"properties/diameter" "properties/diameter" \
	)

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

# moments of D tests

properties_exp_D_projective_DIRS=(\
	"properties/expected_D/projective" "properties/expected_D/projective"
	)

properties_exp_D_planar_DIRS=(\
	"properties/expected_D/planar" "properties/expected_D/planar"
	)

properties_exp_D_bipartite_DIRS=(\
	"properties/expected_D/bipartite" "properties/expected_D/bipartite"
	)

properties_exp_var_D_unconstrained_DIRS=(\
	"properties/exp-var-D/general-formula" "properties/exp-var-D/" \
	)

properties_exp_var_D_DIRS=(\
	"${properties_exp_D_projective_DIRS[@]}" \
	"${properties_exp_D_planar_DIRS[@]}" \
	"${properties_exp_D_bipartite_DIRS[@]}" \
	"${properties_exp_var_D_unconstrained_DIRS[@]}" \
	)

# maximum spanning caterpillars
properties_maximum_subtrees_caterpillar_DIRS=(\
	"properties/max_subtrees/caterpillar/" "properties/max_subtrees/caterpillar/" \
	)

# maximum subtrees
properties_maximum_subtrees_DIRS=(\
	"${properties_maximum_subtrees_caterpillars_DIRS[@]}" \
	)

# properties
properties_DIRS=(\
	"${properties_basic_DIRS[@]}" \
	"${properties_exp_var_D_DIRS[@]}" \
	"${properties_exp_var_C_DIRS[@]}" \
	"${properties_branchless_paths_DIRS[@]}" \
	"${properties_orbits_DIRS[@]}" \
	"${properties_coloring_DIRS[@]}" \
	"${properties_centre_DIRS[@]}" \
	"${properties_centroid_DIRS[@]}" \
	"${properties_connected_components_DIRS[@]}" \
	"${properties_diameter_DIRS[@]}" \
	"${properties_maximum_subtrees_DIRS[@]}" \
	)
