cmake_minimum_required(VERSION 3.10.2)

project(LAL_TESTS)

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# ******************************************************************************
# Custom functions
function(add_comp_flag flag)
	check_cxx_compiler_flag(${flag} HAS_FLAG)
	if (HAS_FLAG)
		message(STATUS "    Adding flag ${flag}...")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_comp_flag)

function(add_debug_flag flag)
	check_cxx_compiler_flag(${flag} HAS_FLAG)
	if (HAS_FLAG)
		message(STATUS "    Adding flag ${flag}...")
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_debug_flag)

function(add_release_flag flag)
	check_cxx_compiler_flag(${flag} HAS_FLAG)
	if (HAS_FLAG)
		message(STATUS "    Adding flag ${flag}...")
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_release_flag)

# ******************************************************************************
# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ******************************************************************************
# Set default build type
# (following the guide in https://blog.kitware.com/cmake-and-the-default-build-type/)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(
		CMAKE_BUILD_TYPE "${default_build_type}"
		CACHE STRING "Choose the type of build."
		FORCE
	)
	# Set the possible values of build type for cmake-gui
	set_property(
		CACHE CMAKE_BUILD_TYPE
		PROPERTY
			STRINGS "Debug" "Release"
	)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Looking for the Linear Arrangement Library
if (NOT DEFINED LAL_DEV_DIR)
	message(STATUS "LAL's development root directory was not given.")
	message(STATUS "    Note that even if the library is installed in the system, these tests")
	message(STATUS "    use 'private' headers of the library, which are not distributed.")
	message(STATUS "    Please, rerun 'cmake' the location of LAL's development root directory:")
	message(STATUS "        -DLAL_DEV_DIR=/path/to/the/library")
	message(FATAL_ERROR "Please, read the last few lines and follow the instructions given.")
else()
	if (NOT DEFINED LAL_DEV_DIR)
		message(STATUS "Path to the root directory of the linear arrangement library not given.")
		message(STATUS "    Use -DLAL_DIR=/path/to/the/library")
		message(STATUS "Rerun 'cmake' with the appropriate arguments")
		message(FATAL_ERROR "Please, read the last few lines and follow the instructions given.")
	else()
		if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
			message(STATUS "Link against laldebug (debug compilation of lal)")
			set(LAL_LIB_DIR ${LAL_DEV_DIR}/lal-debug)
			set(LAL_LIB_NAME laldebug)
		else()
			message(STATUS "Link against lal (release compilation of lal)")
			set(LAL_LIB_DIR ${LAL_DEV_DIR}/lal-release)
			set(LAL_LIB_NAME lal)
		endif()
		message(STATUS "    LAL_LIB_DIR:  ${LAL_LIB_DIR}")
		message(STATUS "    LAL_LIB_NAME: ${LAL_LIB_NAME}")
	endif()
endif()

# ----------------------
# Find system libraries

# ***
# GMP
find_library(GMP_LIB gmp)
if (GMP_LIB)
	message(STATUS "Found gmp: ${GMP_LIB}")
else()
	message(FATAL_ERROR "Error: gmp library not found.")
endif()

# ******
# OpenMP
find_package(OpenMP REQUIRED)

# ******************************************************************************
# PROJECT

add_library(
	common STATIC
		common/definitions.hpp
		common/io_wrapper.hpp
		common/parse_header.hpp
		common/parse_keywords.hpp
		common/std_utils.hpp
		common/test_utils.hpp
		common/time.hpp
		common/tree_validity_check.hpp
		
		common/io_wrapper.cpp
		common/parse_keywords.cpp
		common/test_utils.cpp
		common/time.cpp
		common/tree_validity_check.cpp
)

add_executable(
	tests
		tests/main.cpp
)

add_executable(
	generate
		generate/exe_tests.hpp
		generate/parse_keywords.hpp
		generate/test_exhaustive_enumeration.hpp
		
		generate/arr_all.cpp
		generate/arr_all_planar.cpp
		generate/arr_all_projective.cpp
		generate/arr_rand.cpp
		generate/arr_rand_planar.cpp
		generate/arr_rand_projective.cpp
		generate/main.cpp
		generate/parse_keywords.cpp
		generate/trees_alf.cpp
		generate/trees_alr.cpp
		generate/trees_auf.cpp
		generate/trees_aur.cpp
		generate/trees_rlf.cpp
		generate/trees_rlr.cpp
		generate/trees_ruf.cpp
		generate/trees_rur.cpp
)

