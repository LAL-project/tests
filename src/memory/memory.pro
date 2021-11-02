TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fPIC -fopenmp -std=c++17
QMAKE_CXXFLAGS +=										\
	-Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
	-Wold-style-cast -Wrestrict -Wduplicated-cond		\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wshadow

QMAKE_CXXFLAGS_DEBUG += -O3 -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -UDEBUG -DNDEBUG -fstrict-aliasing

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
	graphs.cpp \
	graphs_dgraph_copy.cpp \
	graphs_dgraph_move.cpp \
	graphs_ftree_copy.cpp \
	graphs_ftree_move.cpp \
	graphs_rtree_copy.cpp \
	graphs_rtree_move.cpp \
	graphs_ugraph_copy.cpp \
	graphs_ugraph_copy_into_ftree.cpp \
	graphs_ugraph_move.cpp \
	graphs_ugraph_move_into_ftree.cpp \
	internal_data_array.cpp \
	main.cpp \
	numeric.cpp \
	numeric_integer_copy.cpp \
	numeric_integer_move.cpp \
	numeric_integer_swap.cpp \
	numeric_rational_copy.cpp \
	numeric_rational_move.cpp \
	numeric_rational_swap.cpp \
	parse_keywords.cpp

HEADERS += \
	exe_tests.hpp \
	graph_functions.hpp \
	graphs.hpp \
	internal_data_array_functions.hpp \
	numeric.hpp \
	numeric_functions.hpp \
	parse_keywords.hpp

DISTFILES +=
