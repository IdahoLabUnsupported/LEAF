
if( NOT JNI_FOUND )
    return()
endif()

file( GLOB_RECURSE HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB_RECURSE SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cxx" )

set( TARGET_NAME leaf_wrapper_java )
set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )
set( INCDIR_NAME "leaf/wrapper/java" )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${JNI_INCLUDE_DIRS} )
include_directories( ${POCO_INCLUDE_DIR} )

add_definitions( -DLEAF_JAVA_LIBRARY )

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )
target_link_libraries( ${TARGET_NAME}
    ${Boost_LIBRARIES}
    ${JNI_LIBRARIES}
    ${POCO_LIBRARIES} )

set_target_properties( ${TARGET_NAME}
    PROPERTIES LINK_FLAGS "/DELAYLOAD:jvm.dll" )

include( ModuleInstall )
