
cmake_minimum_required( VERSION 2.8.8 )

project( LEAF )

set( ${CMAKE_PROJECT_NAME}_MAJOR_VERSION 0 )
set( ${CMAKE_PROJECT_NAME}_MINOR_VERSION 1 )
set( ${CMAKE_PROJECT_NAME}_PATCH_VERSION 0 )
set( v ${CMAKE_PROJECT_NAME}_VERSION )
set( ${v} ${${CMAKE_PROJECT_NAME}_MAJOR_VERSION} )
set( ${v} ${${v}}.${${CMAKE_PROJECT_NAME}_MINOR_VERSION} )
set( ${v} ${${v}}.${${CMAKE_PROJECT_NAME}_PATCH_VERSION} )
set( v ${${v}} )
message( STATUS "Configuring ${CMAKE_PROJECT_NAME}: v${v}\n" )

set( INSTALL_LIBDIR lib )
set( INSTALL_BINDIR bin )
set( INSTALL_INCDIR include )
set( INSTALL_SHRDIR share )

if( WIN32 )
    string( REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
        CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" )
    string( REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
        CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" )

    if( MSVC )
        if( ${CMAKE_GENERATOR} MATCHES "Win64" )
            set( VC_PLATFORM "x64" )
        else()
            set( VC_PLATFORM "Win32" )
        endif()
        message( STATUS "VC++ Platform: ${VC_PLATFORM}\n" )

        if( MSVC90 )
            set( VC_VERSION 9 )
        elseif( MSVC10 )
            set( VC_VERSION 10 )
        elseif( MSVC11 )
            set( VC_VERSION 11 )
        else()
            message( FATAL_ERROR "VC++ version not supported" )
        endif()
        message( STATUS "VC++ Version: ${VC_VERSION}\n" )
    endif()
endif()

#Set a default build type if none was specified
if( NOT WIN32 AND NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Set build type" FORCE )
endif()

#Update CMake module path
set( CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/CMakeModules"
    ${CMAKE_MODULE_PATH} )

#Ant
set( ANT_ROOT "Please specify"
    CACHE PATH "Root directory where Ant is installed" )
find_package( Ant 1.8.4 )

#Boost
if( WIN32 )
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()
set( BOOST_USE_STATIC_LIBS OFF )
set( BOOST_USE_MULTITHREADED ON )
set( BOOST_USE_STATIC_RUNTIME OFF )
set( BOOST_ROOT "Please specify"
    CACHE PATH "Root directory where Boost is installed" )
find_package( Boost 1.53.0 REQUIRED COMPONENTS
    chrono date_time filesystem prg_exec_monitor program_options regex system
    thread )

#CSharp
find_package( CSharp 3.5 )

#MinGW GFortran
set( MINGW_GFORTRAN "Please specify"
    CACHE FILEPATH "Full path to the MinGW GFortran compiler - C:\\MinGW\\bin\\gfortran.exe" )

#Java
find_package( Java 1.6.0 REQUIRED )
get_filename_component( JAVA_HOME ${JAVA_INCLUDE_PATH} PATH )
set( ENV{JAVA_HOME} "${JAVA_HOME}" )
message( STATUS "JAVA_HOME=$ENV{JAVA_HOME}" )

#JNI
find_package( JNI 1.6.0 REQUIRED )

#POCO
if( POCO_STATIC )
    add_definitions( -DPOCO_STATIC )
endif()
set( POCO_ROOT "Please specify"
    CACHE PATH "Root directory where POCO is installed" )
find_package( POCO 1.5.0 REQUIRED COMPONENTS
    Data DataODBC DataSQLite Foundation JSON Net Util XML )
set(POCO_MYSQL_API_DEFINE "undef")
set(POCO_ODBC_API_DEFINE "undef")

foreach( config IN LISTS CMAKE_CONFIGURATION_TYPES ITEMS ${CMAKE_BUILD_TYPE} )
    string( TOUPPER ${config} config )
    if( config MATCHES "DEBUG" )
        if( POCO_DataMySQL_LIBRARY_DEBUG )
            #set( CMAKE_CXX_FLAGS_DEBUG
            #    "-DPOCO_MYSQL_API ${CMAKE_CXX_FLAGS_DEBUG}" )
            set( POCO_MYSQL_API_DEFINE "define" )
        endif()
        if( POCO_DataODBC_LIBRARY_DEBUG )
            #set( CMAKE_CXX_FLAGS_DEBUG
            #    "-DPOCO_ODBC_API ${CMAKE_CXX_FLAGS_DEBUG}" )
            set( POCO_ODBC_API_DEFINE "define" )
        endif()
    else()
        if( POCO_DataMySQL_LIBRARY )
            #set( CMAKE_CXX_FLAGS_${config}
            #    "-DPOCO_MYSQL_API ${CMAKE_CXX_FLAGS_${config}}" )
            set( POCO_MYSQL_API_DEFINE "define" )
        endif()
        if( POCO_DataODBC_LIBRARY )
            #set( CMAKE_CXX_FLAGS_${config}
            #    "-DPOCO_ODBC_API ${CMAKE_CXX_FLAGS_${config}}" )
            set( POCO_ODBC_API_DEFINE "define" )
        endif()
    endif()
endforeach()
configure_file( ${PROJECT_SOURCE_DIR}/src/leaf/LEAFConfig.h.in
    ${PROJECT_SOURCE_DIR}/src/leaf/LEAFConfig.h @ONLY )

#Swig
find_package( Swig )

#XERCESC
if( WIN32 )
    set( XERCESC_ROOT_DIR "Please specify"
        CACHE PATH "Root directory where Xerces-C is installed" )
    find_package( XERCESC 3.1.1 REQUIRED )
endif()

#
option( WEPS_INSTALL "Install files needed to run weps.gui" OFF )
add_subdirectory( external )
add_subdirectory( src )

set( EXPORT_LIBRARY_TARGETS
    leaf_open
    leaf_util
    leaf_wrapper_java
    leaf_wrapper_weps )

if( TARGET leaf_wrapper_daycent )
    list( APPEND EXPORT_LIBRARY_TARGETS leaf_wrapper_daycent )
endif()

if( TARGET leaf_wrapper_rusle2 )
    list( APPEND EXPORT_LIBRARY_TARGETS leaf_wrapper_rusle2 )
endif()

if( TARGET leaf_wrapper_powersim )
    list( APPEND EXPORT_LIBRARY_TARGETS leaf_wrapper_powersim )
endif()

set( CONFIGURE_VARS
    INSTALL_LIBDIR )

include( InstallAutoDiscoveryTools )
