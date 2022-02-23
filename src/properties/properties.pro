TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fPIC -fopenmp -std=c++17 -fsanitize=address
QMAKE_CXXFLAGS +=										\
	-Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
	-Wold-style-cast -Wrestrict -Wduplicated-cond		\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wshadow

QMAKE_CXXFLAGS_DEBUG += -O3 -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -UDEBUG -DNDEBUG -fstrict-aliasing

LIBS += -lasan
LIBS += -L../common/ -lcommon
LIBS += -lgmp -fopenmp -lpthread

PRE_TARGETDEPS += ../common/libcommon.a

isEmpty(ENVIR) {
	ENVIR = "HOME"
}

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
	MHD_All_trees.cpp \
	V_rla_C_brute_force_Q.cpp \
	V_rla_C_brute_force_freqs.cpp \
	expected_D_planar.cpp \
	expected_D_projective.cpp \
	expvar_C.cpp \
	expvar_D.cpp \
	general.cpp \
	main.cpp \
	parse_keywords.cpp

HEADERS += \
	C_rla_brute_force_algorithms.hpp \
	exe_tests.hpp \
	parse_keywords.hpp
