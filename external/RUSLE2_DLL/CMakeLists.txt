
cmake_minimum_required( VERSION 2.8.8 )

include( ExternalProject )

#
get_filename_component( REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR} REALPATH )
set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/RomeDLL SDK/${VC_PLATFORM}/vc${VC_VERSION}" )

set( BUILD_COMMAND "" )
if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/RomeDLL.vc8.vcproj )
    #set( BUILD_COMMAND devenv ${REAL_PATH}/RomeDLL.vc8.vcproj /Upgrade )
    #set( BUILD_COMMAND ${BUILD_COMMAND} COMMAND devenv ${REAL_PATH}/RomeDLL.vc8.vcproj /Build "Release|Win32" )
    #set( BUILD_COMMAND ${BUILD_COMMAND} COMMAND msbuild ${REAL_PATH}/RomeDLL.vc8.vcproj /p:Configuration=Release /p:Platform=Win32 )
    #set( BUILD_COMMAND ${BUILD_COMMAND} COMMAND ${REAL_PATH}/"build RomeDLL SDK.bat" )
endif()

externalproject_add( RUSLE2_DLL
    SOURCE_DIR ${SOURCE_DIR}
    INSTALL_DIR ${INSTALL_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND "${BUILD_COMMAND}"
    INSTALL_COMMAND "" )

file( GLOB HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.c" )

#Create imported targets for all libraries
set( TARGET_NAME RomeDLL )
set( PUBLIC_HEADERS
    api-rome.h
    rxencode.h )
set( DLL ${CMAKE_FIND_LIBRARY_PREFIXES}${TARGET_NAME}.dll )
set( LIB ${CMAKE_FIND_LIBRARY_PREFIXES}${TARGET_NAME}${CMAKE_FIND_LIBRARY_SUFFIXES} )
set( PDB ${CMAKE_FIND_LIBRARY_PREFIXES}${TARGET_NAME}.pdb )

add_library( ${TARGET_NAME} SHARED IMPORTED GLOBAL )
set_property(
    TARGET ${TARGET_NAME}
    APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG )
set_target_properties(
    ${TARGET_NAME}
    PROPERTIES
        IMPORTED_IMPLIB_NOCONFIG ${INSTALL_DIR}/${LIB}
        IMPORTED_LOCATION_NOCONFIG ${INSTALL_DIR}/${DLL} )
add_dependencies( ${TARGET_NAME} RUSLE2_DLL )

install(
    FILES ${INSTALL_DIR}/${DLL}
    DESTINATION ${INSTALL_BINDIR} )

install(
    FILES ${INSTALL_DIR}/${LIB}
    DESTINATION ${INSTALL_LIBDIR} )

if( EXISTS ${INSTALL_DIR}/${PDB} )
install(
    FILES ${INSTALL_DIR}/${PDB}
    DESTINATION ${INSTALL_BINDIR} )
endif()

foreach( HDR ${PUBLIC_HEADERS} )
    install(
        FILES ${INSTALL_DIR}/${HDR}
        DESTINATION ${INSTALL_INCDIR} )
endforeach()
