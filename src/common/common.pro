TEMPLATE = lib
CONFIG += static
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
LIBS += -lgmp -fopenmp -lpthread

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

INCLUDEPATH += $${LAL_DIR}
INCLUDEPATH += $${THIS_DIR}

SOURCES += \
	io_wrapper.cpp \
	parse_keywords.cpp \
	test_utils.cpp \
	time.cpp

HEADERS += \
	definitions.hpp \
	io_wrapper.hpp \
	parse_header.hpp \
	parse_keywords.hpp \
	std_utils.hpp \
	test_utils.hpp \
	time.hpp \
	tree_validity_check.hpp
