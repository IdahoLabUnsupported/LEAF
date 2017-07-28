REM Edit this file to reflect your local environment

set BOOST_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\boost_1_53_0\%INSTALL_DIR%
set GLOG_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\glog-0.3.3\%INSTALL_DIR%
set "JAVA_HOME=C:\%PROGRAM_FILES%\Java\jdk1.6.0_45"
set KIBITZ_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\kibitz\%INSTALL_DIR%
set LEAF_BUILD_DIR=%SystemDrive%%HOMEPATH%\dev\leaf-tools\trunk\%BUILD_DIR%
set LEAF_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\leaf-tools\trunk\%INSTALL_DIR%
set LEAF_APPS_BUILD_DIR=%SystemDrive%%HOMEPATH%\dev\leaf-apps\trunk\%BUILD_DIR%
set LEAF_APPS_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\leaf-apps\trunk\%INSTALL_DIR%
set IOWAMMP_BUILD_DIR=%SystemDrive%%HOMEPATH%\dev\iowammp\trunk\%BUILD_DIR%
set IOWAMMP_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\iowammp\trunk\%INSTALL_DIR%
set POCO_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\poco-1.5.1-all\%INSTALL_DIR%
set XERCES_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\xerces-c-3.1.1\%INSTALL_DIR%
set YAML_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\yaml-cpp-0.3.0\%INSTALL_DIR%
set ZMQ_INSTALL_DIR=%SystemDrive%%HOMEPATH%\dev\zeromq-3.2.3\%INSTALL_DIR%

set Path=^
%BOOST_INSTALL_DIR%\lib;^
%GLOG_INSTALL_DIR%\lib;^
%JAVA_HOME%\jre\bin;%JAVA_HOME%\jre\bin\server;^
%KIBITZ_INSTALL_DIR%\bin;%KIBITZ_INSTALL_DIR%\share\kibitz\bin;%KIBITZ_INSTALL_DIR%\lib;^
%IOWAMMP_INSTALL_DIR%\bin;^
%LEAF_INSTALL_DIR%\bin;%LEAF_INSTALL_DIR%\lib;^
%LEAF_APPS_INSTALL_DIR%\bin;^
%POCO_INSTALL_DIR%\bin;%POCO_INSTALL_DIR%\lib;^
%XERCES_INSTALL_DIR%\lib;^
%YAML_INSTALL_DIR%\lib;^
%ZMQ_INSTALL_DIR%\bin;%ZMQ_INSTALL_DIR%\lib;^
%Path%

