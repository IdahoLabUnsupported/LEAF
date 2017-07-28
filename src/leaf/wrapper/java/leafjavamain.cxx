/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2013 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/


///This code is ripped directly from:
///vrkit = https://realityforge.vrsource.org/trac/vrkit

#if defined( WIN32 ) || defined( WIN64 )
#include <windows.h>
#endif

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

#if !defined( WIN32 ) && !defined( WIN64 )
#include <dlfcn.h>
#endif

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/version.hpp>

namespace fs = boost::filesystem;

#if defined( WIN32 ) || defined( WIN64 )

///Windows DLL entry point function. This ensures that the environment
///variable \c LEAF_BASE_DIR is set as soon as this DLL is attached to the
///process. If it is not set, then it sets it based on an assumption about the
///structure of a leaf installation. More specifically, an assumption is made
///that this DLL lives in the \c lib subdirectory of the leaf installation.
///Therefore, the root of the leaf installation is the parent of the
///directory containing this DLL.
BOOL __stdcall DllMain( HINSTANCE module, DWORD reason, LPVOID reserved )
{
    switch( reason )
    {
    case DLL_PROCESS_ATTACH:
    {
        try
        {
            char tmppath[1024];
            std::memset( tmppath, 0, sizeof( tmppath ) );
            GetModuleFileName( module, tmppath, sizeof( tmppath ) );

            fs::path dll_path( tmppath, fs::native );
            dll_path = dll_path.parent_path();

            char* path_env( NULL );
            size_t path_len;
            _dupenv_s( &path_env, &path_len, "PATH" );
            fs::path path = fs::path( path_env, fs::native );
            //std::string path_string = path.string();
            fs::path jreBinPath = dll_path/"jre"/"bin";
            fs::path jreBinServerPath = jreBinPath/"server";
            std::string path_string =
                jreBinPath.string() + ";" +
                jreBinServerPath.string() + ";" + path.string();


            _putenv_s( "PATH", path_string.c_str() );
            std::cout << "PATH=" << path_string << std::endl;

        }
        catch( fs::filesystem_error& ex )
        {
            std::cerr << "Automatic assignment of VP environment "
                      << "variables failed:\n" << ex.what() << std::endl;
        }
    }
    break;
    default:
        break;
    }

    return TRUE;
}
//#else

#endif //defined( WIN32 ) || defined( WIN64 )
