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


// --- LEAF Includes --- //
#include <leaf/wrapper/daycent/file/F_header.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

// --- STL Includes --- //
#include <iostream>
#include <io.h> //<unistd.h>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace file
{

////////////////////////////////////////////////////////////////////////////////
F_100::F_100()
    :
    m_fileIsOpen( false ),
    m_dirRoot(),
    m_options()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
F_100::~F_100()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void F_100::AddOption(
    Option const& option )
{
    m_options.push_back( option );
}
////////////////////////////////////////////////////////////////////////////////
void F_100::CloseFile()
{
    assert( m_fileIsOpen );
    fs::path cp( fs::current_path() );
    //fs::current_path( m_dirRoot );
    closefiles();
    m_options.clear();
    m_fileIsOpen = false;
    fs::current_path( cp );
}
////////////////////////////////////////////////////////////////////////////////
void F_100::DeleteOption(
    std::string const& abbrev )
{
    OptByAbb::const_iterator itr = Find( abbrev );
    m_options.get< ABB >().erase( itr );
}
////////////////////////////////////////////////////////////////////////////////
void F_100::DeleteOption(
    Option const& option )
{
    std::string const& abbrev = option.GetAbbrev();
    DeleteOption( abbrev );
}
////////////////////////////////////////////////////////////////////////////////
void F_100::Exit()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
OptByAbb::const_iterator F_100::Find(
    std::string const& abbrev ) const
{
    OptByAbb::const_iterator itr = abbrev.empty() ?
        m_options.get< ABB >().begin() : m_options.get< ABB >().find( abbrev );
    if( itr == m_options.get< ABB >().end() )
    {
        std::stringstream err;
        err << "leaf::wrapper::daycent::file::F_100::GetOption( \"" << abbrev
            << "\" ) could not find specified option in Options map!";

        throw std::bad_exception( err.str().c_str() );
    }

    return itr;
}
////////////////////////////////////////////////////////////////////////////////
void F_100::GenerateKey(
    int const& vk,
    bool const& extended )
{
    BOOL bExtended = extended ? TRUE : FALSE;

    KEYBDINPUT kb = { 0 };
    INPUT input = { 0 };

    //Generate a "key down"
    if( bExtended ) kb.dwFlags = KEYEVENTF_EXTENDEDKEY;
    kb.wVk  = vk;
    input.type  = INPUT_KEYBOARD;
    input.ki  = kb;
    SendInput( 1, &input, sizeof( input ) );

    //Generate a "key up"
    ZeroMemory( &kb, sizeof( KEYBDINPUT ) );
    ZeroMemory( &input, sizeof( INPUT ) );
    kb.dwFlags = KEYEVENTF_KEYUP;
    if( bExtended ) kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    kb.wVk = vk;
    input.type = INPUT_KEYBOARD;
    input.ki = kb;
    SendInput( 1, &input, sizeof( input ) );

    return;
}
////////////////////////////////////////////////////////////////////////////////
void F_100::GenerateKeys(
    std::string const& s )
{
    std::locale loc;
    for( std::string::size_type i = 0; i < s.length(); ++i )
    {
        char c = s[ i ];
        bool up = std::isupper( c, loc );
        if( up ) GenerateKey( VK_CAPITAL, true );
        else c = std::toupper( c, loc );
        GenerateKey( c );
        if( up ) GenerateKey( VK_CAPITAL, true );
    }
}
////////////////////////////////////////////////////////////////////////////////
Option const& F_100::GetOption(
    std::string const& abbrev ) const
{
    return *Find( abbrev );
}
////////////////////////////////////////////////////////////////////////////////
Options const& F_100::GetOptions() const
{
    return m_options;
}
////////////////////////////////////////////////////////////////////////////////
void F_100::Init(
    std::string const& dirRoot )
{
    m_dirRoot = dirRoot;
}
////////////////////////////////////////////////////////////////////////////////
F_100* F_100::instance(
    bool const& erase )
{
    static F_100* sF100 = new F_100();
    if( erase )
    {
        sF100->Exit();
        delete sF100;
        sF100 = NULL;
    }

    return sF100;
}
////////////////////////////////////////////////////////////////////////////////
bool F_100::OpenFile(
    unsigned int const& type,
    std::string const& path,
    std::string const& abbr )
{
    assert( !m_fileIsOpen );
    fs::path cp( fs::current_path() );
    fs::path temp( cp/path );
    fs::current_path( temp );



    // -------------------- WINDOWS ONLY!!! -------------------- //
    //Hide user input
    /*HANDLE hStdin = GetStdHandle( STD_INPUT_HANDLE );
    DWORD mode = 0;
    GetConsoleMode( hStdin, &mode );
    SetConsoleMode( hStdin, mode & ( ~ENABLE_ECHO_INPUT ) );
    //Redirect stdout
    int stdout_dupfd = _dup( 1 );
    FILE* temp_out = fopen( "NUL:", "w" );
    _dup2( _fileno( temp_out ), 1 );*/

    //Enter keyboard input; this is stupid
    if( type == SITE )
    {
        //Enter the site file name:
        GenerateKeys( abbr );
        GenerateKey( VK_RETURN );
        //Enter a new site file name to save changes
        //to or a <return> to save to site.100:
        GenerateKey( VK_RETURN );
    }
    bool success =
        ( openfiles( static_cast< int >( type ), (char*)abbr.c_str() ) != 0 );

    //Restore stdout
    /*fflush( stdout );
    fclose( temp_out );
    _dup2( stdout_dupfd, 1 );
    _close( stdout_dupfd );
    //Show user input
    SetConsoleMode( hStdin, mode );*/

    success = success && ReadOptions( type );
    if( success ) m_fileIsOpen = true;
    fs::current_path( cp );
    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool F_100::ReadOptions(
    unsigned int const& type )
{
    int numparms = lines[ type ];
    int head;
    int headlines[ HEADERS ];
    char abbrev[ SWORD ]; abbrev[ 0 ] = 0;
    char descript[ BWORD ];
    char value[ MAXVALS ][ MWORD ];
    char name[ MAXVALS ][ MWORD ];
    char heading[ HEADERS ][ BWORD ];

    while( readoption(
        type, &head, headlines, abbrev,
        descript, value, name, heading ) == FALSE )
    {
        if( abbrev[ 0 ] == 0 ) continue;
        Option option( numparms, type, head, headlines,
            abbrev, descript, value, name, heading );
        AddOption( option );
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////
void F_100::Weather(
    std::string const& abbr,
    std::string const& path )
{
    // -------------------- WINDOWS ONLY!!! -------------------- //
    //Hide user input
    /*HANDLE hStdin = GetStdHandle( STD_INPUT_HANDLE );
    DWORD mode = 0;
    GetConsoleMode( hStdin, &mode );
    SetConsoleMode( hStdin, mode & ( ~ENABLE_ECHO_INPUT ) );
    //Redirect stdout
    int stdout_dupfd = _dup( 1 );
    FILE* temp_out = fopen( "NUL:", "w" );
    _dup2( _fileno( temp_out ), 1 );*/

    //Enter keyboard input; this is stupid
    //Enter the name of the actual weather file:
    fs::path cp( fs::current_path() );
    fs::path temp( cp/path );
    fs::current_path( temp );

    GenerateKeys( abbr );
    GenerateKey( VK_RETURN );
    GenerateKeys( abbr );
    GenerateKey( VK_RETURN );
    GenerateKey( VK_RETURN );

    weather( (char*)(path.c_str()), (char*)(path.c_str()) );

    fs::current_path( cp );

    //Restore stdout
    /*fflush( stdout );
    fclose( temp_out );
    _dup2( stdout_dupfd, 1 );
    _close( stdout_dupfd );
    //Show user input
    SetConsoleMode( hStdin, mode );*/
}
////////////////////////////////////////////////////////////////////////////////
void F_100::WriteOptions() const
{
    OptBySeq::const_iterator itr = m_options.get< SEQ >().begin();
    for( ; itr != m_options.get< SEQ >().end(); ++itr )
    {
        Option const& option = *itr;
        option.Write();
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end file
} //end daycent
} //end wrapper
} //end leaf
