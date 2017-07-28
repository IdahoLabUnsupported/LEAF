
# http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file

# Add all targets to the build-tree export set
export(
    TARGETS ${EXPORT_LIBRARY_TARGETS}
    FILE "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}LibraryDepends.cmake" )

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export( PACKAGE ${CMAKE_PROJECT_NAME} )

#
#configure_file( ${CMAKE_MODULE_PATH}/Config.cmake.in
#    "${PROJECT_BINARY_DIR}/${INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake" @ONLY )
configure_file( ${CMAKE_MODULE_PATH}/ConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/${INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}ConfigVersion.cmake" @ONLY )
configure_file( ${CMAKE_MODULE_PATH}/Use.cmake.in
    "${PROJECT_BINARY_DIR}/Use${CMAKE_PROJECT_NAME}.cmake" @ONLY )

# Create the Config.cmake and ConfigVersion files
include( CMakePackageConfigHelpers )
configure_package_config_file(
    ${CMAKE_MODULE_PATH}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION "${INSTALL_LIBDIR}/cmake"
    PATH_VARS ${CONFIGURE_VARS} )

# ... for ...
configure_file( ${CMAKE_MODULE_PATH}/Find.cmake.in
    "${PROJECT_BINARY_DIR}/Find${CMAKE_PROJECT_NAME}.cmake" @ONLY )

# Install the Config.cmake, ConfigVersion.cmake, and Use.cmake
install( FILES
    "${PROJECT_BINARY_DIR}/${INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}/Use${CMAKE_PROJECT_NAME}.cmake"
    DESTINATION "${INSTALL_LIBDIR}/cmake" )

# Install the export set for use with the install-tree
install(
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    DESTINATION "${INSTALL_LIBDIR}/cmake" )

# Install the Find.cmake
install( FILES
    "${PROJECT_BINARY_DIR}/Find${CMAKE_PROJECT_NAME}.cmake"
    DESTINATION "${INSTALL_SHRDIR}/extra" )
