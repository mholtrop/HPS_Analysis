#
#	@author: Maurik Holtrop <maurik@physics.unh.edu>
#	@date: July 14, 2016
#
project(SimAna)

message("Creating the ${PROJECT_NAME} project")

include_directories(${BaseAna_INCLUDE_DIRS})

SET(PROJECT_HEADER_FILES SimAna.h)
SET(PROJECT_HEADER_LINKDEF_FILES SimAna_LinkDef.h)
SET(PROJECT_CXX_FILES  SimAna.cpp)
SET(PROJECT_EXTRA_LIBS ${LCIO_LIB} )

#--- Below this line, you should not need to specify project specific things.

#--- Generate the ROOT dictionary 
if( ROOT_VERSION VERSION_GREATER 6.0.0)
  ROOT_GENERATE_DICTIONARY(${PROJECT_NAME}Dict ${PROJECT_HEADER_FILES} MODULE ${PROJECT_NAME} LINKDEF ${PROJECT_HEADER_LINKDEF_FILES})
else()
  ROOT_GENERATE_DICTIONARY(${PROJECT_NAME}Dict ${PROJECT_HEADER_FILES} LINKDEF ${PROJECT_HEADER_LINKDEF_FILES})
endif()
#---Create a shared library with geneated dictionary
add_library(${PROJECT_NAME} SHARED ${PROJECT_CXX_FILES} ${PROJECT_NAME}Dict.cxx)

target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES} ${PROJECT_EXTRA_LIBS})

install(TARGETS ${PROJECT_NAME}  DESTINATION ${CMAKE_INSTALL_LIBDIR})
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Dict_rdict.pcm  DESTINATION ${INSTALL_LIB_DIR})

# Expose BaseAna public includes to other
# subprojects through cache variable.

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

