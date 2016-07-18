#
#	@author: Maurik Holtrop <maurik@physics.unh.edu>
#	@date: July 14, 2016
#
# Based on the CMakeLists.txt file from Omar Moreno for hps-dst
#
# Specify the minimum CMake version required.  If the cmake version 
# minimum is not met, cmake will fail.
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
cmake_policy(SET CMP0005 NEW)

if( CMAKE_VERSION VERSION_GREATER 3.0)
    cmake_policy(SET CMP0042 NEW)
endif()

# Set the name of the project
project(HPS_ANALYSIS)

# Set the project version
set(HPS_ANALYSIS_VERSION_MAJOR 0)
set(HPS_ANALYSIS_VERSION_MINOR 1)

# Offer the user the choice of overriding the installation directories

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH  "Installation directory for header files")

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 
        SET(CMAKE_INSTALL_PREFIX ${HPS_ANALYSIS_SOURCE_DIR} CACHE PATH "HPS_Analysis install prefix, default is project top dir." FORCE) 
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 

message("The installation prefix is set to:  ${CMAKE_INSTALL_PREFIX} ")   
 
# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()


# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT

if( NOT DEFINED ENV{ROOTSYS} ) 
    message( FATAL_ERROR  "You need to setup ROOT first. ")
else()
    message("Using ROOT from $ENV{ROOTSYS} ")
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED)

message("Got the FindROOT.cmake from: ${ROOT_DIR} ")
message("We have ROOT_USE_FILE: ${ROOT_USE_FILE} ")

#---Overwrite some of the ROOT_DEFINITIONS since these are way too pedantic.
set(ROOT_DEFINITIONS "")
message("ROOT_DEFINITIONS = ${ROOT_DEFINITIONS} ")

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

message("ROOT_DEFINITIONS = ${ROOT_DEFINITIONS}")
message("ROOT_CXX_FLAGS = ${ROOT_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add the hps-dst include directories.

set(HPS_DST_ROOT_DIR "${HPS_ANALYSIS_SOURCE_DIR}/hps-dst" CACHE PATH "Location of the hps-dst installation to use.")
include_directories("${HPS_DST_ROOT_DIR}/include/dst")
include_directories("${HPS_DST_ROOT_DIR}/include/hps_event")
include_directories("${HPS_DST_ROOT_DIR}/include/utils")

set(HPS_DST_LIB_DIR "${HPS_DST_ROOT_DIR}/lib" CACHE PATH "Location of the libraries for hps-dst.")


# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${INSTALL_LIB_DIR}")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${INSTALL_LIB_DIR}" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
   message("CMAKE_INSTALL_RPATH set to ${CMAKE_INSTALL_RPATH} ")

ENDIF("${isSystemDir}" STREQUAL "-1")


include_directories(${CMAKE_SOURCE_DIR}/BaseAna/BaseAna)
add_subdirectory(BaseAna/BaseAna)

include_directories(${CMAKE_SOURCE_DIR}/PiZero/PiZero)
add_subdirectory(PiZero/PiZero)
