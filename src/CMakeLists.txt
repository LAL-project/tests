cmake_minimum_required(VERSION 3.10.2)

project(LAL_TESTS)

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

# includes to lal
set(LAL_DIR ../../linear-arrangement-library)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Link against laldebug (debug compilation of lal)")
	set(LAL_LIB_DIR ${LAL_DIR}/lal-debug)
	set(LAL_LIB_NAME laldebug)
else()
	message(STATUS "Link against lal (release compilation of lal)")
	set(LAL_LIB_DIR ${LAL_DIR}/lal-release)
	set(LAL_LIB_NAME lal)
endif()
message(STATUS "    LAL_LIB_DIR: ${LAL_LIB_DIR}")
message(STATUS "    LAL_LIB_NAME: ${LAL_LIB_NAME}")

link_directories(${LAL_LIB_DIR})

# **********************************************************************
# PROJECT

add_executable(
	tests
		generate/arr_all_proj.cpp
		generate/arr_rand_proj.cpp
		generate/trees_alf.cpp
		generate/trees_alr.cpp
		generate/trees_auf.cpp
		generate/trees_aur.cpp
		generate/trees_rlf.cpp
		generate/trees_rlr.cpp
		generate/trees_ruf.cpp
		generate/trees_rur.cpp
		generate/tree_validity_check.cpp
		generate/tree_validity_check.hpp
	
		graphs/construct.cpp
		graphs/construct_assert.cpp
		graphs/construct_utils.cpp
		graphs/exe_construction.hpp
		graphs/tre_type_classification.cpp

		internal/centre.cpp
		internal/centroid.cpp
		internal/sorting.cpp
		internal/traversal.cpp

		linarr/approx_exp_C.cpp
		linarr/C.cpp
		linarr/C_list.cpp
		linarr/D.cpp
		linarr/Dmin_comparison.cpp
		linarr/Dmin_planar.cpp
		linarr/Dmin_projective.cpp
		linarr/Dmin_unconstrained.cpp
		linarr/headedness.cpp
		linarr/klevel.cpp
		linarr/linarr_brute_force_testing.cpp
		linarr/syntree_classification.cpp

		memory/graphs.cpp
		memory/numeric.cpp

		numeric/integer.cpp
		numeric/rational.cpp

		properties/C_rla_brute_force_algorithms.hpp
		properties/expvar_C.cpp
		properties/expvar_D.cpp
		properties/general.cpp
		properties/MHD_All_trees.cpp
		properties/V_rla_C_brute_force_freqs.cpp
		properties/V_rla_C_brute_force_Q.cpp

		utilities/tree_isomorphism.cpp

		arrgmnt_validity_check.hpp
		arrgmnt_validity_check.cpp
		definitions.hpp
		exe_tests.hpp
		exe_tests.cpp
		io_wrapper.cpp
		io_wrapper.hpp
		main.cpp
		parse_keywords.cpp
		parse_keywords.hpp
		test_utils.cpp
		test_utils.hpp
		time.cpp
		time.hpp
)

# **********************************************************************
# LIBRARIES

# ----------------------
# Add external libraries

# self includes
target_include_directories(tests PRIVATE .)

# lal includes
target_include_directories(tests PRIVATE ${LAL_DIR})
# lal linking
target_link_libraries(tests ${LAL_LIB_NAME})

# ----------------------
# Add system libraries

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

