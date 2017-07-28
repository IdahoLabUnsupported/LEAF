
file( GLOB HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cxx" )

set( TARGET_NAME leaf_util )
set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )
set( INCDIR_NAME "leaf/util" )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${POCO_INCLUDE_DIR} )

add_definitions( -DLEAF_UTIL_LIBRARY )

add_library( ${TARGET_NAME} SHARED ${SOURCES} ${HEADERS} )
target_link_libraries( ${TARGET_NAME}
    ${Boost_LIBRARIES}
    ${POCO_LIBRARIES} )

include( ModuleInstall )
