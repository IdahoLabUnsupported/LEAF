
file( GLOB HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cxx" )

set( TARGET_NAME leaf_wrapper_rusle2 )
set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )
set( INCDIR_NAME "leaf/wrapper/rusle2" )

include_directories( "${CMAKE_SOURCE_DIR}/external/RUSLE2_DLL/RomeDLL SDK/${VC_PLATFORM}/vc${VC_VERSION}" )
include_directories( ${CMAKE_SOURCE_DIR}/external/sqlite2 )
include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${POCO_INCLUDE_DIR} )

add_definitions( -D_USRDLL )
add_definitions( -D_AFXDLL )
add_definitions( -DLEAF_RUSLE2_LIBRARY )
add_definitions( -D__RUSLE2_version=2002005 )

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )
target_link_libraries( ${TARGET_NAME}
    RomeDLL
    sqlite2
    leaf_util
    leaf_open
    ${Boost_LIBRARIES}
    ${POCO_LIBRARIES} )
target_link_libraries( ${TARGET_NAME} LINK_INTERFACE_LIBRARIES )

include( ModuleInstall )
