TEMPLATE = app
CONFIG += console c++17
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

# link against common
LIBS += -L../common/ -lcommon
PRE_TARGETDEPS += ../common/libcommon.a

isEmpty(ENVIR) {
    ENVIR = "HOME"
}

# configure home
equals(ENVIR, "HOME") {
    LAL_DIR = /home/lluis/Documents/projects/linear-arrangement-library
	THIS_DIR = /home/lluis/Documents/projects/LAL-tests/src/
}

# configure cluster
equals(ENVIR, "CLUSTER") {
    QMAKE_CXX = /home/soft/gcc-9.2.0/bin/g++
	QMAKE_LINK = /home/soft/gcc-9.2.0/bin/g++
	QMAKE_CXXFLAGS += -std=c++17

    LAL_DIR = /home/usuaris/lalemany/linear-arrangement-library
	THIS_DIR = /home/usuaris/lalemany/LAL-tests/src
}

CONFIG(debug, debug|release) {
    LIBS += -L$${LAL_DIR}/lal-debug/ -llaldebug
	PRE_TARGETDEPS += $${LAL_DIR}/lal-debug/liblaldebug.so
}
CONFIG(release, debug|release) {
    LIBS += -L$${LAL_DIR}/lal-release/ -llal
	PRE_TARGETDEPS += $${LAL_DIR}/lal-release/liblal.so
}

INCLUDEPATH += $$LAL_DIR/
INCLUDEPATH += $$THIS_DIR

DEPENDPATH += $$LAL_DIR/

# gmp library
LIBS += -lgmp
# OpenMP library
LIBS += -fopenmp

SOURCES += \
    C.cpp \
    C_list.cpp \
    D.cpp \
    Dmin_comparison.cpp \
    Dmin_planar.cpp \
    Dmin_projective.cpp \
    Dmin_unconstrained.cpp \
    flux.cpp \
    headedness.cpp \
    klevel.cpp \
        main.cpp \
		parse_keywords.cpp \
    predicted_C.cpp \
    syntree_classification.cpp

HEADERS += \
    exe_tests.hpp \
    linarr_brute_force_testing.hpp \
    n_crossings_m2.hpp \
    parse_keywords.hpp

DISTFILES +=
