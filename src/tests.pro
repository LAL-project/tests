TEMPLATE = app

CONFIG += c++1z console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG -fstrict-aliasing

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
    construction/construct.cpp \
    construction/construct_assert.cpp \
    construction/construct_utils.cpp \
    generate/arr_all_proj.cpp \
    generate/arr_rand_proj.cpp \
    generate/trees_alf.cpp \
    generate/trees_auf.cpp \
    generate/trees_aur.cpp \
    generate/trees_rlf.cpp \
    generate/trees_rlr.cpp \
    generate/trees_ruf.cpp \
    generate/trees_rur.cpp \
    linarr/C.cpp \
    linarr/C_list.cpp \
    linarr/D.cpp \
    linarr/Dmin_free.cpp \
    linarr/Dmin_rooted.cpp \
    linarr/approx_exp_C.cpp \
    linarr/headedness.cpp \
    linarr/klevel.cpp \
    linarr/syntree_classification.cpp \
	main.cpp \
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
    utils/centre.cpp \
    utils/sorting.cpp \
    utils/traversal.cpp

HEADERS += \
    class_hierarchy.hpp \
    construction/exe_construction.hpp \
	definitions.hpp \
    parse_keywords.hpp \
    exe_tests.hpp \
	io_wrapper.hpp \
    properties/C_rla_brute_force_algorithms.hpp \
    test_utils.hpp \
    time.hpp

DISTFILES += \
