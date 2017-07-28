
if( CSHARP_FOUND )
    set( TARGET_NAME leaf_wrapper_rusle2_csharp )
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
    set( CMAKE_SWIG_FLAGS -namespace leaf.wrapper.rusle2 -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set_source_files_properties( ${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp
        boost_csharp
        Poco_csharp
        Poco_Data_csharp
        Poco_Dynamic_csharp
        leaf_util_csharp
        leaf_open_csharp
        leaf_open_ero_csharp
        leaf_open_man_csharp
        leaf_open_scn_csharp
        leaf_open_soi_csharp
        ${SWIG_DIR}/leaf/util/module.i
        ${SWIG_DIR}/leaf/open/ero/module.i
        ${SWIG_DIR}/leaf/open/man/module.i
        ${SWIG_DIR}/leaf/open/soi/module.i )
    swig_add_module( ${TARGET_NAME} csharp ${INTERFACE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        leaf_util
        leaf_open
        leaf_wrapper_rusle2
        ${Boost_LIBRARIES}
        ${POCO_LIBRARIES} )

    include( ModuleInstall )
endif()
