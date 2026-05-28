cmake_minimum_required(VERSION 3.16)

message(STATUS "====================")
message(STATUS "LAL properties tests")
message(STATUS "====================")

project(LAL_TESTS_DETAIL VERSION "${LAL_TESTS_VERSION}")
project(LAL_TESTS_DETAIL DESCRIPTION "Tests for LAL's interpreter")

set(LAL_TESTS_DETAIL_DIR "${LAL_TESTS_DIR}/src/properties/*")
message(STATUS "    LAL_TESTS_DETAIL_DIR: ${LAL_TESTS_DETAIL_DIR}")

# **********************************************************************

find_library(NAUTY_LIB nauty)
if (NAUTY_LIB)
	message(STATUS "    Found nauty: ${NAUTY_LIB}")
	set(AUTOMATIC_NAUTY 1)
else()
	set(AUTOMATIC_NAUTY 0)
	if (NOT DEFINED NAUTY_DIR)
		message(FATAL_ERROR "    Error: nauty library not found and no paths were provided.")
		message(FATAL_ERROR "    Indicate path to nauty library with:")
		message(FATAL_ERROR "        -DNAUTY_DIR")
	else()
		set(NAUTY_LIB_DIR ${NAUTY_DIR}/lib)
		set(NAUTY_INC_DIR ${NAUTY_DIR}/)
		message(STATUS "    NAUTY_LIB_DIR: ${NAUTY_LIB_DIR}")
		message(STATUS "    NAUTY_INC_DIR: ${NAUTY_INC_DIR}")
	endif()
endif()

# **********************************************************************

function(configure_properties executable)

	# nauty
	if ("${AUTOMATIC_NAUTY}" STREQUAL "1")
		target_link_libraries(properties nauty)
	else()
		target_link_libraries(properties ${NAUTY_LIB_DIR}/libnauty.a)
		target_include_directories(properties PUBLIC ${NAUTY_INC_DIR})
	endif()

	configure_test(${executable})

endfunction(configure_properties)

# **********************************************************************
# MAKE EXECUTABLE

file(GLOB_RECURSE SOURCES ${LAL_TESTS_DETAIL_DIR})
add_executable(properties ${SOURCES})

set_target_properties(properties PROPERTIES VERSION "${LAL_TESTS_VERSION}")
set_target_properties(properties PROPERTIES SOVERSION 1)
set_target_properties(properties PROPERTIES LINKER_LANGUAGE CXX)

configure_properties(properties)

