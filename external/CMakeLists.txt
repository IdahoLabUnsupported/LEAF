cmake_minimum_required( VERSION 2.8.8 )

#http://www.kitware.com/blog/home/post/231
#http://www.netlib.org/lapack/lawnspdf/lawn270.pdf
#32-bit: http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst
#64-bit: http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/rubenvb/gcc-4.8-release/x86_64-w64-mingw32-gcc-4.8.0-win64_rubenvb.7z/download
#Extract to "C:/mingw64"
#Add "C:/mingw64/bin" to environment
include( CMakeAddFortranSubdirectory )

if( WIN32 )
    #Daycent
    add_subdirectory( DailyDayCent_file100 )

    set( TARGET_NAME DailyDayCent_list100 )
    set( GNU_HOST x86_64-w64-mingw32 )
    set( CMAKE_TOOLCHAIN_FILE "${CMAKE_MODULE_PATH}/mingw_cross_toolchain.cmake" )
    cmake_add_fortran_subdirectory( ${TARGET_NAME}
        PROJECT ${TARGET_NAME}
        ARCHIVE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}
        RUNTIME_DIR ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}
        LIBRARIES ${TARGET_NAME}
        LINK_LIBRARIES
        LINK_LIBS
        CMAKE_COMMAND_LINE
            -DCMAKE_SYSTEM_NAME=Windows
        NO_EXTERNAL_INSTALL )

    #Override hack for CMakeAddFortranSubdirectory
    set_target_properties(
        ${TARGET_NAME}
        PROPERTIES
            IMPORTED_IMPLIB_NOCONFIG ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/${TARGET_NAME}.lib
            IMPORTED_LOCATION_NOCONFIG ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/${TARGET_NAME}.dll )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/${TARGET_NAME}.lib
        DESTINATION lib )

    #Rusle2
    add_subdirectory( RUSLE2_DLL )
endif()

#Weps
if( ANT_FOUND )
    add_subdirectory( MSI-release-1.2.9 )
else()
    message( SEND_ERROR
        "Could not build weps.gui: Please define ANT_HOME in ENV" )
endif()

#Sqlite2
add_subdirectory( sqlite2 )