add_executable(
	graphs
		graphs/exe_construction.hpp
		graphs/exe_tests.hpp
		graphs/parse_keywords.hpp
		
		graphs/construct.cpp
		graphs/construct_assert.cpp
		graphs/construct_utils.cpp
		graphs/main.cpp
		graphs/parse_keywords.cpp
		graphs/tree_type_classification.cpp
)

add_executable(
	internal
		internal/exe_tests.hpp
		internal/parse_keywords.hpp
		
		internal/centre.cpp
		internal/centroid.cpp
		internal/main.cpp
		internal/parse_keywords.cpp
		internal/sorting.cpp
		internal/traversal.cpp
)

add_executable(
	io
		io/exe_tests.hpp
		io/parse_keywords.hpp
		
		io/correctness.cpp
		io/main.cpp
		io/parse_keywords.cpp
)

add_executable(
	linarr
		linarr/exe_tests.hpp
		linarr/linarr_brute_force_testing.hpp
		linarr/n_crossings_m2.hpp
		linarr/parse_keywords.hpp
		
		linarr/arrangement_validity.cpp
		linarr/C.cpp
		linarr/C_list.cpp
		linarr/D.cpp
		linarr/Dmin_comparison.cpp
		linarr/Dmin_planar.cpp
		linarr/Dmin_projective.cpp
		linarr/Dmin_unconstrained.cpp
		linarr/flux.cpp
		linarr/headedness.cpp
		linarr/klevel.cpp
		linarr/main.cpp
		linarr/parse_keywords.cpp
		linarr/predicted_C.cpp
		linarr/syntree_classification.cpp
)

add_executable(
	memory
		memory/exe_tests.hpp
		memory/graph_functions.hpp
		memory/graphs.hpp
		memory/internal_data_array_functions.hpp
		memory/numeric.hpp
		memory/numeric_functions.hpp
		memory/parse_keywords.hpp
		
		memory/graphs.cpp
		memory/graphs_dgraph_copy.cpp
		memory/graphs_dgraph_move.cpp
		memory/graphs_ftree_copy.cpp
		memory/graphs_ftree_move.cpp
		memory/graphs_rtree_copy.cpp
		memory/graphs_rtree_move.cpp
		memory/graphs_ugraph_copy.cpp
		memory/graphs_ugraph_copy_into_ftree.cpp
		memory/graphs_ugraph_move.cpp
		memory/graphs_ugraph_move_into_ftree.cpp
		memory/internal_data_array.cpp
		memory/main.cpp
		memory/numeric.cpp
		memory/numeric_integer_copy.cpp
		memory/numeric_integer_move.cpp
		memory/numeric_integer_swap.cpp
		memory/numeric_rational_copy.cpp
		memory/numeric_rational_move.cpp
		memory/numeric_rational_swap.cpp
		memory/parse_keywords.cpp
)

add_executable(
	numeric
		numeric/exe_tests.hpp
		numeric/parse_keywords.hpp
		
		numeric/integer.cpp
		numeric/main.cpp
		numeric/parse_keywords.cpp
		numeric/rational.cpp
)

add_executable(
	properties
		properties/C_rla_brute_force_algorithms.hpp
		properties/exe_tests.hpp
		properties/parse_keywords.hpp
		
		properties/expected_D_planar.cpp
		properties/expected_D_projective.cpp
		properties/expvar_C.cpp
		properties/expvar_D.cpp
		properties/general.cpp
		properties/main.cpp
		properties/MHD_All_trees.cpp
		properties/parse_keywords.cpp
		properties/V_rla_C_brute_force_freqs.cpp
		properties/V_rla_C_brute_force_Q.cpp
)

add_executable(
	utilities
		utilities/exe_tests.hpp
		utilities/parse_keywords.hpp
		
		utilities/main.cpp
		utilities/parse_keywords.cpp
		utilities/tree_isomorphism.cpp
)

