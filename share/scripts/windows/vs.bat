REM Specify the environment variables
call %~dp0setup.bat

REM only change this if your Visual Studio .NET is installed somewhere else
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.exe" %LEAF_BUILD_DIR%\LEAF.sln