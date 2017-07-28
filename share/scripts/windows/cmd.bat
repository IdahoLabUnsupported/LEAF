REM @echo off

REM Specify the environment variables

set /p ARCH=ARCH (x86,x64):

set "PROGRAM_FILES=Program Files"
if %ARCH%==x86 (
    set BUILD_DIR=build-32-bit
    set INSTALL_DIR=install-32-bit

    if NOT %PROCESSOR_ARCHITECTURE%==x86 (
        set "PROGRAM_FILES=%PROGRAM_FILES% (x86)"
    )
) else (
    set BUILD_DIR=build-64-bit
    set INSTALL_DIR=install-64-bit
)

call %~dp0setup.bat

REM only change this if your Visual Studio .NET is installed somewhere else
%comspec% /k ""C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat"" %ARCH%

cmd
