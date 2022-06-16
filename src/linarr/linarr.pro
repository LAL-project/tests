TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

isEmpty(ENVIR) {
    ENVIR = "HOME"
}

QMAKE_CXXFLAGS += -fPIC -fopenmp -std=c++17
QMAKE_CXXFLAGS +=										\
	-Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
	-Wold-style-cast -Wrestrict -Wduplicated-cond		\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wshadow

QMAKE_CXXFLAGS_DEBUG += -O3 -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -UDEBUG -DNDEBUG -fstrict-aliasing

equals(ENVIR, "CLUSTER") {
    QMAKE_CXXFLAGS += -fsanitize=address
	LIBS += -lasan
}

LIBS += -L../common/ -lcommon
LIBS += -lgmp -fopenmp -lpthread

PRE_TARGETDEPS += ../common/libcommon.a

# configure home
equals(ENVIR, "HOME") {
	LAL_DIR = /home/lluis/Documents/projects/LAL-dev/linear-arrangement-library
	THIS_DIR = /home/lluis/Documents/projects/LAL-dev/tests/src
}

# configure cluster
equals(ENVIR, "CLUSTER") {
	QMAKE_CXX = /home/soft/gcc-11.2.0/bin/g++
	QMAKE_LINK = /home/soft/gcc-11.2.0/bin/g++

	LAL_DIR = /home/usuaris/lalemany/LAL-dev/linear-arrangement-library
	THIS_DIR = /home/usuaris/lalemany/LAL-dev/tests/src
}

CONFIG(debug, debug|release) {
	LIBS += -L$${LAL_DIR}/lal-debug -llaldebug
	PRE_TARGETDEPS += $${LAL_DIR}/lal-debug/liblaldebug.so
}
CONFIG(release, debug|release) {
	LIBS += -L$${LAL_DIR}/lal-release -llal
	PRE_TARGETDEPS += $${LAL_DIR}/lal-release/liblal.so
}

INCLUDEPATH += $${LAL_DIR}
INCLUDEPATH += $${THIS_DIR}

DEPENDPATH += $${LAL_DIR}

SOURCES += \
	C.cpp \
	C_list.cpp \
	D.cpp \
	DMax_planar.cpp \
	DMax_projective.cpp \
	DMax_projective_all_roots.cpp \
	Dmin_comparison.cpp \
	Dmin_planar.cpp \
	Dmin_projective.cpp \
	Dmin_unconstrained.cpp \
	arrangement_validity.cpp \
	flux.cpp \
	headedness.cpp \
	klevel.cpp \
	linear_arrangement.cpp \
	main.cpp \
	parse_keywords.cpp \
	predicted_C.cpp \
	syntree_classification.cpp

HEADERS += \
	arrangement_check.hpp \
	exe_tests.hpp \
	linarr_brute_force_testing.hpp \
	n_crossings_m2.hpp \
	parse_keywords.hpp

DISTFILES +=
