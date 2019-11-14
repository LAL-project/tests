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
    -Wnon-virtual-dtor -Woverloaded-virtual

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
    construction/exe_construction.cpp \
    linarr/exe_linarr_compute_headedness.cpp \
    linarr/exe_linarr_syn_dep_tree_type.cpp \
	main.cpp \
	parse_keywords.cpp \
	io_wrapper.cpp \
    properties/exe_properties_MHD_All_trees.cpp \
    numeric/exe_integer.cpp \
    numeric/exe_rational_integer.cpp \
    numeric/exe_rational_rational.cpp \
    properties/exe_properties_general.cpp \
    properties/exe_properties_expvar_C.cpp \
    properties/exe_properties_expvar_D.cpp \
    time.cpp \
    linarr/exe_linarr_compute_D.cpp \
    linarr/exe_linarr_compute_C.cpp \
    linarr/exe_linarr_compute_C_list.cpp \
    generate/exe_gen_trees.cpp \
    properties/exe_properties_approx_exp_C.cpp \
    utils/exe_utils_sorting.cpp \
    utils/exe_utils_traversal.cpp

HEADERS += \
	definitions.hpp \
    parse_keywords.hpp \
    exe_tests.hpp \
	io_wrapper.hpp \
    time.hpp

DISTFILES += \
