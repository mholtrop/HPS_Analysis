#	@author: Maurik Holtrop <maurik@physics.unh.edu>
#	@date: July 14, 2016
#
# Based on the CMakeLists.txt file from Omar Moreno for hps-dst
#
# Specify the minimum CMake version required.  If the cmake version 
# minimum is not met, cmake will fail.
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
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

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 
        SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "HPS_Analysis install prefix, default is project top dir." FORCE) 
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 

message("The installation prefix is set to:  ${CMAKE_INSTALL_PREFIX} ")   

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" CACHE PATH "Build location of binary executatbles.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" CACHE PATH "Build location of shared libraries.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" CACHE PATH "Build location of static libraries.")


set(CMAKE_INSTALL_LIBDIR           "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation location of libraries. ")
set(CMAKE_INSTALL_BINDIR           "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation location of binaries. ")
set(CMAKE_INSTALL_DOCDIR           "${CMAKE_INSTALL_PREFIX}/doc" CACHE PATH "Installation location of documentation. ")


# Set the location of the LCIO root directory.
set(LCIO_ROOT_DIR "$ENV{LCIO}" CACHE PATH "Location of the LCIO installation. ")

message(STATUS "Using LCIO from ${LCIO_ROOT_DIR}")


# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT

if( NOT DEFINED ENV{ROOTSYS} ) 
    message( FATAL_ERROR  "You need to setup ROOT first. ")
else()
    message("Using ROOT from $ENV{ROOTSYS} ")
endif()

list(APPEND CMAKE_PREFIX_PATH "$ENV{ROOTSYS}/cmake" )
message("CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)

message("Got the FindROOT.cmake from: ${ROOT_DIR} ")
message("We have ROOT_USE_FILE: ${ROOT_USE_FILE} ")

#---Overwrite some of the ROOT_DEFINITIONS since these are way too pedantic.
set(ROOT_DEFINITIONS "")

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add the hps-dst include directories.

set(HPS_DST_ROOT_DIR "${HPS_ANALYSIS_SOURCE_DIR}/hps-dst" CACHE PATH "Location of the hps-dst installation to use.")
include_directories("${HPS_DST_ROOT_DIR}/include/dst")
include_directories("${HPS_DST_ROOT_DIR}/include/hps_event")
include_directories("${HPS_DST_ROOT_DIR}/include/utils")

set(HPS_DST_LIB_DIR "${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Location of the libraries for hps-dst.")

# Include the LCIO headers
include_directories(${LCIO_ROOT_DIR}/include)

# Find the LCIO libraries
find_library(LCIO_LIB lcio ${LCIO_ROOT_DIR}/lib)
if(NOT LCIO_LIB)
	message(FATAL_ERROR
		"\nThe library liblcio was not found.  "
		"Please make sure LCIO has been installed."
		"Then specify -DLCIO_ROOT_DIR=<path> or "
		"set the environmental variable LCIO=<path> "
		"See documentation for details.")
endif()

message(STATUS "LCIO: ${LCIO_LIB}")


if(APPLE)
  # use, i.e. don't skip the full RPATH for the build tree
  SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

  # when building, don't use the install RPATH already
  # (but later on when installing)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

  SET(CMAKE_INSTALL_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

  # the RPATH to be used when installing, but only if it's not a system directory
  LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" isSystemDir)
  IF("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    message("CMAKE_INSTALL_RPATH set to ${CMAKE_INSTALL_RPATH} ")
    
  ENDIF("${isSystemDir}" STREQUAL "-1")
endif(APPLE)

add_subdirectory(hps-dst)

include_directories(${CMAKE_SOURCE_DIR}/BaseAna/BaseAna)
add_subdirectory(BaseAna/BaseAna)

include_directories(${CMAKE_SOURCE_DIR}/PiZero/PiZero)
add_subdirectory(PiZero/PiZero)

include_directories(${CMAKE_SOURCE_DIR}/FullEElec/FullEElec)
add_subdirectory(FullEElec/FullEElec)

# include_directories(${CMAKE_SOURCE_DIR}/SimAna/SimAna)
add_subdirectory(SimAna/SimAna)


#------------------------------#
#--- Generate Documentation ---#
#------------------------------#

# Find Doxygen
find_package(Doxygen)

# If Doxygen is found, add a target to generate the documentation
if(DOXYGEN_FOUND)
	configure_file(doc/Doxyfile ${HPS_ANALYSIS_SOURCE_DIR}/doc/Doxyfile @ONLY)
	add_custom_target(docs ${DOXYGEN_EXECUTABLE} ${HPS_ANALYSIS_SOURCE_DIR}/doc/Doxyfile WORKING_DIRECTORY ${HPS_ANALYSIS_SOURCE_DIR}/doc COMMENT "Generating documentation" VERBATIM)
else()
	MESSAGE(STATUS "Doxygen was not found.  Documentation will not be generated.")
endif(DOXYGEN_FOUND)

