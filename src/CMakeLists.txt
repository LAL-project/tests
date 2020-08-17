cmake_minimum_required(VERSION 3.10.2)

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# **********************************************************************
# Custom functions
function(add_comp_flag flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG)
	if (CXX_HAS_FLAG)
		message(STATUS "    Adding flag ${flag}...")
		target_compile_options(tests PRIVATE ${flag})
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

# **********************************************************************
# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# **********************************************************************
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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Link against laldebug (debug compilation of lal)")
	set(LALLibrary "laldebug")
else()
	message(STATUS "Link against lal (release compilation of lal)")
	set(LALLibrary "lal")
endif()

# PROJECT

project(tests)
set(
	SOURCES
		construction/exe_construction.hpp
		construction/construct.cpp
		construction/construct_assert.cpp
		construction/construct_utils.cpp

		generate/tree_validity_check.hpp
		generate/arr_all_proj.cpp
		generate/arr_rand_proj.cpp
		generate/tree_validity_check.cpp
		generate/tree_alf.cpp
		generate/tree_alr.cpp
		generate/tree_auf.cpp
		generate/tree_aur.cpp
		generate/tree_rlf.cpp
		generate/tree_rlr.cpp
		generate/tree_ruf.cpp
		generate/tree_rur.cpp

		linarr/approx_exp_C.cpp
		linarr/C.cpp
		linarr/C_list.cpp
		linarr/D.cpp
		linarr/D_min_projective.cpp
		linarr/D_min_unconstrained.cpp
		linarr/headedness.cpp
		linarr/klevel.cpp
		linarr/syntree_classification.cpp

		numeric/integer.cpp
		numeric/rational.cpp

		properties/C_rla_brute_force_algorithms.hpp
		properties/expvar_C.cpp
		properties/expvar_D.cpp
		properties/general.cpp
		properties/MHD_All_trees.cpp
		properties/V_rla_C_brute_force_freqs.cpp
		properties/V_rla_C_brute_force_Q.cpp

		internal/centre.cpp
		internal/centroid.cpp
		internal/sorting.cpp
		internal/traversal.cpp

		arrgmnt_validity_check.hpp
		arrgmnt_validity_check.cpp

		definitions.hpp
		definitions.cpp

		exe_tests.hpp
		exe_tests.cpp

		io_wrapper.hpp
		io_wrapper.cpp

		parse_keywords.hpp
		parse_keywords.cpp

		test_utils.hpp
		test_utils.cpp

		time.hpp
		time.cpp

		tree_classification.hpp
		tree_classification.cpp
)

# **********************************************************************
# LIBRARIES
# Check if libraries are installed in the system.
# If some library does not exist, do not compile
# the executables that need them

# ***
# LAL
find_library(LAL_LIB ${LALLibrary})
if (LAL_LIB)
	message(STATUS "Found lal: ${LAL_LIB}")
else()
	message(FATAL_ERROR "Error: Linear Arrangement Library (${LAL_LIB}) library not found.")
endif()

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

# **********************************************************************
# COMPILATION FLAGS

# Wrestrict
add_comp_flag(-Wrestrict)

# OpenMP
target_link_libraries(tests ${OpenMP_CXX_LIBRARIES})

# Linear Arrangement Library
target_link_libraries(tests ${LALLibrary})
# GMP
target_link_libraries(tests gmp)

# Optimisation flags
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Adding flags for 'debug' mode")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	message(STATUS "Adding flags for 'release' mode")
	add_release_flag(-fstrict-aliasing)
endif()

# include path (required by the compiler to find the headers)
target_include_directories(tests PRIVATE ../tests)
