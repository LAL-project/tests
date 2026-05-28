cmake_minimum_required(VERSION 3.10.2)

project(LAL_TESTS)

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)

set(LAL_TESTS_MAJOR 99)
set(LAL_TESTS_MINOR 99)
set(LAL_TESTS_PATCH 99)
set(LAL_TESTS_VERSION "${LAL_TESTS_MAJOR}.${LAL_TESTS_MINOR}.${LAL_TESTS_PATCH}")

set(LAL_TESTS_DIR ${CMAKE_SOURCE_DIR})
message(STATUS "LAL_TESTS_DIR: ${LAL_TESTS_DIR}")

# ******************************************************************************
# Custom functions

function(add_compile_flag thing flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	if (CXX_HAS_FLAG_${flag})
		target_compile_options(${thing} PRIVATE ${flag})
	endif()
endfunction(add_compile_flag)

function(add_link_flag thing flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	if (CXX_HAS_FLAG_${flag})
		target_link_options(${thing} PRIVATE ${flag})
	endif()
endfunction(add_link_flag)

function(define_symbol thing symbol)
	target_compile_options(${thing} PRIVATE ${symbol})
endfunction(define_symbol)

function(add_sanitize_option thing option)
	target_compile_options(${thing} PRIVATE -fsanitize=${option})
	target_link_options(${thing} PRIVATE -fsanitize=${option})
endfunction(add_sanitize_option)

function(configure_object thing)
	# thing's properties
	set_target_properties(${thing} PROPERTIES SOVERSION 1)
	set_target_properties(${thing} PROPERTIES LINKER_LANGUAGE CXX)

	# Warning flags

	add_compile_flag(${thing} -Wall)
	add_compile_flag(${thing} -Wextra)
	add_compile_flag(${thing} -Wshadow)
	add_compile_flag(${thing} -Wnon-virtual-dtor)
	add_compile_flag(${thing} -Wold-style-cast)
	add_compile_flag(${thing} -Wcast-align)
	add_compile_flag(${thing} -Wunused)
	add_compile_flag(${thing} -Woverloaded-virtual)
	add_compile_flag(${thing} -Wpedantic)
	add_compile_flag(${thing} -Wconversion)
	add_compile_flag(${thing} -Wsign-conversion)
	add_compile_flag(${thing} -Wnull-dereference)
	add_compile_flag(${thing} -Wdouble-promotion)
	#add_compile_flag(${thing} -Wformat=2)
	add_compile_flag(${thing} -Wduplicated-cond)
	add_compile_flag(${thing} -Wduplicated-branches)
	add_compile_flag(${thing} -Wlogical-op)
	add_compile_flag(${thing} -Wuseless-cast)
	add_compile_flag(${thing} -Wrestrict)
	add_compile_flag(${thing} -Wswitch-unreachable)
	add_compile_flag(${thing} -DLAL_DEBUG_AVL)
	add_compile_flag(${thing} -DLAL_REGISTER_BIBLIOGRAPHY)

	# Compile-mode dependent flags

	# OpenMP
	add_compile_flag(${thing} ${OpenMP_CXX_FLAGS})

	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		add_compile_flag(${thing} -g)
		add_compile_flag(${thing} -Og)
		define_symbol(${thing} -DDEBUG)
		define_symbol(${thing} -D_GLIBCXX_DEBUG)
	else()
		define_symbol(${thing} -UDEBUG)
		add_compile_flag(${thing} -fstrict-aliasing)
	endif()

	if (ENABLE_ASAN)
		add_compile_flag(${thing} -fno-omit-frame-pointer)
		add_sanitize_option(${thing} address)
		add_sanitize_option(${thing} undefined)
		add_sanitize_option(${thing} leak)
		add_sanitize_option(${thing} unreachable)
		add_sanitize_option(${thing} null)
	endif()

	target_include_directories(${thing} PRIVATE ${LAL_DEV_DIR})
	target_include_directories(${thing} PRIVATE ${LAL_TESTS_DIR}/src)

	target_link_libraries(${thing} ${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
	target_link_libraries(${thing} gmp)
	target_link_libraries(${thing} pthread)
	target_link_libraries(${thing} ${OpenMP_CXX_LIBRARIES})

endfunction(configure_object)

function(configure_test executable)

	add_dependencies(${executable} ${LAL_TESTS_LIB_NAME})
	target_link_libraries(${executable} ${LAL_TESTS_COMMON_LIB_DIR}/lib${LAL_TESTS_LIB_NAME}.so)

	configure_object(${executable})

	add_test(NAME ${executable} COMMAND ${executable})

endfunction(configure_test)

# ******************************************************************************
# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ******************************************************************************
# Set default build type

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
get_filename_component(BUILD_TYPE_DIR "${CMAKE_BINARY_DIR}" NAME)

# Looking for the Linear Arrangement Library
if (NOT DEFINED LAL_DEV_DIR)
	message(STATUS "LAL's development root directory was not given.")
	message(STATUS "    Note that even if the library is installed in the system, these tests")
	message(STATUS "    use 'private' headers of the library, which are not distributed.")
	message(STATUS "    Please, rerun 'cmake' the location of LAL's development root directory:")
	message(STATUS "        -DLAL_DEV_DIR=/path/to/the/library")
	message(STATUS "    Make sure it is an absolute path!")
	message(FATAL_ERROR "Please, read the last few lines and follow the instructions given.")
else()
	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		message(STATUS "Link against laldebug (debug compilation of lal)")
		set(LAL_LIB_DIR ${LAL_DEV_DIR}/lal-debug)
		set(LAL_LIB_NAME lal)
	else()
		message(STATUS "Link against lal (release compilation of lal)")
		set(LAL_LIB_DIR ${LAL_DEV_DIR}/lal-release)
		set(LAL_LIB_NAME laloptimized)
	endif()
	message(STATUS "    LAL_LIB_DIR:  ${LAL_LIB_DIR}")
	message(STATUS "    LAL_LIB_NAME: ${LAL_LIB_NAME}")
endif()

set(LAL_TESTS_LIB_NAME "common")

set(LAL_TESTS_COMMON_LIB_DIR ${LAL_TESTS_DIR}/${BUILD_TYPE_DIR}/src/common)

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

# ----------------------------------------------------------------------
# Compile projects

add_subdirectory(src)
