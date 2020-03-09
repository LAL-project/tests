TEMPLATE = app

CONFIG += c++1z console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG
QMAKE_CXXFLAGS += -fopenmp

QMAKE_CXXFLAGS +=										\
    -Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
    -Wold-style-cast -Wrestrict -Wduplicated-cond		\
    -Wnon-virtual-dtor -Woverloaded-virtual -Wshadow

# lal library
LAL_DIR = /home/lluis/Documents/projects/linear-arrangement-library
CONFIG(debug, debug|release) {
    LIBS += -L$${LAL_DIR}/lal-debug/ -llaldebug
    PRE_TARGETDEPS += $${LAL_DIR}/lal-debug/liblaldebug.a
}
CONFIG(release, debug|release) {
    LIBS += -L$${LAL_DIR}/lal-release/ -llal
    PRE_TARGETDEPS += $${LAL_DIR}/lal-release/liblal.a
}
INCLUDEPATH += $${LAL_DIR}/
DEPENDPATH += $${LAL_DIR}/

# gmp library
LIBS += -lgmp
# OpenMP library
LIBS += -fopenmp

SOURCES += \
    class_hierarchy.cpp \
    construction/exe_construction.cpp \
    construction/exe_construction_assert.cpp \
    construction/exe_construction_utils.cpp \
    generate/exe_gen_trees_alf.cpp \
    generate/exe_gen_trees_auf.cpp \
    generate/exe_gen_trees_aur.cpp \
    generate/exe_gen_trees_rlf.cpp \
    generate/exe_gen_trees_rlr.cpp \
    generate/exe_gen_trees_ruf.cpp \
    linarr/exe_linarr_C.cpp \
    linarr/exe_linarr_C_list.cpp \
    linarr/exe_linarr_D.cpp \
    linarr/exe_linarr_approx_exp_C.cpp \
    linarr/exe_linarr_headedness.cpp \
    linarr/exe_linarr_klevel.cpp \
    linarr/exe_linarr_syn_dep_tree_type.cpp \
	main.cpp \
    numeric/exe_rational.cpp \
	parse_keywords.cpp \
	io_wrapper.cpp \
    properties/V_rla_C_brute_force_Q.cpp \
    properties/V_rla_C_brute_force_freqs.cpp \
    properties/exe_properties_MHD_All_trees.cpp \
    numeric/exe_integer.cpp \
    properties/exe_properties_general.cpp \
    properties/exe_properties_expvar_C.cpp \
    properties/exe_properties_expvar_D.cpp \
    time.cpp \
    utils/exe_utils_sorting.cpp \
    utils/exe_utils_traversal.cpp

HEADERS += \
    class_hierarchy.hpp \
    construction/exe_construction.hpp \
	definitions.hpp \
    parse_keywords.hpp \
    exe_tests.hpp \
	io_wrapper.hpp \
    properties/C_rla_brute_force_algorithms.hpp \
    time.hpp

DISTFILES += \
