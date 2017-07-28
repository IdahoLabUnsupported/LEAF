
cmake_minimum_required( VERSION 2.8.8 )

#The autofortran library linking does not work unless Fortran is enabled
include( CheckLanguage )
check_language( Fortran )
if( CMAKE_Fortran_COMPILER )
    enable_language( Fortran )
else()
    return()
endif()

set( TARGET_NAME DailyDayCent_list100 )
add_library( ${TARGET_NAME} STATIC Table.f Table.inc )
if( WIN32 )
    set_target_properties(
      ${TARGET_NAME}
      PROPERTIES
          OUTPUT_NAME ${TARGET_NAME}
          PREFIX ""
          SUFFIX ".lib" )
endif()

#Important for Intel Fortran Compiler
#if( NOT ${CMAKE_Fortran_COMPILER_ID} STREQUAL "Intel" )
#    return()
#endif()
#add_definitions( /libdir:noauto )
