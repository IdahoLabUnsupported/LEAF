#Required variables:
    #TARGET_NAME
    #TARGET_VERSION
    #TARGET_EXPORT
    #TARGET_CATEGORY - App/Lib/Jar/Swig
    #TARGET_LANGUAGE - C/CXX/Fortran/...
    #PRIVATE_HEADERS - must be relative
    #PUBLIC_HEADERS - must be relative
    #PRIVATE_SOURCES - must be relative
    #INCDIR_NAME

#
if( NOT TARGET_LANGUAGE )
    set( TARGET_LANGUAGE CXX )
endif()

#
set_target_properties( ${TARGET_NAME}
    PROPERTIES PROJECT_LABEL "${TARGET_CATEGORY} ${TARGET_NAME}" )
set_target_properties( ${TARGET_NAME} PROPERTIES VERSION ${TARGET_VERSION} )
set_target_properties( ${TARGET_NAME} PROPERTIES SOVERSION ${TARGET_VERSION} )

#
if( WIN32 )
    foreach( HDR ${PRIVATE_HEADERS} )
        get_filename_component( PATH ${HDR} PATH )
        file( TO_NATIVE_PATH "${PATH}" PATH )
        source_group( "Header Files\\${PATH}" FILES ${HDR} )
    endforeach()

    foreach( SRC ${PRIVATE_SOURCES} )
        get_filename_component( PATH ${SRC} PATH )
        file( TO_NATIVE_PATH "${PATH}" PATH )
        source_group( "Source Files\\${PATH}" FILES ${SRC} )
    endforeach()

    include( InstallPDBFiles )
endif()

if( ${TARGET_CATEGORY} STREQUAL "App" )
    #
    install(
        TARGETS ${TARGET_NAME}
        DESTINATION ${INSTALL_BINDIR} )
elseif( ${TARGET_CATEGORY} STREQUAL "Lib" )
    #
    install(
        TARGETS ${TARGET_NAME}
        EXPORT ${TARGET_EXPORT}
        RUNTIME DESTINATION ${INSTALL_BINDIR}
        LIBRARY DESTINATION ${INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${INSTALL_LIBDIR} )

    #
    if( INCDIR_NAME )
        foreach( HDR ${PUBLIC_HEADERS} )
            get_filename_component( PATH ${HDR} PATH )
            install(
                FILES ${HDR}
                DESTINATION ${INSTALL_INCDIR}/${INCDIR_NAME}/${PATH} )
        endforeach()
    endif()
elseif( ${TARGET_CATEGORY} STREQUAL "Jar" )
    #
    install_jar( ${TARGET_NAME} ${INSTALL_BINDIR} )
elseif( ${TARGET_CATEGORY} STREQUAL "Swig" )
    #
    install(
        TARGETS ${TARGET_NAME}
        EXPORT ${TARGET_EXPORT}
        RUNTIME DESTINATION ${INSTALL_BINDIR}
        LIBRARY DESTINATION ${INSTALL_BINDIR}
        ARCHIVE DESTINATION ${INSTALL_BINDIR} )

    if( ${SWIG_LANGUAGE} STREQUAL "CSHARP" )
        set( CSHARP_SFX _dotnet )
        set( CSHARP_REF /reference:System.dll )
        foreach( DEP ${SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS} )
            if( TARGET ${DEP} )
                get_property( LOC TARGET ${DEP}${CSHARP_SFX} PROPERTY LOCATION )
                if( WIN32 )
                    string( REPLACE "/" "\\" LOC ${LOC} )
                endif()
                list( APPEND CSHARP_REF /reference:${LOC} )
            endif()
        endforeach()

        set( CSHARP_NAME ${TARGET_NAME}${CSHARP_SFX} )
        set( CSHARP_DLL ${CMAKE_SWIG_OUTDIR}/${CSHARP_NAME}.dll )
        set( CSHARP_SRC ${CMAKE_SWIG_OUTDIR}/*.cs )

        #Create imported targets for all libraries
        add_library( ${CSHARP_NAME} SHARED IMPORTED GLOBAL )
        set_target_properties( ${CSHARP_NAME}
            PROPERTIES IMPORTED_LOCATION ${CSHARP_DLL} )

        install(
            FILES ${CSHARP_DLL}
            DESTINATION ${INSTALL_BINDIR} )

        set( CSHARP_CREATED
            ${CSHARP_DLL}
            ${swig_generated_file_fullname}
            ${swig_extra_generated_files} )
        add_custom_command(
            TARGET ${TARGET_NAME}
            PRE_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E echo "Executing Pre-Build Script..." &&
                ${CMAKE_COMMAND}
                    -Dswig_generated_file_fullname=${swig_generated_file_fullname}
                    -DCMAKE_SWIG_OUTDIR=${CMAKE_SWIG_OUTDIR}
                    -P ${SWIG_DIR}/prebuild.cmake )

        if( WIN32 )
            string( REPLACE "/" "\\" CSHARP_DLL ${CSHARP_DLL} )
            string( REPLACE "/" "\\" CSHARP_SRC ${CSHARP_SRC} )
        endif()
        add_custom_command(
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E echo "Executing Post-Build Script..." &&
                ${CMAKE_COMMAND}
                    -DTARGET_NAME=${TARGET_NAME}
                    -DTARGET_FILE=$<TARGET_FILE:${TARGET_NAME}>
                    -DCSHARP_COMPILER=${CSHARP_COMPILER}
                    -DCSHARP_PLATFORM=${CSHARP_PLATFORM}
                    -DCSHARP_SDK=${CSHARP_SDK}
                    -DCSHARP_REF="${CSHARP_REF}"
                    -DCSHARP_DLL=${CSHARP_DLL}
                    -DCSHARP_SRC=${CSHARP_SRC}
                    -P ${SWIG_DIR}/postbuild.cmake )
    endif()
endif()
