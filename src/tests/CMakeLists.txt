
if( JAVA_FOUND )
    include( UseJava )
endif()

if( CSHARP_FOUND )
    include( ${CSHARP_USE_FILE} )
endif()

set( TARGET_VERSION ${LEAF_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${JNI_INCLUDE_DIRS} )
include_directories( ${POCO_INCLUDE_DIR} )

if( WIN32 )
    if( TARGET leaf_open )
        set( TARGET_NAME db_test )
        set( TARGET_CATEGORY App )
        set( TARGET_LANGUAGE CXX )

        add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx ${TARGET_NAME}.h )
        target_link_libraries( ${TARGET_NAME}
            leaf_util
            leaf_open
            leaf_wrapper_weps
            ${Boost_LIBRARIES}
            ${POCO_LIBRARIES} )

        include( ModuleInstall )
    endif()

    if( TARGET leaf_wrapper_daycent )
        set( TARGET_NAME daycent_test )
        set( TARGET_CATEGORY App )
        set( TARGET_LANGUAGE CXX )

        add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx ${TARGET_NAME}.h )
        target_link_libraries( ${TARGET_NAME}
            leaf_util
            leaf_open
            leaf_wrapper_daycent
            ${Boost_LIBRARIES}
            ${POCO_LIBRARIES} )

        include( ModuleInstall )
    endif()

    if( TARGET leaf_wrapper_weps )
        set( TARGET_NAME weps_test )
        set( TARGET_CATEGORY App )
        set( TARGET_LANGUAGE CXX )

        add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx ${TARGET_NAME}.h )
        target_link_libraries( ${TARGET_NAME}
            leaf_util
            leaf_open
            leaf_wrapper_java
            leaf_wrapper_weps
            ${Boost_LIBRARIES}
            ${POCO_LIBRARIES} )

        include( ModuleInstall )
    endif()

    if( TARGET leaf_wrapper_rusle2 )
        set( TARGET_NAME man_test )
        set( TARGET_CATEGORY App )
        set( TARGET_LANGUAGE CXX )

        add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx ${TARGET_NAME}.h )
        target_link_libraries( ${TARGET_NAME}
            leaf_util
            leaf_open
            leaf_wrapper_rusle2
            ${Boost_LIBRARIES}
            ${POCO_LIBRARIES} )

        include( ModuleInstall )
    endif()

    if( SWIG_FOUND )
        set( TARGET_NAME swig_test_cs )
        set( TARGET_CATEGORY App )
        set( SWIG_LANGUAGE CSHARP )

        set( CSHARP_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
        set( CSHARP_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

        set( CSHARP_REF )
        set( CSHARP_DEPS
            std_csharp_dotnet
            boost_csharp_dotnet
            Poco_csharp_dotnet
            Poco_Data_csharp_dotnet
            Poco_Dynamic_csharp_dotnet
            leaf_util_csharp_dotnet
            leaf_open_csharp_dotnet
            leaf_open_ero_csharp_dotnet
            leaf_open_scn_csharp_dotnet
            leaf_open_soi_csharp_dotnet
            leaf_wrapper_rusle2_csharp_dotnet
            leaf_wrapper_weps_csharp_dotnet )
        foreach( DEP ${CSHARP_DEPS} )
            get_property( LOC TARGET ${DEP} PROPERTY LOCATION )
            list( APPEND CSHARP_REF ${LOC} )
        endforeach()

        csharp_add_executable( ${TARGET_NAME} swig_test.cs ${CSHARP_REF} )
        add_dependencies( ${TARGET_NAME}
            CSHARP_DEPS )

        install(
            FILES ${CSHARP_BINARY_DIRECTORY}/${TARGET_NAME}.exe
            DESTINATION ${INSTALL_BINDIR} )
    endif()

    if( TARGET leaf_wrapper_powersim )
        set( TARGET_NAME powersim_test )
        set( TARGET_CATEGORY App )
        set( TARGET_LANGUAGE CXX )

        add_executable( ${TARGET_NAME} ${TARGET_NAME}.cxx ${TARGET_NAME}.h )
        target_link_libraries( ${TARGET_NAME}
            leaf_wrapper_powersim
            ${Boost_LIBRARIES}
            ${POCO_LIBRARIES} )

        include( ModuleInstall )

        #To run powersim_test.java: java -cp * powersim_test
        if( TARGET leaf_wrapper_powersim_jni AND TARGET leaf_wrapper_powersim_java )
            set( TARGET_NAME powersim_test_java )
            set( TARGET_CATEGORY Jar )
            set( TARGET_LANGUAGE Java )

            set( JAR_NAME powersim_test )
            set( CMAKE_JAVA_TARGET_OUTPUT_NAME ${JAR_NAME} )
            set( CMAKE_JAVA_JAR_ENTRY_POINT ${JAR_NAME} )
            get_property( JAVA_JAR_LOC
                TARGET leaf_wrapper_powersim_java
                PROPERTY INSTALL_FILES )
            set( CMAKE_JAVA_INCLUDE_PATH ${JAVA_JAR_LOC} )

            add_jar( ${TARGET_NAME} ${JAR_NAME}.java )
            add_dependencies( ${TARGET_NAME} leaf_wrapper_powersim_java )

            include( ModuleInstall )
        endif()
    endif()
endif()
