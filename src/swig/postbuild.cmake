
SEPARATE_ARGUMENTS( CSHARP_REF )

set( CSHARP_COMMAND
    ${CSHARP_COMPILER}
        /t:library /out:${CSHARP_DLL}
        /platform:${CSHARP_PLATFORM} ${CSHARP_SDK}
        ${CSHARP_REF}
        /debug:pdbonly /warn:0 /nologo
        /recurse:${CSHARP_SRC} )
message( STATUS "CSHARP_COMMAND: ${CSHARP_COMMAND}" )

execute_process(
    COMMAND ${CSHARP_COMMAND} RESULT_VARIABLE RESULT )

if( NOT RESULT EQUAL 0 )
    file( REMOVE ${TARGET_FILE} )
    message( FATAL_ERROR "Target ${TARGET_NAME} [${TARGET_FILE}] failed." )
endif()
