
file( GLOB_RECURSE HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB_RECURSE SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cxx" )

set( TARGET_NAME leaf_wrapper_daycent )
set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_CATEGORY Lib )
set( PRIVATE_HEADERS ${HEADERS} )
set( PUBLIC_HEADERS ${HEADERS} )
set( PRIVATE_SOURCES ${SOURCES} )
set( INCDIR_NAME "leaf/wrapper/daycent" )

include_directories( ${CMAKE_SOURCE_DIR}/external/DailyDayCent_file100 )
include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${POCO_INCLUDE_DIR} )
include_directories( ${XERCESC_INCLUDE_DIR} )

add_definitions( -D_SCL_SECURE_NO_WARNINGS )
add_definitions( -DLEAF_DAYCENT_LIBRARY )

add_library( ${TARGET_NAME} SHARED ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} )
target_link_libraries( ${TARGET_NAME}
    user32
    leaf_util
    leaf_open
    DailyDayCent_file100
    DailyDayCent_list100
    ${Boost_LIBRARIES}
    ${POCO_LIBRARIES}
    ${XERCESC_LIBRARY} )
target_link_libraries( ${TARGET_NAME} LINK_INTERFACE_LIBRARIES )

include( ModuleInstall )
