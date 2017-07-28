
if( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/PsSimEng.h )
    return()
endif()

file( GLOB HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.c*" )

set( TARGET_NAME leaf_wrapper_powersim )
set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
list( REMOVE_ITEM PUBLIC_HEADERS "PsSimEng.h" )
set( PRIVATE_SOURCES ${SOURCES} )
set( INCDIR_NAME "leaf/wrapper/powersim" )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${POCO_INCLUDE_DIR} )

add_definitions( -DSTRICT )
add_definitions( -DWIN32 )
add_definitions( -D_WINDOWS )
add_definitions( -DWIN32_LEAN_AND_MEAN )
add_definitions( -DVC_EXTRALEAN )
add_definitions( -D_WINDLL )
add_definitions( -D_USRDLL )
add_definitions( -D_ATL_DLL )
add_definitions( -DLEAF_POWERSIM_LIBRARY )

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )
target_link_libraries( ${TARGET_NAME}
    leaf_util
    ${Boost_LIBRARIES}
    ${POCO_LIBRARIES} )

include( ModuleInstall )
