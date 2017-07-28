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
#include <leaf/wrapper/daycent/out/OutFiles.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>

#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

// --- STL Includes --- //
#include <iostream>
#include <sstream>
#include <fstream>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace out
{

////////////////////////////////////////////////////////////////////////////////
OutFiles::OutFiles()
    :
    m_dirRoot()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
OutFiles::~OutFiles()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void OutFiles::Exit()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void OutFiles::Init(
    std::string const& dirRoot,
    unsigned int const& fileNames )
{
    m_dirRoot = dirRoot;
    m_outFilesInPath = m_dirRoot/OutFilesName;

    //Reconcile with input file
    std::ifstream outFilesIn( m_outFilesInPath.c_str() );
    assert( outFilesIn.is_open() );

    //
    File file;
    unsigned int allFileNames( 0 );
    std::getline( outFilesIn, m_header );
    while( outFilesIn >> file.output, outFilesIn >> file.name )
    {
        outFilesIn.ignore(
            std::numeric_limits< std::streamsize >::max(), '\n' );
        file.id = Id( file.name );
        m_files.push_back( file );
        allFileNames |= file.id;
    }
    assert( allFileNames == OUTPUT );
    outFilesIn.close();

    Set( fileNames );
}
////////////////////////////////////////////////////////////////////////////////
OutFiles* OutFiles::instance(
    bool const& erase )
{
    static OutFiles* sOutFiles = new OutFiles();
    if( erase )
    {
        sOutFiles->Exit();
        delete sOutFiles;
        sOutFiles = NULL;
    }

    return sOutFiles;
}
////////////////////////////////////////////////////////////////////////////////
void OutFiles::Set(
    unsigned int const& fileNames )
{
    m_fileNames = fileNames;
    FilesById::const_iterator idItr = m_files.get< ID >().begin();
    for( ; idItr != m_files.get< ID >().end(); ++idItr )
    {
        File const& file = *idItr;
        if( file.id & DEFAULT ) continue;
        file.output = m_fileNames & file.id;
    }

    std::ofstream outFilesIn(
        m_outFilesInPath.c_str(), std::ios::out | std::ios::trunc );
    assert( outFilesIn.is_open() );
    outFilesIn << m_header << std::endl;
    FilesBySeq::const_iterator seqItr = m_files.get< SEQ >().begin();
    for( ; seqItr != m_files.get< SEQ >().end(); ++seqItr )
    {
        outFilesIn << *seqItr;
    }
    outFilesIn.close();
}
////////////////////////////////////////////////////////////////////////////////
void OutFiles::Read()
{
    FilesById::const_iterator idItr = m_files.get< ID >().begin();
    for( ; idItr != m_files.get< ID >().end(); ++idItr )
    {
        File const& file = *idItr;

    }
}
////////////////////////////////////////////////////////////////////////////////

} //end out
} //end daycent
} //end wrapper
} //end leaf
