#Find Ant

#
if( DEFINED ENV{ANT_HOME} )
    set( ANT_ROOT $ENV{ANT_HOME} )
endif()

#
find_file( ANT_EXEC NAMES ant ant.sh ant.bat PATHS ${ANT_ROOT}/bin )

#
mark_as_advanced( ANT_EXEC )

#
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( ANT DEFAULT_MSG ANT_EXEC )