# ******************************************************************************
# DEPENDENCIES

# common
add_dependencies(tests		common)
add_dependencies(generate	common)
add_dependencies(graphs		common)
add_dependencies(internal	common)
add_dependencies(io			common)
add_dependencies(linarr		common)
add_dependencies(memory		common)
add_dependencies(numeric	common)
add_dependencies(properties	common)
add_dependencies(utilities	common)

# ----------------------
# Add external libraries

# self includes
target_include_directories(common		PUBLIC .)
target_include_directories(generate		PUBLIC .)
target_include_directories(graphs		PUBLIC .)
target_include_directories(internal		PUBLIC .)
target_include_directories(io			PUBLIC .)
target_include_directories(linarr		PUBLIC .)
target_include_directories(memory		PUBLIC .)
target_include_directories(numeric		PUBLIC .)
target_include_directories(properties	PUBLIC .)
target_include_directories(tests		PUBLIC .)
target_include_directories(utilities	PUBLIC .)

# common linking
target_link_libraries(generate		${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(graphs		${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(internal		${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(io			${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(linarr		${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(memory		${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(numeric		${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(properties	${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(tests			${CMAKE_BINARY_DIR}/libcommon.a)
target_link_libraries(utilities 	${CMAKE_BINARY_DIR}/libcommon.a)

# LAL
# lal includes
target_include_directories(common		PUBLIC ${LAL_DEV_DIR})
target_include_directories(generate		PUBLIC ${LAL_DEV_DIR})
target_include_directories(graphs		PUBLIC ${LAL_DEV_DIR})
target_include_directories(internal		PUBLIC ${LAL_DEV_DIR})
target_include_directories(io			PUBLIC ${LAL_DEV_DIR})
target_include_directories(linarr		PUBLIC ${LAL_DEV_DIR})
target_include_directories(memory		PUBLIC ${LAL_DEV_DIR})
target_include_directories(numeric		PUBLIC ${LAL_DEV_DIR})
target_include_directories(properties	PUBLIC ${LAL_DEV_DIR})
target_include_directories(tests		PUBLIC ${LAL_DEV_DIR})
target_include_directories(utilities	PUBLIC ${LAL_DEV_DIR})
# lal linking
target_link_libraries(generate		${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(graphs		${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(internal		${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(io			${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(linarr		${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(memory		${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(numeric		${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(properties	${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(tests			${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
target_link_libraries(utilities 	${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)

# ******************************************************************************
# COMPILATION FLAGS

# OpenMP
target_link_libraries(generate		${OpenMP_CXX_LIBRARIES})
target_link_libraries(graphs		${OpenMP_CXX_LIBRARIES})
target_link_libraries(internal		${OpenMP_CXX_LIBRARIES})
target_link_libraries(io			${OpenMP_CXX_LIBRARIES})
target_link_libraries(linarr		${OpenMP_CXX_LIBRARIES})
target_link_libraries(memory		${OpenMP_CXX_LIBRARIES})
target_link_libraries(numeric		${OpenMP_CXX_LIBRARIES})
target_link_libraries(properties	${OpenMP_CXX_LIBRARIES})
target_link_libraries(tests			${OpenMP_CXX_LIBRARIES})
target_link_libraries(utilities		${OpenMP_CXX_LIBRARIES})

# GMP
target_link_libraries(generate		gmp)
target_link_libraries(graphs		gmp)
target_link_libraries(internal		gmp)
target_link_libraries(io			gmp)
target_link_libraries(linarr		gmp)
target_link_libraries(memory		gmp)
target_link_libraries(numeric		gmp)
target_link_libraries(properties	gmp)
target_link_libraries(tests			gmp)
target_link_libraries(utilities		gmp)

# Optimisation and warning flags
add_comp_flag(-Wrestrict)
add_comp_flag(-Wall)
add_comp_flag(-Wextra)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_debug_flag(-O3)
	add_debug_flag(-DDEBUG)
	add_debug_flag(-D_GLIBCXX_DEBUG)
else()
	# these two are added by default
	#add_release_flag(-O3)
	#add_release_flag(-DNDEBUG)
	add_release_flag(-UDEBUG)
	add_release_flag(-fstrict-aliasing)
endif()

