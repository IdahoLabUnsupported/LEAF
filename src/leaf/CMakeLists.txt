
add_subdirectory( open )
add_subdirectory( util )
add_subdirectory( wrapper )

install(
    FILES LEAFConfig.h
    DESTINATION ${INSTALL_INCDIR}/leaf )
