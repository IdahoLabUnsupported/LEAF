
#On windows provide the user with the pdb files for debugging if they are present
if( MSVC )
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/\${CMAKE_INSTALL_CONFIG_NAME}/${TARGET_NAME}.pdb
        DESTINATION ${INSTALL_BINDIR}
        CONFIGURATIONS RelWithDebInfo Debug )
endif()
