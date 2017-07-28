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


// --- ATL Includes --- //
#include <atlstr.h>
using namespace ATL;

// --- LEAF Includes --- //
#include <leaf/wrapper/rusle2/GdbHelper.h>
#include <leaf/wrapper/rusle2/RomeWrapper.h>

// --- ROME Includes --- //
#include <cstdlib> //This must be included before api-rome.h
#if __RUSLE2_version <= 2000004
#define AFX_API_IMPORT
#endif //__RUSLE2_version
#include <api-rome.h>
#include <rxencode.h>

// --- SQLite Includes --- //
#include <sqlite.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

// --- STL Includes --- //
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

namespace fs = boost::filesystem;

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

////////////////////////////////////////////////////////////////////////////////
bool GdbFileToXml(
    std::string const& gdbFilePath,
    ObjectType const& objType,
    std::string const& virtualFilePath,
    std::string const& newFilePath )
{
    sqlite* db = sqlite_open( gdbFilePath.c_str(), 0, NULL );
    assert( db != NULL );

    std::string fixed = FixUpPath( virtualFilePath, objType );
    std::string path = EncodeXML( fixed );
    std::string table = TBL( objType );
    assert( table == fixed.substr( 0, fixed.find( "\\" ) ) );

    int nrow = 0, ncol = 0;
    char** result = 0;
    std::string format =
        "SELECT path, name, data FROM '%q' WHERE path||'\\'||name = '%q';";
    int ret = sqlite_get_table_printf(
        db, format.c_str(), &result, &nrow, &ncol, NULL,
        table.c_str(), path.c_str() );
    assert( ret == SQLITE_OK );

    assert( nrow == 1 && ncol == 3 );
    int index = 3;
    std::string filePath( result[ index++ ] );
    std::string fileName( result[ index++ ] );
    std::string fileData( result[ index++ ] );
    sqlite_free_table( result );
    sqlite_close( db );

    //std::ios::binary is important to keep file format correct
    std::ofstream ofile( newFilePath.c_str(),
        std::ios::out | std::ios::trunc | std::ios::binary );
    if( !ofile.is_open() )
    {
        return false;
    }

    //Erase old science version
    std::string tag = "<Science>";
    std::string end = "</Science>";
    size_t tagPos = fileData.find( tag );
    size_t endPos = fileData.find( end );
    if( tagPos != std::string::npos && endPos != std::string::npos )
    {
        fileData.erase( tagPos, ( endPos + end.length() ) - tagPos );
    }

    //commonfile.cpp: ExportObject() - line 131
    ofile << "<?xml version='1.0'?>\n";
    tag = "<Obj>";
    fileData.insert(
        fileData.find( tag ) + tag.length(),
        "<Type>" + NAME( objType ) + "</Type>" +
        "<Science>" + RomeWrapper::Instance().GetSciVerStr() + "</Science>" +
        "<Filename>" + filePath + "\\" + fileName + "</Filename>" );
    ofile << DecodeXML( fileData );
    ofile.close();

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool GdbToXml(
    std::string const& gdbFilePath,
    ObjectType const& objType,
    std::string const& newFilePath,
    bool const& useDbStruct )
{
    sqlite* db = sqlite_open( gdbFilePath.c_str(), 0, NULL );
    assert( db != NULL );

    std::string table = TBL( objType );

    int nrow = 0, ncol = 0;
    char** result = 0;
    std::string format = "SELECT path, name, data FROM '%q';";
    int ret = sqlite_get_table_printf(
        db, format.c_str(), &result, &nrow, &ncol, NULL, table.c_str() );
    assert( ret == SQLITE_OK );

    assert( nrow >= 1 && ncol == 3 );
    int index = 3;
    for( int i = 0; i < nrow; ++i )
    {
        std::string filePath( result[ index++ ] );
        std::string fileName( result[ index++ ] );
        std::string fileData( result[ index++ ] );

        //Create the weps directory if it does not exist
        fs::path path( newFilePath );
        fs::path fname( DecodeXML( fileName ) );
        if( useDbStruct )
        {
            fs::path fpath( DecodeXML( filePath ) );
            fs::create_directories( path/fpath );
            path = path/fpath;
        }
        //std::ios::binary is important to keep file format correct
        std::ofstream ofile( ( path/fname ).string().c_str(),
            std::ios::out | std::ios::trunc | std::ios::binary );
        //assert( ofile.is_open() );

        //Erase old science version
        std::string tag = "<Science>";
        std::string end = "</Science>";
        size_t tagPos = fileData.find( tag );
        size_t endPos = fileData.find( end );
        if( tagPos != std::string::npos && endPos != std::string::npos )
        {
            fileData.erase( tagPos, ( endPos + end.length() ) - tagPos );
        }

        //commonfile.cpp: ExportObject() - line 131
        ofile << "<?xml version='1.0'?>\n";
        tag = "<Obj>";
        fileData.insert(
            fileData.find( tag ) + tag.length(),
            "<Type>" + NAME( objType ) + "</Type>" +
            "<Science>" + RomeWrapper::Instance().GetSciVerStr() + "</Science>" +
            "<Filename>" + filePath + "\\" + fileName + "</Filename>" );
        ofile << DecodeXML( fileData );
        ofile.close();
    }

    sqlite_free_table( result );
    sqlite_close( db );
    return true;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > GdbSearch(
    std::string const& gdbFilePath,
    ObjectType const& objType,
    std::string const& whereClause )
{
    std::vector< std::string > paths;

    sqlite* db = sqlite_open( gdbFilePath.c_str(), 0, NULL );
    assert( db != NULL );

    std::string table = TBL( objType );

    int nrow = 0, ncol = 0;
    char** result = 0;
    std::stringstream format;
    format
        << "select\n"
        << "  path,\n"
        << "  name\n"
        << "from '%q'\n"
        << whereClause.c_str() << ";";
    int ret = sqlite_get_table_printf(
        db, format.str().c_str(), &result, &nrow, &ncol, NULL,
        table.c_str() );
    assert( ret == SQLITE_OK );

    assert( nrow >= 1 && ncol == 2 );
    int index = 2;
    for( int i = 0; i < nrow; ++i )
    {
        std::string filePath( result[ index++ ] );
        std::string fileName( result[ index++ ] );

        //Create the weps directory if it does not exist
        fs::path fpath( filePath );
        fs::path fname( fileName );

        paths.push_back( ( fpath/fname ).string() );
    }

    sqlite_free_table( result );
    sqlite_close( db );
    return paths;
}
////////////////////////////////////////////////////////////////////////////////
bool GdbTrim(
    std::string const& gdbFilePath,
    ObjectType const& objType )
{
    sqlite* db = sqlite_open( gdbFilePath.c_str(), 0, NULL );
    assert( db != NULL );

    std::string table = TBL( objType );

    int nrow = 0, ncol = 0;
    char** result = 0;
    std::string format = "SELECT name FROM '%q';";
    int ret = sqlite_get_table_printf(
        db, format.c_str(), &result, &nrow, &ncol, NULL,
        table.c_str() );
    assert( ret == SQLITE_OK );

    assert( nrow >= 1 && ncol == 1 );
    int index = 1;
    for( int i = 0; i < nrow; ++i )
    {
        std::string oldName( result[ index++ ] );
        std::string newName( oldName );
        boost::trim( newName );

        std::string sql = sqlite_mprintf(
            "UPDATE '%q' SET name = '%q' where name = '%q';",
            table.c_str(), newName.c_str(), oldName.c_str() );
        ret = sqlite_exec( db, sql.c_str(), NULL, NULL, NULL );
        assert( ret == SQLITE_OK );

        if( objType == OBJTYP_OPERATION )
        {
            std::string filePath =
                rusle2::TBL( rusle2::OBJTYP_OPERATION ) +
                "\\" + newName;

            //Open operation file
            RT_FileObj* newOp = RomeWrapper::Instance().OpenFile( filePath );
            if( newOp == NULL ) break;
            //assert( newOp != NULL );

            bool success( false );
            unsigned int numOpBldOpPtrs = RomeWrapper::Instance().GetAttrSize(
                newOp, "#RD:OP_OPERATION_BUILDER_PTR:OPERATION_BUILDER_OP_PTR" );
            for( unsigned int j = 0; j < numOpBldOpPtrs; ++j )
            {
                std::string opBldOpPtr = RomeWrapper::Instance().GetAttrVal(
                    newOp,
                    "#RD:OP_OPERATION_BUILDER_PTR:OPERATION_BUILDER_OP_PTR",
                    j );
                boost::trim( opBldOpPtr );
                success = RomeWrapper::Instance().SetAttrVal(
                    newOp,
                    "#RD:OP_OPERATION_BUILDER_PTR:OPERATION_BUILDER_OP_PTR",
                    opBldOpPtr, j );
                assert( success );
            }

            success = RomeWrapper::Instance().SaveFile( newOp, filePath );
            success = RomeWrapper::Instance().CloseFile( newOp );
            //assert( success );
        }
    }

    sqlite_free_table( result );
    sqlite_close( db );
    return true;
}
////////////////////////////////////////////////////////////////////////////////
std::string DecodeXML(
    std::string const& pszEncoded )
{
    return XMLDecode( pszEncoded.c_str() );
}
////////////////////////////////////////////////////////////////////////////////
std::string DecodeXML(
    std::string const& pszEncoded,
    unsigned int const& nFlags )
{
    return XMLDecodeAux( pszEncoded.c_str(), nFlags );
}
////////////////////////////////////////////////////////////////////////////////
bool IsDecodedXML(
    std::string const& pszEncoded )
{
    return XMLIsDecoded( pszEncoded.c_str() );
}
////////////////////////////////////////////////////////////////////////////////
bool IsDecodedXML(
    std::string const& pszEncoded,
    unsigned int const& nFlags )
{
    return XMLIsDecodedAux( pszEncoded.c_str(), nFlags );
}
////////////////////////////////////////////////////////////////////////////////
std::string EncodeXML(
    std::string const& pszDecoded )
{
    return XMLEncode( pszDecoded.c_str() );
}
////////////////////////////////////////////////////////////////////////////////
std::string EncodeXML(
    std::string const& pszDecoded,
    unsigned int const& nFlags )
{
    return XMLEncodeAux( pszDecoded.c_str(), nFlags );
}
////////////////////////////////////////////////////////////////////////////////
bool IsEncodedXML(
    std::string const& pszEncoded )
{
    return XMLIsEncoded( pszEncoded.c_str() );
}
////////////////////////////////////////////////////////////////////////////////
bool IsEncodedXML(
    std::string const& pszEncoded,
    unsigned int const& nFlags )
{
    return XMLIsEncodedAux( pszEncoded.c_str(), nFlags );
}
////////////////////////////////////////////////////////////////////////////////

} //end rusle2
} //end wrapper
} //end leaf
