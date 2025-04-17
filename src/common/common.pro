TEMPLATE = lib
CONFIG += static
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++23

LAL_VERSION = latest

isEmpty(ENVIR) {
	ENVIR = "HOME"
}

isEmpty(ENVIR) {
    ENVIR = "HOME"
}

isEmpty(ADDRESS_SANITIZER) {
    ADDRESS_SANITIZER = "NO"
}

DEFINES += LAL_REGISTER_BIBLIOGRAPHY

QMAKE_CXXFLAGS += -std=c++23 -fPIC -fopenmp
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

LIBS += -lgmp -fopenmp -lpthread

# configure home
equals(ENVIR, "HOME") {
	LAL_DIR = $$(HOME)/Documents/projects/LAL-dev/$${LAL_VERSION}/linear-arrangement-library
	THIS_DIR = $$(HOME)/Documents/projects/LAL-dev/$${LAL_VERSION}/tests/src
}

# configure cluster
equals(ENVIR, "CLUSTER") {
	QMAKE_CXX = /home/soft/gcc-11.3/bin/g++
	QMAKE_LINK = /home/soft/gcc-11.3/bin/g++

	LAL_DIR = $$(HOME)/LAL-dev/$${LAL_VERSION}/linear-arrangement-library
	THIS_DIR = $$(HOME)/LAL-dev/$${LAL_VERSION}/tests/src
}

INCLUDEPATH += $${LAL_DIR}
INCLUDEPATH += $${THIS_DIR}

SOURCES += \
	io_wrapper.cpp \
	parse_header.cpp \
	parse_keywords.cpp \
	test_utils.cpp

HEADERS += \
	definitions.hpp \
	io_wrapper.hpp \
	parse_header.hpp \
	parse_keywords.hpp \
	std_utils.hpp \
	test_utils.hpp \
	time.hpp \
	tree_validity_check.hpp
