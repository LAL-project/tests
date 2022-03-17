TEMPLATE = lib

CONFIG += static c++17
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
    LAL_DIR = /home/lluis/Documents/projects/LAL-DISTRIBUTION/21.07/linear-arrangement-library
	THIS_DIR = /home/lluis/Documents/projects/LAL-DISTRIBUTION/21.07/LAL-tests/src
}

# configure cluster
equals(ENVIR, "CLUSTER") {
    QMAKE_CXX = /home/soft/gcc-9.2.0/bin/g++
	QMAKE_LINK = /home/soft/gcc-9.2.0/bin/g++
	QMAKE_CXXFLAGS += -std=c++17

	LAL_DIR = /home/usuaris/lalemany/LAL-DISTRIBUTION/21.07/linear-arrangement-library
	THIS_DIR = /home/usuaris/lalemany/LAL-DISTRIBUTION/21.07/LAL-tests/src
}

INCLUDEPATH += $$LAL_DIR
INCLUDEPATH += $$THIS_DIR

SOURCES += \
    io_wrapper.cpp \
    parse_keywords.cpp \
    test_utils.cpp \
    time.cpp \
    tree_validity_check.cpp

HEADERS += \
    definitions.hpp \
    io_wrapper.hpp \
    parse_header.hpp \
    parse_keywords.hpp \
    std_utils.hpp \
    test_utils.hpp \
    time.hpp \
    tree_validity_check.hpp
