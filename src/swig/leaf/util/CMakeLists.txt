
if( CSHARP_FOUND )
    set( TARGET_NAME leaf_util_csharp )
    set( TARGET_VERSION ${LEAF_VERSION} )
    set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
    set( TARGET_CATEGORY Swig )
    set( SWIG_LANGUAGE CSHARP )
    set( PRIVATE_HEADERS "" )
    set( PUBLIC_HEADERS "" )
    set( PRIVATE_SOURCES "" )

    include_directories( ${Boost_INCLUDE_DIRS} )
    include_directories( ${POCO_INCLUDE_DIR} )

    set( INTERFACE_FILE module.i )
    set( CMAKE_SWIG_FLAGS -namespace leaf.util -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set_source_files_properties( ${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp
        boost_csharp
        Poco_csharp
        Poco_Data_csharp
        Poco_Dynamic_csharp
        ${SWIG_DIR}/boost/module.i
        ${SWIG_DIR}/Poco/Data/module.i )
    swig_add_module( ${TARGET_NAME} csharp ${INTERFACE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        leaf_util
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()
