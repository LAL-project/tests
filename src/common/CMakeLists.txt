cmake_minimum_required(VERSION 3.16)

message(STATUS "=============================================")
message(STATUS "Configuring utility functions for LAL's tests")
message(STATUS "=============================================")

project(LAL_TESTS_COMMON VERSION "${LAL_TESTS_VERSION}")
project(LAL_TESTS_COMMON DESCRIPTION "Tests for LAL's tests")

set(LAL_TESTS_COMMON_DIR "${LAL_TESTS_DIR}/src/common/*")
message(STATUS "    LAL_TESTS_COMMON_DIR:  ${LAL_TESTS_COMMON_DIR}")

# **********************************************************************

set(TargetStringLibrary "${LAL_TESTS_LIB_NAME}")

add_library(${TargetStringLibrary} SHARED "")

set_target_properties(${TargetStringLibrary} PROPERTIES VERSION "${LAL_TESTS_VERSION}")
set_target_properties(${TargetStringLibrary} PROPERTIES SOVERSION 1)
set_target_properties(${TargetStringLibrary} PROPERTIES LINKER_LANGUAGE CXX)

file(GLOB_RECURSE SOURCES ${LAL_TESTS_COMMON_DIR})
target_sources(${TargetStringLibrary} PRIVATE ${SOURCES})

# **********************************************************************

configure_object(${TargetStringLibrary})
