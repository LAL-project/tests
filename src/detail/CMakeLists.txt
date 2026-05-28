cmake_minimum_required(VERSION 3.16)

message(STATUS "================")
message(STATUS "LAL detail tests")
message(STATUS "================")

project(LAL_TESTS_DETAIL VERSION "${LAL_TESTS_VERSION}")
project(LAL_TESTS_DETAIL DESCRIPTION "Tests for LAL's interpreter")

set(LAL_TESTS_DETAIL_DIR "${LAL_TESTS_DIR}/src/detail/*")
message(STATUS "    LAL_TESTS_DETAIL_DIR: ${LAL_TESTS_DETAIL_DIR}")

# **********************************************************************

function(configure_detail executable)

	configure_test(${executable})

endfunction(configure_detail)

# **********************************************************************
# MAKE EXECUTABLE

file(GLOB_RECURSE SOURCES ${LAL_TESTS_DETAIL_DIR})
add_executable(detail ${SOURCES})

set_target_properties(detail PROPERTIES VERSION "${LAL_TESTS_VERSION}")
set_target_properties(detail PROPERTIES SOVERSION 1)
set_target_properties(detail PROPERTIES LINKER_LANGUAGE CXX)

configure_detail(detail)
