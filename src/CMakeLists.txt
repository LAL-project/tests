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
add_executable(tests "\
construction/exe_construction.cpp;\
generate/exe_gen_trees_alf.cpp;\
generate/exe_gen_trees_auf.cpp;\
generate/exe_gen_trees_rlf.cpp;\
generate/exe_gen_trees_rlr.cpp;\
generate/exe_gen_trees_ruf.cpp;\
linarr/exe_linarr_approx_exp_C.cpp;\
linarr/exe_linarr_C.cpp;\
linarr/exe_linarr_C_list.cpp;\
linarr/exe_linarr_D.cpp;\
linarr/exe_linarr_headedness.cpp;\
linarr/exe_linarr_klevel.cpp;\
linarr/exe_linarr_syn_dep_tree_type.cpp;\
numeric/exe_integer.cpp;\
numeric/exe_rational_integer.cpp;\
numeric/exe_rational_rational.cpp;\
properties/exe_properties_expvar_C.cpp;\
properties/exe_properties_expvar_D.cpp;\
properties/exe_properties_general.cpp;\
properties/exe_properties_MHD_All_trees.cpp;\
utils/exe_utils_sorting.cpp;\
utils/exe_utils_traversal.cpp;\
io_wrapper.cpp;\
main.cpp;\
parse_keywords.cpp;\
time.cpp\
")

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
