
#Remove the swig file if it exists
if( EXISTS ${swig_generated_file_fullname} )
    file( REMOVE ${swig_generated_file_fullname} )
    message( STATUS "Removed ${swig_generated_file_fullname}" )
endif()

if( NOT EXISTS ${CMAKE_SWIG_OUTDIR} )
    return()
endif()

#Remove generated C# files and leftover cruft
file( GLOB FILES ${CMAKE_SWIG_OUTDIR}/* )
foreach( FILE ${FILES} )
    file( REMOVE ${FILE} )
    message( STATUS "Removed ${FILE}" )
endforeach()
