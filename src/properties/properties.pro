TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

isEmpty(ENVIR) {
	ENVIR = "HOME"
}

isEmpty(ADDRESS_SANITIZER) {
    ADDRESS_SANITIZER = "NO"
}

QMAKE_CXXFLAGS += -std=c++17 -fPIC -fopenmp
QMAKE_CXXFLAGS +=			\
	-Wall					\
	-Wextra					\ # reasonable and standard
	-Wshadow				\ # warn if a variable declaration shadows one from
	                        \ # a parent context
	-Wnon-virtual-dtor		\ # warn if a class with virtual functions has
	                        \ # non-virtual destructors
	-Wold-style-cast		\ # warn for c-style casts
	-Wcast-align			\ # warn for potential performance problem casts
	-Wunused				\ # warn on anything being unused
	-Woverloaded-virtual	\ # warn if a virtual is overloaded (not overridden)
	-Wpedantic				\ # warn if non-standard C++ is used
	-Wconversion			\ # warn on type conversions that may lose data
	-Wsign-conversion		\ # warn on sign conversions
	-Wnull-dereference		\ # warn if a null dereference is detected
	-Wdouble-promotion		\ # warn if float is implicitly promoted to double
	-Wformat=2				\ # warn on security issues around functions that
	                        \ # format output
	-Wduplicated-cond		\ # warn if if-then-else chan has duplicated conditions
	-Wduplicated-branches	\ # warn if if-then-else have duplicated code
	-Wlogical-op			\ # warn about logical operations being used where
	                        \ # bitwise were probably prefered
	-Wuseless-cast			\ # warn if you perform a cast to the same type
	-Wrestrict

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG -Og
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -UDEBUG -DNDEBUG -O3

QMAKE_LFLAGS += -fPIC -O3 -Wl,-O3
QMAKE_LFLAGS_RELEASE += -DNDEBUG -UDEBUG
QMAKE_LFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG

equals(ADDRESS_SANITIZER, "YES") {
    # not to be used in combination with valgrind
	QMAKE_CXXFLAGS += -fsanitize=address
	LIBS += -lasan
}

LIBS += -L../common/ -lcommon
LIBS += -lgmp -fopenmp -lpthread

PRE_TARGETDEPS += ../common/libcommon.a

# configure home
equals(ENVIR, "HOME") {
	LAL_DIR = /home/lluis/Documents/projects/LAL-dev/latest/linear-arrangement-library
	THIS_DIR = /home/lluis/Documents/projects/LAL-dev/latest/tests/src
}

# configure cluster
equals(ENVIR, "CLUSTER") {
	QMAKE_CXX = /home/soft/gcc-11.3/bin/g++
	QMAKE_LINK = /home/soft/gcc-11.3/bin/g++

	LAL_DIR = /home/usuaris/lalemany/LAL-dev/latest/linear-arrangement-library
	THIS_DIR = /home/usuaris/lalemany/LAL-dev/latest/tests/src
}

CONFIG(debug, debug|release) {
	LIBS += -L$${LAL_DIR}/lal-debug -llal
	PRE_TARGETDEPS += $${LAL_DIR}/lal-debug/liblal.so
}
CONFIG(release, debug|release) {
	LIBS += -L$${LAL_DIR}/lal-release -llaloptimized
	PRE_TARGETDEPS += $${LAL_DIR}/lal-release/liblaloptimized.so
}

INCLUDEPATH += $${LAL_DIR}
INCLUDEPATH += $${THIS_DIR}

DEPENDPATH += $${LAL_DIR}

SOURCES += \
	MHD_All_trees.cpp \
	V_rla_C_brute_force_Q.cpp \
	V_rla_C_brute_force_freqs.cpp \
	bipartite_coloring.cpp \
	branchless_path_find.cpp \
	caterpillar_distance.cpp \
	diameter.cpp \
	expected_D_bipartite.cpp \
	expected_D_planar.cpp \
	expected_D_projective.cpp \
	expvar_C.cpp \
	expvar_D.cpp \
	general.cpp \
	main.cpp \
	parse_keywords.cpp \
	centre.cpp \
	centroid.cpp

HEADERS += \
	C_rla_brute_force_algorithms.hpp \
	exe_tests.hpp \
	parse_keywords.hpp
