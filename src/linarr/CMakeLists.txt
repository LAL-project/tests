cmake_minimum_required(VERSION 3.16)

message(STATUS "================")
message(STATUS "LAL linarr tests")
message(STATUS "================")

project(LAL_TESTS_DETAIL VERSION "${LAL_TESTS_VERSION}")
project(LAL_TESTS_DETAIL DESCRIPTION "Tests for LAL's interpreter")

set(LAL_TESTS_DETAIL_DIR "${LAL_TESTS_DIR}/src/linarr/*")
message(STATUS "    LAL_TESTS_DETAIL_DIR: ${LAL_TESTS_DETAIL_DIR}")

# **********************************************************************

function(configure_linarr executable)

	configure_test(${executable})

endfunction(configure_linarr)

# **********************************************************************
# MAKE EXECUTABLE

file(GLOB_RECURSE SOURCES ${LAL_TESTS_DETAIL_DIR})
add_executable(linarr ${SOURCES})

set_target_properties(linarr PROPERTIES VERSION "${LAL_TESTS_VERSION}")
set_target_properties(linarr PROPERTIES SOVERSION 1)
set_target_properties(linarr PROPERTIES LINKER_LANGUAGE CXX)

configure_linarr(linarr)
