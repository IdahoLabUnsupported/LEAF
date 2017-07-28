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


#pragma once

// --- LEAF Includes --- //
#include <leaf/LEAFConfig.h>
#include <leaf/open/ero/Erosion.h>

// --- POCO Includes --- //
#include <Poco/Data/Session.h>
#include <Poco/SharedPtr.h>

// --- STL Includes --- //
#include <vector>
#include <string>

namespace leaf
{
namespace wrapper
{
namespace daycent
{

class PostProcessor
{
public:
    ///
    PostProcessor(void);

    ///
    ~PostProcessor(void);

    ///
    static PostProcessor* instance(
        bool const& erase = false );

    ///
    std::vector< double > GetResults(
        std::string const& abbr,
        std::string const& file );

    ///
    std::vector< double > ParseDataFile( 
        const std::string& file, 
        const std::string& dbTableName );

    ///
    void TEST();

    ///
    void RegisterDB();

    ///
    std::vector< std::string > GetFilesInDirectory( 
        const std::string& root, 
        const std::string& extenstion );

    ///
    void WriteToDB( 
        std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& rem,
        std::vector< std::string > remreg,
        std::string const& rotreg,
        std::vector< std::string > cropVec,
        std::vector< std::string > yldreg,
        std::vector< std::string > eroVec );

    void PostProcessor::WriteToAdlerDB( 
        std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& remreg,
        std::string const& rotreg,
        std::string const& yldreg );

    ///
    void WriteToGHGDB( 
        std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& remreg,
        std::string const& rotreg,
        std::string const& yldreg );

    ///
    void CreateOutDB( 
        std::string const& file );

    ///
    void CreateVSWCDB( 
        std::string const& file );

    ///
    void PostProcessor::CreateSustDB(); 
        /*std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& remreg,
        std::string const& rotreg,
        std::string const& yldreg );*/

    ///
    void WriteNDB(
        std::string const& file,
        std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& remreg,
        std::string const& rotreg,
        std::string const& yldreg );

    ///
    void WriteVSWCDB(
        std::string const& file,
        std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& remreg,
        std::string const& rotreg,
        std::string const& yldreg );

    ///
    void PostProcessor::WriteSustDB( 
        std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& remreg,
        std::string const& rotreg,
        std::string const& yldreg,
        leaf::open::ero::ErosionPtr data);

    ///
    void PostProcessor::WriteOutFile(
        std::string const& file,
        std::string const& abbr,
        std::string const& mukey,
        std::string const& cokey,
        std::string const& tillreg,
        std::string const& remreg,
        std::string const& rotreg,
        std::string const& yldreg );

    ///
    void SetField( std::string const& field );

    ///
    void SetYields( 
        std::pair< std::string, std::string > yields );
    std::pair< std::string, std::string > GetYields();

    ///
    std::string const& GetTableName(
        std::string const& file );

    ///
    std::vector< double > GetTime();

    ///
    std::string m_dbFilename;

    ///
    std::string m_results;

    ///
    Poco::SharedPtr< Poco::Data::Session > m_session;

    ///
    std::vector< double > m_time;

    ///
    std::string m_field;

    ///
    std::pair< std::string, std::string > m_yields;

private:
    
};

}//daycent
}//wrapper
}//leaf
