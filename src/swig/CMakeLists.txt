
include( ${SWIG_USE_FILE} )

set( SWIG_DIR ${CMAKE_SOURCE_DIR}/src/swig )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${SWIG_DIR} )

add_subdirectory( std )
add_subdirectory( Poco )
add_subdirectory( boost )
add_subdirectory( leaf )
