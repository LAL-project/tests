TEMPLATE = app

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fPIC -fopenmp
QMAKE_CXXFLAGS_DEBUG += -O3 -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -UDEBUG -DNDEBUG -fstrict-aliasing

QMAKE_CXXFLAGS += -fopenmp

QMAKE_CXXFLAGS +=										\
    -Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
    -Wold-style-cast -Wrestrict -Wduplicated-cond		\
    -Wnon-virtual-dtor -Woverloaded-virtual -Wshadow

isEmpty(ENVIR) {
    ENVIR = "HOME"
}

# configure home
equals(ENVIR, "HOME") {
    LAL_DIR = /home/lluis/Documents/projects/linear-arrangement-library
}

# configure cluster
equals(ENVIR, "CLUSTER") {
    QMAKE_CXX = /home/soft/gcc-9.2.0/bin/g++
	QMAKE_LINK = /home/soft/gcc-9.2.0/bin/g++
	QMAKE_CXXFLAGS += -std=c++17

    LAL_DIR = /home/usuaris/lalemany/linear-arrangement-library
}

CONFIG(debug, debug|release) {
    LIBS += -L$${LAL_DIR}/lal-debug/ -llaldebug
	PRE_TARGETDEPS += $${LAL_DIR}/lal-debug/liblaldebug.so
}
CONFIG(release, debug|release) {
    LIBS += -L$${LAL_DIR}/lal-release/ -llal
	PRE_TARGETDEPS += $${LAL_DIR}/lal-release/liblal.so
}
INCLUDEPATH += $${LAL_DIR}/
DEPENDPATH += $${LAL_DIR}/

# gmp library
LIBS += -lgmp
# OpenMP library
LIBS += -fopenmp

SOURCES += \
    arrgmnt_validity_check.cpp \
	graphs/construct.cpp \
	graphs/construct_assert.cpp \
	graphs/construct_utils.cpp \
    generate/arr_all_proj.cpp \
    generate/arr_rand_proj.cpp \
    generate/tree_validity_check.cpp \
    generate/trees_alf.cpp \
    generate/trees_alr.cpp \
    generate/trees_auf.cpp \
    generate/trees_aur.cpp \
    generate/trees_rlf.cpp \
    generate/trees_rlr.cpp \
    generate/trees_ruf.cpp \
    generate/trees_rur.cpp \
    graphs/tree_type_classification.cpp \
    linarr/C.cpp \
    linarr/C_list.cpp \
    linarr/D.cpp \
    linarr/Dmin_comparison.cpp \
    linarr/Dmin_planar.cpp \
    linarr/Dmin_projective.cpp \
    linarr/Dmin_unconstrained.cpp \
    linarr/approx_exp_C.cpp \
    linarr/flux.cpp \
    linarr/headedness.cpp \
    linarr/klevel.cpp \
    linarr/syntree_classification.cpp \
	main.cpp \
    memory/graphs.cpp \
    memory/graphs_dgraph_copy.cpp \
    memory/graphs_dgraph_move.cpp \
    memory/graphs_ftree_copy.cpp \
    memory/graphs_ftree_move.cpp \
    memory/graphs_rtree_copy.cpp \
    memory/graphs_rtree_move.cpp \
    memory/graphs_ugraph_copy.cpp \
    memory/graphs_ugraph_copy_into_ftree.cpp \
    memory/graphs_ugraph_move.cpp \
    memory/graphs_ugraph_move_into_ftree.cpp \
    memory/numeric.cpp \
    memory/numeric_integer_copy.cpp \
    memory/numeric_integer_move.cpp \
    memory/numeric_integer_swap.cpp \
    memory/numeric_rational_copy.cpp \
    memory/numeric_rational_move.cpp \
    memory/numeric_rational_swap.cpp \
    numeric/integer.cpp \
    numeric/rational.cpp \
	parse_keywords.cpp \
	io_wrapper.cpp \
    properties/MHD_All_trees.cpp \
    properties/V_rla_C_brute_force_Q.cpp \
    properties/V_rla_C_brute_force_freqs.cpp \
    properties/expvar_C.cpp \
    properties/expvar_D.cpp \
    properties/general.cpp \
    test_utils.cpp \
    time.cpp \
    internal/centre.cpp \
    internal/centroid.cpp \
    internal/sorting.cpp \
    internal/traversal.cpp \
	utilities/tree_isomorphism.cpp

HEADERS += \
    arrgmnt_validity_check.hpp \
    generate/test_exhasutive_enumeration.hpp \
	graphs/exe_construction.hpp \
	definitions.hpp \
    generate/tree_validity_check.hpp \
    linarr/linarr_brute_force_testing.hpp \
    linarr/n_crossings_m2.hpp \
    memory/graphs.hpp \
    memory/numeric.hpp \
    parse_keywords.hpp \
	io_wrapper.hpp \
    properties/C_rla_brute_force_algorithms.hpp \
    std_utils.hpp \
    test_utils.hpp \
	exe_tests.hpp \
    time.hpp

DISTFILES += \
