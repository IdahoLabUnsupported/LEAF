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
#include "PostProcessor.h"
#include <leaf/wrapper/daycent/file/F_header.h>
#include <leaf/wrapper/rusle2/api-rusle2.h>

// --- STL Includes --- //

// --- POCO Includes --- //
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SQLite/Connector.h>

#include <Poco/SharedPtr.h>
#include <Poco/Tuple.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>

// --- Boost Includes --- //
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp> 
#include <boost/filesystem/path.hpp>
#include <boost/assign.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace Poco::Data::Keywords;
using namespace Poco::Data;
using namespace leaf::open;

namespace leaf
{
namespace wrapper
{
namespace daycent
{

////////////////////////////////////////////////////////////////////////////////
PostProcessor::PostProcessor(void)
{
    m_dbFilename = "daycent.db";
    //m_results = "dcBoone.db";
    //m_results = "dcBooneTest.db";
    //m_results = "adler_update.db";
    //m_results = "Boone.db";
    m_results = "db/ars_results.db";
}
////////////////////////////////////////////////////////////////////////////////
PostProcessor::~PostProcessor(void)
{
}
////////////////////////////////////////////////////////////////////////////////
PostProcessor* PostProcessor::instance(
    bool const& erase )
{
    static PostProcessor* sPostProcessor = new PostProcessor();
    if( erase )
    {
        delete sPostProcessor;
        sPostProcessor = NULL;
    }

    return sPostProcessor;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< double > PostProcessor::GetResults( 
    std::string const& abbr,
    std::string const& file )
{
    std::vector< double > yield;
    yield = ParseDataFile( file , abbr );

    return yield;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< double > PostProcessor::ParseDataFile( 
    const std::string& csvFilename,
    const std::string& dbTableName)
{
    std::ifstream infile( csvFilename.c_str() );
    
    infile.seekg( 0, std::ios::end);
    std::streampos length = infile.tellg();
    infile.seekg (0, std::ios::beg);

    char* buffer = new char [ length ];
    infile.read( buffer, (length) );
    infile.close();

    std::string networkData( buffer );
    delete [] buffer;
    boost::algorithm::replace_all( networkData, "\r", "" );
    
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tok;
    boost::escaped_list_separator<char> sep( "", " \n", "");
    Tok tok( networkData, sep );
    std::string tempTok;
    double tempDouble = 0;
    size_t columnCount1 = 0;
    std::vector< std::string > columnNames;
    Tok::iterator firstDouble;
    for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
    {
        tempTok = *tok_iter;
        if( tempTok.empty() )
        {
            continue;
        }
        //std::cout << "<" << tempTok << "> ";
            
        try
        {
            tempDouble = boost::lexical_cast<double>( tempTok );
            firstDouble = tok_iter;
            break;
            //std::cout << tempDouble << " "; 
        }
        catch( boost::bad_lexical_cast& ex )
        {
            //std::cout << tempTok << " ";
            columnNames.push_back( tempTok );
            columnCount1 +=1;
        }
    }
    std::cout << "Column Count " << columnCount1 << std::endl;
    
    //////////////////////////////
    //////////////////////////////

    Poco::Data::Session session("SQLite", m_dbFilename );
    //manage open and closing the session our self so that the population of the
    //db is faster
    session.begin();
    // drop sample table, if it exists
    std::ostringstream dropTable;
    dropTable << "DROP TABLE IF EXISTS " << dbTableName;
    session << dropTable.str().c_str(), now;
    
    std::ostringstream createCommand;
    createCommand << "CREATE TABLE " << dbTableName << " (";
    for( size_t i = 0; i < columnNames.size(); ++i )
    {
        createCommand << "'" << columnNames.at( i ) << "' DOUBLE";
        
        if( i < columnCount1 - 1 )
        {
            createCommand << ",";
        }
    }
    createCommand << ")";
    
    //try
    //{
        session << createCommand.str(), now;
    //}
    //catch( Poco::Data::DataException& ex )
    //{
    //    std::cout << ex.displayText() << std::endl;
    //    return;
    //}
    
    //////////////////////////////
    //////////////////////////////
    
    size_t columnCounter = 0;
    std::ostringstream insertCommand;
    for(Tok::iterator tok_iter = firstDouble; tok_iter != tok.end(); ++tok_iter)
    {
        tempTok = *tok_iter;
        if( tempTok.empty() )
        {
            continue;
        }
        //std::cout << "<" << tempTok << "> ";

        try
        {
            tempDouble = boost::lexical_cast<double>( tempTok );
            if( columnCounter == 0 )
            {
                insertCommand << "INSERT INTO " << dbTableName << " VALUES(";
            }
        }
        catch( boost::bad_lexical_cast& ex )
        {
            ;
        }
        
        insertCommand << tempDouble;
    
        if( columnCounter < columnCount1 - 1 )
        {
            insertCommand << ",";
            ++columnCounter;
        }
        else
        {
            insertCommand << ")";
            //std::cout << insertCommand.str() << std::endl;
            Poco::Data::Statement insert( session );
            try
            {
                insert << insertCommand.str(), now;
            }
            catch( Poco::Data::DataException& ex )
            {
                std::cout << ex.displayText() << std::endl;
            }
            insertCommand.str("");
            columnCounter = 0;
        }
    }

    std::vector< double > yield;
    std::vector< double > time;

    Statement stmt = ( session
        << "select\n"
        << "    t1.cgrain\n"
        << "from [" << dbTableName << "] as t1",
        into( yield ) );
        //useRef( abbr ) );

    stmt.execute();

    //Now close the session to match the previous being statement
    session.commit();   

    return yield;
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::RegisterDB()
{
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();
    //
}
////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> PostProcessor::GetFilesInDirectory( 
    const std::string& root, 
    const std::string& extension )
{
    boost::filesystem::path dir_path( root.c_str() );
    std::list< std::string > filesInDir;
    try
    {
        if( boost::filesystem::is_directory( dir_path ) )
        {
            boost::filesystem::directory_iterator end_iter;
            for( boost::filesystem::directory_iterator dir_itr( dir_path );
                dir_itr != end_iter; ++dir_itr )
            {
                try
                {
                    if( dir_itr->path().extension() == extension )
                    {
                        std::string pathAndFileName;
                        pathAndFileName.assign( dir_path.string() );
                        pathAndFileName.append( "/" );
                        pathAndFileName.append( dir_itr->path().filename().string() );
                        
                        filesInDir.push_back( pathAndFileName );
                    }
                    else if( fs::is_directory( dir_itr->status() ) )
                    {
                        std::vector<std::string> tempFiles = 
                            GetFilesInDirectory( dir_itr->path().string(), extension );
                        for( size_t j = 0; j < tempFiles.size(); ++j )
                        {
                            filesInDir.push_back( tempFiles.at( j ) );
                        }
                    }
                }
                catch( const std::exception& ex )
                {
                    std::cout << ex.what() << std::endl;
                }
            }
        }
    }
    catch( const std::exception& ex )
    {
        std::cout << ex.what() << std::endl;
    }
    filesInDir.sort();
    
    std::vector< std::string > filesList;
    for( std::list< std::string >::const_iterator iter = filesInDir.begin(); iter != filesInDir.end(); ++iter )
    {
        filesList.push_back( *iter );
    }
    
    return filesList;
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::WriteToDB( 
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& rem,
    std::vector< std::string > remVec,
    std::string const& rotreg,
    std::vector< std::string > cropVec,
    std::vector< std::string > yldreg,
    std::vector< std::string > eroVec )
{
    fs::path cp( fs::current_path() );

    //Open the file and parse it
    //Add the data from each .lis file to an .lis specific table in a db
    RegisterDB();

    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );
    std::vector< std::string > resultFiles;
    std::string baseline = "resultsBL";
    std::string sim = "results50";
    resultFiles.push_back( baseline );
    resultFiles.push_back( sim );
    std::string dbTableName = "lis";

    //std::string file( "./daycent/switchgrass_" + ck + ".lis" );
    std::string file( "./daycent/" + abbr + "_" + ck + ".lis" );
    std::cout << cp.string() << " " << file << std::endl;
    std::ifstream infile( file.c_str() );
    
    infile.seekg( 0, std::ios::end);
    std::streampos length = infile.tellg();
    infile.seekg (0, std::ios::beg);

    char* buffer = new char [ length ];
    infile.read( buffer, (length) );
    infile.close();

    std::string networkData( buffer );
    delete [] buffer;
    boost::algorithm::replace_all( networkData, "\r", "" );
    
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tok;
    boost::escaped_list_separator<char> sep( "", " \n", "");
    Tok tok( networkData, sep );
    std::string tempTok;
    double tempDouble = 0;
    size_t columnCount1 = 0;
    std::vector< std::string > columnNames;
    Tok::iterator firstDouble;
    for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
    {
        tempTok = *tok_iter;
        if( tempTok.empty() )
        {
            continue;
        }
        //std::cout << "<" << tempTok << "> ";
            
        try
        {
            tempDouble = boost::lexical_cast<double>( tempTok );
            firstDouble = tok_iter;
            break;
            //std::cout << tempDouble << " "; 
        }
        catch( boost::bad_lexical_cast& ex )
        {
            //std::cout << tempTok << " ";
            columnNames.push_back( tempTok );
            columnCount1 +=1;
        }
    }
    
    std::cout << "Column Count " << columnCount1 << std::endl;

    //////////////////////////////
    //////////////////////////////

    Poco::Data::Session session("SQLite", m_results );
    //manage open and closing the session our self so that the population of the
    //db is faster
    session.begin();
    // drop sample table, if it exists
    //std::ostringstream dropTable;
    //dropTable << "DROP TABLE IF EXISTS " << dbTableName;
    //session << dropTable.str().c_str(), now;

    m_session = new Poco::Data::Session( "SQLite", m_results );

    std::ostringstream createCommand;            
    createCommand << "CREATE TABLE IF NOT EXISTS [" << dbTableName
        << "]( [abbr] VARCHAR( 50 ) NOT NULL,"
        << " [mukey]  VARCHAR( 50 ) NOT NULL,"
        << " [cokey]  VARCHAR( 50 ) NOT NULL,"
        << " [tillreg]  VARCHAR( 50 ) NOT NULL,"
        << " [remreg]  VARCHAR( 50 ) NOT NULL,"
        << " [rotreg]  VARCHAR( 50 ) NOT NULL,"
        << " [crop]  VARCHAR( 50 ) NOT NULL,"
        << " [yldstart]  VARCHAR( 50 ) NOT NULL,"
        << " [yldend]  VARCHAR( 50 ) NOT NULL,"
        << " [yldreg]  VARCHAR( 50 ) NOT NULL,"
        << " [name]  VARCHAR( 50 ) NOT NULL,";
    for( size_t i = 0; i < columnNames.size(); ++i )
    {
        createCommand << "[" << columnNames.at( i ) << "] DOUBLE";
        
        if( i < columnCount1 - 1 )
        {
            createCommand << ",";
        }
    }
    createCommand << ",";
    createCommand << " UNIQUE( [abbr], [mukey], [cokey],\n"
                  << " [tillreg], [remreg], [rotreg], [crop], [yldstart], [yldend], [yldreg], [name], [time] )\n"
                  << "  ON CONFLICT IGNORE )";

    //std::cout << createCommand.str() << std::endl;

    Poco::Data::Statement stmt = ( *m_session
        << createCommand.str(), now );

    stmt.execute();

    //////////////////////////////
    //////////////////////////////

    std::ifstream lisFile;
    lisFile.open( file.c_str() , std::ios::out );
    std::string line;
    std::vector< std::vector< std::string > > data;
    while( getline( lisFile, line ) )
    {
        std::vector< std::string > linedata;
        std::istringstream iss( line );
        std::copy( std::istream_iterator< std::string >( iss ),
            std::istream_iterator< std::string >(),
            std::back_inserter< std::vector< std::string > >(linedata) );
        //boost::algorithm::split( linedata, line, boost::algorithm::is_any_of("\t") );
        data.push_back( linedata );
    }
    size_t columnCounter = 0;
    std::ostringstream insertCommand;
    unsigned int counter(0);
    bool remCounter( false );
    double year(0);
    double year1(0);

    for(Tok::iterator tok_iter = firstDouble; tok_iter != tok.end(); ++tok_iter)
    {
        tempTok = *tok_iter;
        if( tempTok.empty() )
        {
            continue;
        }
        //std::cout << "<" << tempTok << "> ";

        //std::string ck = cokey; ck.replace( ck.find( "-" ), 1, ":" );
        try
        {
            tempDouble = boost::lexical_cast<double>( tempTok );
            if( !remCounter )
            {
                if( columnCounter == 0 )
                {
                    insertCommand << "INSERT INTO " << dbTableName << " VALUES('";
                    insertCommand << abbr << "','" << mukey << "','" << cokey << "','" << tillreg << "','";
                    insertCommand << "NRH" << "','" << rotreg << "','" << "" << "','" << "" << "','" << "" << "','" << "" << "','" << m_field << "',";
                    if( tempDouble == 2011.00 )
                    {
                        remCounter = true;
                        year = tempDouble;
                    }
                }
            }
            else
            {                
                if( columnCounter == 0 )
                {
                    if( year != tempDouble )
                    {
                        insertCommand << "INSERT INTO " << dbTableName << " VALUES('";
                        insertCommand << abbr << "','" << mukey << "','" << cokey << "','" << tillreg << "','";
                        insertCommand << remVec.at( counter ) << "','" << rotreg << "','" << cropVec.at( counter ) << "','" << m_yields.first << "','" << m_yields.second << "','";
                        insertCommand << yldreg.at( counter ) << "','" << m_field << "',";
                        counter += 1;
                        year = tempDouble;
                    }
                    else if( counter == remVec.size() )
                    {
                        break;
                    }
                }
            }
        }
        catch( boost::bad_lexical_cast& ex )
        {
            ;
        }
        
        insertCommand << tempDouble;
        
    
        if( columnCounter < columnCount1 - 1 )
        {
            insertCommand << ",";
            ++columnCounter;
        }
        else
        {
            insertCommand << ")";
            if( year != year1 )
            {
                std::cout << insertCommand.str() << std::endl;

                Poco::Data::Statement stmt = ( *m_session
                    << insertCommand.str(), now );
            }
            /*Poco::Data::Statement insert( session );

            try
            {
                insert << insertCommand.str(), now;
            }
            catch( Poco::Data::DataException& ex )
            {
                std::cout << ex.displayText() << std::endl;
            }*/
            insertCommand.str("");
            columnCounter = 0;
            year1 = year;
        }
    }

    fs::current_path( cp );
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::WriteToAdlerDB( 
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg )
{
    fs::path cp( fs::current_path() );
    //fs::current_path( "X:\\USDA\\AdlerData" );
    std::string root( "./daycent/" + abbr + "/" + tillreg + "/"
         + rotreg + "/" + remreg + "/" + abbr + "_" + cokey );
    std::cout<< root << std::endl;
    std::string extension( ".lis" );
    //Iterate over all of the directories
    //Find if a file has a .lis extension
    std::vector< std::string > lisFiles = GetFilesInDirectory( root, extension );
    //Open the file and parse it
    //Add the data from each .lis file to an .lis specific table in a db
    RegisterDB();
    //Iterate over the files
    for( size_t i = 0; i < lisFiles.size(); ++i )
    {
        /*std::string dbTableName = lisFiles.at( i );
        boost::algorithm::replace_first( dbTableName, root, "" );
        boost::algorithm::replace_last( dbTableName, extension, "" );
        boost::algorithm::replace_all( dbTableName, "/", "" );
        //boost::algorithm::replace_all( dbTableName, " ", "_" );
        std::cout << dbTableName << " " << lisFiles.at( i ) << std::endl;*/

        std::vector< std::string > test;
        boost::algorithm::split( test, lisFiles.at(i), boost::algorithm::is_any_of("/") );
        std::string abbr( test.back().substr(0, test.back().find(".") ) );
        std::string mukey( test.at(4).substr(6, 6) );
        std::string cokey( test.at(4).substr(6) );
        std::string tillreg( test.at(2) );
        std::string remreg( test.at(1) );
        std::string rotreg( test.at(3) );
        std::cout << abbr << " " << mukey << " " << cokey << " " << tillreg << " " << remreg << " " << rotreg << std::endl;

        std::vector< std::string > resultFiles;
        std::string baseline = "resultsBL";
        std::string sim = "results50";
        resultFiles.push_back( baseline );
        resultFiles.push_back( sim );
        std::string dbTableName = "sim_update";
    
        std::ifstream infile( lisFiles.at( i ).c_str() );
        
        infile.seekg( 0, std::ios::end);
        std::streampos length = infile.tellg();
        infile.seekg (0, std::ios::beg);

        char* buffer = new char [ length ];
        infile.read( buffer, (length) );
        infile.close();

        std::string networkData( buffer );
        delete [] buffer;
        boost::algorithm::replace_all( networkData, "\r", "" );
        
        typedef boost::tokenizer< boost::escaped_list_separator<char> > Tok;
        boost::escaped_list_separator<char> sep( "", " \n", "");
        Tok tok( networkData, sep );
        std::string tempTok;
        double tempDouble = 0;
        size_t columnCount1 = 0;
        std::vector< std::string > columnNames;
        Tok::iterator firstDouble;
        for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
        {
            tempTok = *tok_iter;
            if( tempTok.empty() )
            {
                continue;
            }
            //std::cout << "<" << tempTok << "> ";
                
            try
            {
                tempDouble = boost::lexical_cast<double>( tempTok );
                firstDouble = tok_iter;
                break;
                //std::cout << tempDouble << " "; 
            }
            catch( boost::bad_lexical_cast& ex )
            {
                //std::cout << tempTok << " ";
                columnNames.push_back( tempTok );
                columnCount1 +=1;
            }
        }
        
        std::cout << "Column Count " << columnCount1 << std::endl;

        //////////////////////////////
        //////////////////////////////

        Poco::Data::Session session("SQLite", m_results );
        //manage open and closing the session our self so that the population of the
        //db is faster
        session.begin();
        // drop sample table, if it exists
        //std::ostringstream dropTable;
        //dropTable << "DROP TABLE IF EXISTS " << dbTableName;
        //session << dropTable.str().c_str(), now;

        m_session = new Poco::Data::Session( "SQLite", m_results );

        /*Poco::Data::Statement stmt = ( *m_session 
            << "CREATE TABLE IF NOT EXISTS [test](\n"
            << "[abbr] VARCHAR( 50 ) NOT NULL)", now );

        stmt.execute();*/
        for( size_t a=0; a<resultFiles.size(); ++a )
        {
        
            std::ostringstream createCommand;
            /*createCommand << "if object_id('results') is null \*/
            
            createCommand << "CREATE TABLE IF NOT EXISTS [" << dbTableName
                << "]( [abbr] VARCHAR( 50 ) NOT NULL,"
                << " [mukey]  VARCHAR( 50 ) NOT NULL,"
                << " [cokey]  VARCHAR( 50 ) NOT NULL,"
                << " [tillreg]  VARCHAR( 50 ) NOT NULL,"
                << " [remreg]  VARCHAR( 50 ) NOT NULL,"
                << " [rotreg]  VARCHAR( 50 ) NOT NULL,"
                << " [yldreg]  VARCHAR( 50 ) NOT NULL,";
            for( size_t i = 0; i < columnNames.size(); ++i )
            {
                createCommand << "[" << columnNames.at( i ) << "] DOUBLE";
                
                if( i < columnCount1 - 1 )
                {
                    createCommand << ",";
                }
            }
            createCommand << ",";
            createCommand << " UNIQUE( [abbr], [mukey], [cokey],\n"
                          << " [tillreg], [remreg], [rotreg], [yldreg], [time] )\n"
                          << "  ON CONFLICT IGNORE )";

            //std::cout << createCommand.str() << std::endl;

            Poco::Data::Statement stmt = ( *m_session
                << createCommand.str(), now );

            stmt.execute();

        }
        //////////////////////////////
        //////////////////////////////

        std::ifstream lisFile;
        lisFile.open( lisFiles.at( i ).c_str() , std::ios::out );
        std::string line;
        std::vector< std::vector< std::string > > data;
        while( getline( lisFile, line ) )
        {
            std::vector< std::string > linedata;
            std::istringstream iss( line );
            std::copy( std::istream_iterator< std::string >( iss ),
                std::istream_iterator< std::string >(),
                std::back_inserter< std::vector< std::string > >(linedata) );
            //boost::algorithm::split( linedata, line, boost::algorithm::is_any_of("\t") );
            data.push_back( linedata );
        }

        std::vector< std::vector< std::string > >::iterator itr = data.begin();
        for( ; itr != data.end(); ++itr )
        {
            if( !(*itr).empty() )
            {
                //std::string dbTableName;
                if( (*itr).at(0) == "2061.00" )
                /*if( (*itr).at(0) == "2011.00" )
                {
                    //dbTableName = resultFiles.at(0);
                }
                else if( (*itr).at(0) == "2061.00" )
                {
                    //dbTableName = resultFiles.at(1);
                }*/

                //if( (*itr).at(0) == "2011.00" || (*itr).at(0) == "2061.00" )
                {
                    try
                    {
                        //tempDouble = boost::lexical_cast<double>( tempTok );
                        double time = boost::lexical_cast<double>( (*itr).at(0) ); 
                        double cgrain = boost::lexical_cast<double>( (*itr).at(1) );
                        double agcacc = boost::lexical_cast<double>( (*itr).at(2) );
                        double agcprd = boost::lexical_cast<double>( (*itr).at(3) ); 
                        double crmvst = boost::lexical_cast<double>( (*itr).at(4) ); 
                        double accrst = boost::lexical_cast<double>( (*itr).at(5) ); 
                        double bgcmacc = boost::lexical_cast<double>( (*itr).at(6) ); 
                        double bgcjacc = boost::lexical_cast<double>( (*itr).at(7) ); 
                        double somtc = boost::lexical_cast<double>( (*itr).at(8) ); 
                        double somsc = boost::lexical_cast<double>( (*itr).at(9) );
                        double totsysc = boost::lexical_cast<double>( (*itr).at(10) );
                        double esrsnk_1 = boost::lexical_cast<double>( (*itr).at(11) ); 
                        double nfixac = boost::lexical_cast<double>( (*itr).at(12) ); 
                        double wdfx = boost::lexical_cast<double>( (*itr).at(13) ); 
                        double fertot_1 = boost::lexical_cast<double>( (*itr).at(14) );
                        double eprodc_1 = boost::lexical_cast<double>( (*itr).at(15) ); 
                        double eprodf_1 = boost::lexical_cast<double>( (*itr).at(16) ); 
                        double ereta_1 = boost::lexical_cast<double>( (*itr).at(17) );
                        double crpstg_1 = boost::lexical_cast<double>( (*itr).at(18) ); 
                        double forstg_1 = boost::lexical_cast<double>( (*itr).at(19) );
                        double eupacc_1 = boost::lexical_cast<double>( (*itr).at(20) );
                        double ermvst_1 = boost::lexical_cast<double>( (*itr).at(21) ); 
                        double egrain_1 = boost::lexical_cast<double>( (*itr).at(22) ); 
                        double terem_1 = boost::lexical_cast<double>( (*itr).at(23) ); 
                        double sdrmae_1 = boost::lexical_cast<double>( (*itr).at(24) ); 
                        double shrmae_1 = boost::lexical_cast<double>( (*itr).at(25) ); 
                        double stream_2 = boost::lexical_cast<double>( (*itr).at(26) );
                        double stream_6 = boost::lexical_cast<double>( (*itr).at(27) );;
                        double volpac = boost::lexical_cast<double>( (*itr).at(28) ); 
                        double tminrl_1 = boost::lexical_cast<double>( (*itr).at(29) ); 
                        double somte_1 = boost::lexical_cast<double>( (*itr).at(30) );
                        double aglive_1 = boost::lexical_cast<double>( (*itr).at(31) ); 
                        double bgmlive_1 = boost::lexical_cast<double>( (*itr).at(32) );
                        double bgjlive_1 = boost::lexical_cast<double>( (*itr).at(33) );
                        double woode_1 = boost::lexical_cast<double>( (*itr).at(34) );
                        double frste_1 = boost::lexical_cast<double>( (*itr).at(35) ); 
                        double fsyse_1 = boost::lexical_cast<double>( (*itr).at(36) ); 
                        double totale_1 = boost::lexical_cast<double>( (*itr).at(37) ); 
                        double metabe_1_1 = boost::lexical_cast<double>( (*itr).at(38) ); 
                        double metabe_2_1 = boost::lexical_cast<double>( (*itr).at(39) ); 
                        double struce_1_1 = boost::lexical_cast<double>( (*itr).at(40) );
                        double struce_2_1 = boost::lexical_cast<double>( (*itr).at(41) ); 
                        double stdede_1 = boost::lexical_cast<double>( (*itr).at(42) );

                        Poco::Data::Statement insertstmt = ( *m_session 
                          << "INSERT INTO [" << dbTableName << "]\n"
                            //<< " abbr, mukey, cokey, tillreg, remreg, rotreg, yldreg, time, agcacc,\n"
                            //<< " bgcjacc, bgcmacc, agcprd, bgcjprd, bgcmprd, cgrain, hi,\n"
                            //<< " ast1c2, ast2c2, amt1c2, amt2c2, as11c2, as12c2, as21c2, as22c2, as3c2,\n"
                            //<< " n2oacc, tcnpro, stream(2), strlig(1), strlig(2), accrst, ermvst(1),\n"
                            //<< " somsc, somtc)\n"
                            << " VALUES(\n"
                            << " :abbr, :mukey, :cokey, :tillreg, :remreg, :rotreg, :yldreg, :time, :cgrain, :agcacc, \n"
                            << " :agcprd, :crmvst, :accrst, :bgcmacc, :bgcjacc, :somsc, :somtc, :totsysc, :esrsnk_1,\n"
                            << " :nfixac, :wdfx, :fertot_1, :eprodc_1, :eprodf_1, :ereta_1, :crpstg_1, :forstg_1,\n"
                            << " :eupacc_1, :ermvst_1, :egrain_1, :terem_1, :sdrmae_1, :shrmae_1, :stream_2, :stream_6,\n"
                            << " :volpac, :tminrl_1, :somte_1, :aglive_1, :bgmlive_1, :bgjlive_1, :woode_1, :frste_1, :fsyse_1\n"
                            << " :totale_1, :metabe_1_1, :metabe_2_1, :struce_1_1, :struce_2_1, :stdede_1, \n"
                            << "  )",
                            useRef( abbr ),
                            useRef( mukey ),
                            useRef( cokey ),
                            useRef( tillreg ),
                            useRef( remreg ),
                            useRef( rotreg ),
                            useRef( yldreg ),
                            useRef( time ),
                            useRef( cgrain ),
                            useRef( agcacc ),
                            useRef( agcprd ),
                            useRef( crmvst ),
                            useRef( accrst ),
                            useRef( bgcmacc ),
                            useRef( bgcjacc ),
                            useRef( somsc ),
                            useRef( somtc ),
                            useRef( totsysc ),
                            useRef( esrsnk_1 ),
                            useRef( nfixac ),
                            useRef( wdfx ),
                            useRef( fertot_1 ),
                            useRef( eprodc_1 ),
                            useRef( eprodf_1 ),
                            useRef( ereta_1 ),
                            useRef( crpstg_1 ),
                            useRef( forstg_1 ),
                            useRef( eupacc_1 ),
                            useRef( ermvst_1 ),
                            useRef( egrain_1 ),
                            useRef( terem_1 ),
                            useRef( sdrmae_1 ),
                            useRef( shrmae_1 ),
                            useRef( stream_2 ),
                            useRef( stream_6 ),
                            useRef( volpac ),
                            useRef( tminrl_1 ),
                            useRef( somte_1 ),
                            useRef( aglive_1 ),
                            useRef( bgmlive_1 ),
                            useRef( bgjlive_1 ),
                            useRef( woode_1 ),
                            useRef( frste_1 ),
                            useRef( fsyse_1 ),
                            useRef( totale_1 ),
                            useRef( metabe_1_1 ),
                            useRef( metabe_2_1 ),
                            useRef( struce_1_1 ),
                            useRef( struce_2_1 ),
                            useRef( stdede_1 ),
                            now );
                    }
                    catch( boost::bad_lexical_cast& ex )
                    {
                        ;
                    }
                  
                    /*if(columnCount1 == 28 )
                    {
                    try
                    {
                        //tempDouble = boost::lexical_cast<double>( tempTok );
                        double time = boost::lexical_cast<double>( (*itr).at(0) ); 
                        double agcacc = boost::lexical_cast<double>( (*itr).at(1) );
                        double bgcjacc = boost::lexical_cast<double>( (*itr).at(2) );
                        double bgcmacc = boost::lexical_cast<double>( (*itr).at(3) ); 
                        double agcprd = boost::lexical_cast<double>( (*itr).at(4) ); 
                        double bgcjprd = boost::lexical_cast<double>( (*itr).at(5) ); 
                        double bgcmprd = boost::lexical_cast<double>( (*itr).at(6) ); 
                        double cgrain = boost::lexical_cast<double>( (*itr).at(7) ); 
                        double hi = boost::lexical_cast<double>( (*itr).at(8) ); 
                        double ast1c2 = boost::lexical_cast<double>( (*itr).at(9) );
                        double ast2c2 = boost::lexical_cast<double>( (*itr).at(10) );
                        double amt1c2 = boost::lexical_cast<double>( (*itr).at(11) ); 
                        double amt2c2 = boost::lexical_cast<double>( (*itr).at(12) ); 
                        double as11c2 = boost::lexical_cast<double>( (*itr).at(13) ); 
                        double as12c2 = boost::lexical_cast<double>( (*itr).at(14) );
                        double as21c2 = boost::lexical_cast<double>( (*itr).at(15) ); 
                        double as22c2 = boost::lexical_cast<double>( (*itr).at(16) ); 
                        double as3c2 = boost::lexical_cast<double>( (*itr).at(17) );
                        double n2oacc = boost::lexical_cast<double>( (*itr).at(18) ); 
                        double n2oprd = boost::lexical_cast<double>( (*itr).at(19) ); 
                        double tcnpro = boost::lexical_cast<double>( (*itr).at(20) );
                        double stream2 = boost::lexical_cast<double>( (*itr).at(21) );
                        double strlig1 = boost::lexical_cast<double>( (*itr).at(22) ); 
                        double strlig2 = boost::lexical_cast<double>( (*itr).at(23) ); 
                        double accrst = boost::lexical_cast<double>( (*itr).at(24) ); 
                        double ermvst1 = boost::lexical_cast<double>( (*itr).at(25) ); 
                        double somsc = boost::lexical_cast<double>( (*itr).at(26) ); 
                        double somtc = boost::lexical_cast<double>( (*itr).at(27) );

                        Poco::Data::Statement insertstmt = ( *m_session 
                          << "INSERT INTO [" << dbTableName << "]\n"
                            //<< " abbr, mukey, cokey, tillreg, remreg, rotreg, yldreg, time, agcacc,\n"
                            //<< " bgcjacc, bgcmacc, agcprd, bgcjprd, bgcmprd, cgrain, hi,\n"
                            //<< " ast1c2, ast2c2, amt1c2, amt2c2, as11c2, as12c2, as21c2, as22c2, as3c2,\n"
                            //<< " n2oacc, tcnpro, stream(2), strlig(1), strlig(2), accrst, ermvst(1),\n"
                            //<< " somsc, somtc)\n"
                            << " VALUES(\n"
                            << " :abbr, :mukey, :cokey, :tillreg, :remreg, :rotreg, :yldreg, :time, :agcacc, \n"
                            << " :bgcjacc, :bgcmacc, :agcprd, :bgcjprd, :bgcmprd, :cgrain, :hi,\n"
                            << " :ast1c2, :ast2c2, :amt1c2, :amt2c2, :as11c2, :as12c2, :as21c2, :as22c2, :as3c2,\n"
                            << " :n2oacc, :n2oprd, :tcnpro, :stream2, :strlig1, :strlig2, :accrst, :ermvst1,\n"
                            << " :somsc, :somtc )",
                            useRef( abbr ),
                            useRef( mukey ),
                            useRef( cokey ),
                            useRef( tillreg ),
                            useRef( remreg ),
                            useRef( rotreg ),
                            useRef( yldreg ),
                            useRef( time ),
                            useRef( agcacc ),
                            useRef( bgcjacc ),
                            useRef( bgcmacc ),
                            useRef( agcprd ),
                            useRef( bgcjprd ),
                            useRef( bgcmprd ),
                            useRef( cgrain ),
                            useRef( hi ),
                            useRef( ast1c2 ),
                            useRef( ast2c2 ),
                            useRef( amt1c2 ),
                            useRef( amt2c2 ),
                            useRef( as11c2 ),
                            useRef( as12c2 ),
                            useRef( as21c2 ),
                            useRef( as22c2 ),
                            useRef( as3c2 ),
                            useRef( n2oacc ),
                            useRef( n2oprd ),
                            useRef( tcnpro ),
                            useRef( stream2 ),
                            useRef( strlig1 ),
                            useRef( strlig2 ),
                            useRef( accrst ),
                            useRef( ermvst1 ),
                            useRef( somsc ),
                            useRef( somtc ),
                            now );
                    }
                    catch( boost::bad_lexical_cast& ex )
                    {
                        ;
                    }
                    }*/
                }
            }
        }
    }

    fs::current_path( cp );
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::WriteToGHGDB( 
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg )
{
    fs::path cp( fs::current_path() );
    fs::path fp(cp/abbr/(abbr+"_"+cokey));
    fs::current_path( fp );

    fs::current_path( "X:\\PHD_data\\Paper1" );
    std::string root( "./" );
    std::string extension( ".lis" );
    //Iterate over all of the directories
    //Find if a file has a .lis extension

    boost::filesystem::directory_iterator end_iter;
    for( boost::filesystem::directory_iterator dir_itr( fp );
        dir_itr != end_iter; ++dir_itr )
    {

    }
    std::vector< std::string > lisFiles = GetFilesInDirectory( root, extension );
    //Open the file and parse it
    //Add the data from each .lis file to an .lis specific table in a db
    RegisterDB();
    //Iterate over the files
    for( size_t i = 0; i < lisFiles.size(); ++i )
    {
        /*std::string dbTableName = lisFiles.at( i );
        boost::algorithm::replace_first( dbTableName, root, "" );
        boost::algorithm::replace_last( dbTableName, extension, "" );
        boost::algorithm::replace_all( dbTableName, "/", "" );
        //boost::algorithm::replace_all( dbTableName, " ", "_" );
        std::cout << dbTableName << " " << lisFiles.at( i ) << std::endl;*/

        std::vector< std::string > test;
        boost::algorithm::split( test, lisFiles.at(i), boost::algorithm::is_any_of("/") );
        std::string abbr( test.back().substr(0, test.back().find(".") ) );
        std::string mukey( test.at(4).substr(6, 6) );
        std::string cokey( test.at(4).substr(6) );
        std::string tillreg( test.at(2) );
        std::string remreg( test.at(1) );
        std::string rotreg( test.at(3) );
        std::cout << abbr << " " << mukey << " " << cokey << " " << tillreg << " " << remreg << " " << rotreg << std::endl;

        std::vector< std::string > resultFiles;
        std::string baseline = "resultsBL";
        std::string sim = "results50";
        resultFiles.push_back( baseline );
        resultFiles.push_back( sim );
        std::string dbTableName = "ghg";
    
        std::ifstream infile( lisFiles.at( i ).c_str() );
        
        infile.seekg( 0, std::ios::end);
        std::streampos length = infile.tellg();
        infile.seekg (0, std::ios::beg);

        char* buffer = new char [ length ];
        infile.read( buffer, (length) );
        infile.close();

        std::string networkData( buffer );
        delete [] buffer;
        boost::algorithm::replace_all( networkData, "\r", "" );
        
        typedef boost::tokenizer< boost::escaped_list_separator<char> > Tok;
        boost::escaped_list_separator<char> sep( "", " \n", "");
        Tok tok( networkData, sep );
        std::string tempTok;
        double tempDouble = 0;
        size_t columnCount1 = 0;
        std::vector< std::string > columnNames;
        Tok::iterator firstDouble;
        for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
        {
            tempTok = *tok_iter;
            if( tempTok.empty() )
            {
                continue;
            }
            //std::cout << "<" << tempTok << "> ";
                
            try
            {
                tempDouble = boost::lexical_cast<double>( tempTok );
                firstDouble = tok_iter;
                break;
                //std::cout << tempDouble << " "; 
            }
            catch( boost::bad_lexical_cast& ex )
            {
                //std::cout << tempTok << " ";
                columnNames.push_back( tempTok );
                columnCount1 +=1;
            }
        }
        
        std::cout << "Column Count " << columnCount1 << std::endl;

        //////////////////////////////
        //////////////////////////////

        Poco::Data::Session session("SQLite", m_results );
        //manage open and closing the session our self so that the population of the
        //db is faster
        session.begin();
        // drop sample table, if it exists
        //std::ostringstream dropTable;
        //dropTable << "DROP TABLE IF EXISTS " << dbTableName;
        //session << dropTable.str().c_str(), now;

        m_session = new Poco::Data::Session( "SQLite", m_results );

        /*Poco::Data::Statement stmt = ( *m_session 
            << "CREATE TABLE IF NOT EXISTS [test](\n"
            << "[abbr] VARCHAR( 50 ) NOT NULL)", now );

        stmt.execute();*/
        for( size_t a=0; a<resultFiles.size(); ++a )
        {
        
            std::ostringstream createCommand;
            /*createCommand << "if object_id('results') is null \*/
            
            createCommand << "CREATE TABLE IF NOT EXISTS [" << dbTableName
                << "]( [abbr] VARCHAR( 50 ) NOT NULL,"
                << " [mukey]  VARCHAR( 50 ) NOT NULL,"
                << " [cokey]  VARCHAR( 50 ) NOT NULL,"
                << " [tillreg]  VARCHAR( 50 ) NOT NULL,"
                << " [remreg]  VARCHAR( 50 ) NOT NULL,"
                << " [rotreg]  VARCHAR( 50 ) NOT NULL,"
                << " [yldreg]  VARCHAR( 50 ) NOT NULL,";
            for( size_t i = 0; i < columnNames.size(); ++i )
            {
                createCommand << "[" << columnNames.at( i ) << "] DOUBLE";
                
                if( i < columnCount1 - 1 )
                {
                    createCommand << ",";
                }
            }
            createCommand << ",";
            createCommand << " UNIQUE( [abbr], [mukey], [cokey],\n"
                          << " [tillreg], [remreg], [rotreg], [yldreg], [time] )\n"
                          << "  ON CONFLICT IGNORE )";

            //std::cout << createCommand.str() << std::endl;

            Poco::Data::Statement stmt = ( *m_session
                << createCommand.str(), now );

            stmt.execute();

        }
        //////////////////////////////
        //////////////////////////////

        std::ifstream lisFile;
        lisFile.open( lisFiles.at( i ).c_str() , std::ios::out );
        std::string line;
        std::vector< std::vector< std::string > > data;
        while( getline( lisFile, line ) )
        {
            std::vector< std::string > linedata;
            std::istringstream iss( line );
            std::copy( std::istream_iterator< std::string >( iss ),
                std::istream_iterator< std::string >(),
                std::back_inserter< std::vector< std::string > >(linedata) );
            //boost::algorithm::split( linedata, line, boost::algorithm::is_any_of("\t") );
            data.push_back( linedata );
        }

        std::vector< std::vector< std::string > >::iterator itr = data.begin();
        for( ; itr != data.end(); ++itr )
        {
            if( !(*itr).empty() )
            {
                //std::string dbTableName;
                if( (*itr).at(0) > "2010.00" )
                /*if( (*itr).at(0) == "2011.00" )
                {
                    //dbTableName = resultFiles.at(0);
                }
                else if( (*itr).at(0) == "2061.00" )
                {
                    //dbTableName = resultFiles.at(1);
                }*/

                //if( (*itr).at(0) == "2011.00" || (*itr).at(0) == "2061.00" )
                {
                    try
                    {
                        //tempDouble = boost::lexical_cast<double>( tempTok );
                        double time = boost::lexical_cast<double>( (*itr).at(0) ); 
                        double agcacc = boost::lexical_cast<double>( (*itr).at(1) );
                        double bgcjacc = boost::lexical_cast<double>( (*itr).at(2) );
                        double bgcmacc = boost::lexical_cast<double>( (*itr).at(3) ); 
                        double agcprd = boost::lexical_cast<double>( (*itr).at(4) ); 
                        double bgcjprd = boost::lexical_cast<double>( (*itr).at(5) ); 
                        double bgcmprd = boost::lexical_cast<double>( (*itr).at(6) ); 
                        double cgrain = boost::lexical_cast<double>( (*itr).at(7) ); 
                        double hi = boost::lexical_cast<double>( (*itr).at(8) ); 
                        double ast1c2 = boost::lexical_cast<double>( (*itr).at(9) );
                        double ast2c2 = boost::lexical_cast<double>( (*itr).at(10) );
                        double amt1c2 = boost::lexical_cast<double>( (*itr).at(11) ); 
                        double amt2c2 = boost::lexical_cast<double>( (*itr).at(12) ); 
                        double as11c2 = boost::lexical_cast<double>( (*itr).at(13) ); 
                        double as12c2 = boost::lexical_cast<double>( (*itr).at(14) );
                        double as21c2 = boost::lexical_cast<double>( (*itr).at(15) ); 
                        double as22c2 = boost::lexical_cast<double>( (*itr).at(16) ); 
                        double as3c2 = boost::lexical_cast<double>( (*itr).at(17) );
                        double n2oacc = boost::lexical_cast<double>( (*itr).at(18) ); 
                        double tcnpro = boost::lexical_cast<double>( (*itr).at(19) );
                        double stream2 = boost::lexical_cast<double>( (*itr).at(20) );
                        double strlig1 = boost::lexical_cast<double>( (*itr).at(21) ); 
                        double strlig2 = boost::lexical_cast<double>( (*itr).at(22) ); 
                        double accrst = boost::lexical_cast<double>( (*itr).at(23) ); 
                        double ermvst1 = boost::lexical_cast<double>( (*itr).at(24) ); 
                        double somsc = boost::lexical_cast<double>( (*itr).at(25) ); 
                        double somtc = boost::lexical_cast<double>( (*itr).at(26) );

                        Poco::Data::Statement insertstmt = ( *m_session 
                          << "INSERT INTO [" << dbTableName << "]\n"
                            //<< " abbr, mukey, cokey, tillreg, remreg, rotreg, yldreg, time, agcacc,\n"
                            //<< " bgcjacc, bgcmacc, agcprd, bgcjprd, bgcmprd, cgrain, hi,\n"
                            //<< " ast1c2, ast2c2, amt1c2, amt2c2, as11c2, as12c2, as21c2, as22c2, as3c2,\n"
                            //<< " n2oacc, tcnpro, stream(2), strlig(1), strlig(2), accrst, ermvst(1),\n"
                            //<< " somsc, somtc)\n"
                            << " VALUES(\n"
                            << " :abbr, :mukey, :cokey, :tillreg, :remreg, :rotreg, :yldreg, :time, :agcacc, \n"
                            << " :bgcjacc, :bgcmacc, :agcprd, :bgcjprd, :bgcmprd, :cgrain, :hi,\n"
                            << " :ast1c2, :ast2c2, :amt1c2, :amt2c2, :as11c2, :as12c2, :as21c2, :as22c2, :as3c2,\n"
                            << " :n2oacc, :tcnpro, :stream2, :strlig1, :strlig2, :accrst, :ermvst1,\n"
                            << " :somsc, :somtc )",
                            useRef( abbr ),
                            useRef( mukey ),
                            useRef( cokey ),
                            useRef( tillreg ),
                            useRef( remreg ),
                            useRef( rotreg ),
                            useRef( yldreg ),
                            useRef( time ),
                            useRef( agcacc ),
                            useRef( bgcjacc ),
                            useRef( bgcmacc ),
                            useRef( agcprd ),
                            useRef( bgcjprd ),
                            useRef( bgcmprd ),
                            useRef( cgrain ),
                            useRef( hi ),
                            useRef( ast1c2 ),
                            useRef( ast2c2 ),
                            useRef( amt1c2 ),
                            useRef( amt2c2 ),
                            useRef( as11c2 ),
                            useRef( as12c2 ),
                            useRef( as21c2 ),
                            useRef( as22c2 ),
                            useRef( as3c2 ),
                            useRef( n2oacc ),
                            useRef( tcnpro ),
                            useRef( stream2 ),
                            useRef( strlig1 ),
                            useRef( strlig2 ),
                            useRef( accrst ),
                            useRef( ermvst1 ),
                            useRef( somsc ),
                            useRef( somtc ),
                            now );
                    }
                    catch( boost::bad_lexical_cast& ex )
                    {
                        ;
                    }
                }
            }
        }
    }

    fs::current_path( cp );
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::CreateOutDB( std::string const& file )
{
    RegisterDB();

    m_session = new Poco::Data::Session( "SQLite", m_results );

    std::ifstream infile( file.c_str() );
        
    infile.seekg( 0, std::ios::end);
    std::streampos length = infile.tellg();
    infile.seekg (0, std::ios::beg);

    char* buffer = new char [ length ];
    infile.read( buffer, (length) );
    infile.close();

    std::string networkData( buffer );
    delete [] buffer;
    boost::algorithm::replace_all( networkData, "\r", "" );
    
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tok;
    boost::escaped_list_separator<char> sep( "", " \n", "");
    Tok tok( networkData, sep );
    std::string tempTok;

    double tempDouble = 0;
    size_t columnCount1 = 0;
    std::vector< std::string > columnNames;
    Tok::iterator firstDouble;
    for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
    {
        tempTok = *tok_iter;
        if( tempTok.empty() )
        {
            continue;
        }
        //std::cout << "<" << tempTok << "> ";
            
        try
        {
            tempDouble = boost::lexical_cast<double>( tempTok );
            firstDouble = tok_iter;
            break;
            //std::cout << tempDouble << " "; 
        }
        catch( boost::bad_lexical_cast& ex )
        {
            //std::cout << tempTok << " ";
            columnNames.push_back( tempTok );
            columnCount1 +=1;
        }
    }
    
    std::cout << "Column Count " << columnCount1 << std::endl;

    std::ostringstream createCommand;

    std::string tablename = GetTableName( file );
    
    createCommand << "CREATE TABLE IF NOT EXISTS [" << tablename
        << "]( [abbr] VARCHAR( 50 ) NOT NULL,"
        << " [mukey]  VARCHAR( 50 ) NOT NULL,"
        << " [cokey]  VARCHAR( 50 ) NOT NULL,"
        << " [tillreg]  VARCHAR( 50 ) NOT NULL,"
        << " [remreg]  VARCHAR( 50 ) NOT NULL,"
        << " [rotreg]  VARCHAR( 50 ) NOT NULL,"
        << " [yldreg]  VARCHAR( 50 ) NOT NULL,"
        << " [name]  VARCHAR( 50 ) NOT NULL,";
    for( size_t i = 0; i < columnNames.size(); ++i )
    {
        createCommand << "[" << columnNames.at( i ) << "] DOUBLE";
        
        if( i < columnCount1 - 1 )
        {
            createCommand << ",";
        }
    }
    createCommand << ",";
    createCommand << " UNIQUE( [abbr], [mukey], [cokey],\n"
                  << " [tillreg], [remreg], [rotreg], [yldreg], [name], [time] )\n"
                  << "  ON CONFLICT IGNORE )";

    //std::cout << createCommand.str() << std::endl;

    Poco::Data::Statement stmt = ( *m_session
        << createCommand.str(), now );

    stmt.execute();
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::CreateVSWCDB( std::string const& file )
{
    RegisterDB();

    m_session = new Poco::Data::Session( "SQLite", m_results );

    std::ifstream outFile;
    outFile.open( file.c_str() , std::ios::out );
    std::string line;
    getline( outFile, line ); 
    std::vector< std::string > linedata;
    std::istringstream iss( line );
    std::copy( std::istream_iterator< std::string >( iss ),
        std::istream_iterator< std::string >(),
        std::back_inserter< std::vector< std::string > >(linedata) );

    std::cout << "Column Count " << linedata.size() << std::endl;

    std::ostringstream createCommand;
    
    createCommand << "CREATE TABLE IF NOT EXISTS [vswc]"
        << "( [abbr] VARCHAR( 50 ) NOT NULL,"
        << " [mukey]  VARCHAR( 50 ) NOT NULL,"
        << " [cokey]  VARCHAR( 50 ) NOT NULL,"
        << " [tillreg]  VARCHAR( 50 ) NOT NULL,"
        << " [remreg]  VARCHAR( 50 ) NOT NULL,"
        << " [rotreg]  VARCHAR( 50 ) NOT NULL,"
        << " [yldreg]  VARCHAR( 50 ) NOT NULL,"
        << " [time]  DOUBLE,"
        << " [day] DOUBLE,"
        << " [layer1]  DOUBLE,"
        << " [layer2]  DOUBLE,"
        << " [layer3]  DOUBLE,"
        << " [layer4]  DOUBLE,"
        << " [layer5]  DOUBLE,"
        << " [layer6]  DOUBLE,"
        << " [layer7]  DOUBLE,"
        << " [layer8]  DOUBLE,"
        << " [layer9]  DOUBLE,"
        << " [layer10]  DOUBLE,"
        << " [layer11]  DOUBLE,"
        << " [layer12]  DOUBLE,"
        << " [layer13]  DOUBLE,"
        << " [layer14]  DOUBLE,";

    /*for( size_t i = 2; i < linedata.size(); ++i )
    {
        createCommand << "[layer" << i-1 << "] DOUBLE";
        
        if( i < linedata.size() - 1 )
        {
            createCommand << ",";
        }
    }*/
    //createCommand << ",";
    createCommand << " UNIQUE( [abbr], [mukey], [cokey],\n"
                  << " [tillreg], [remreg], [rotreg], [yldreg], [time] )\n"
                  << "  ON CONFLICT IGNORE )";

    std::cout << createCommand.str() << std::endl;

    Poco::Data::Statement stmt = ( *m_session
        << createCommand.str(), now );

    stmt.execute();
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::WriteNDB(
    std::string const& file,
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg )
{
    std::ifstream outFile;
    outFile.open( file.c_str() , std::ios::out );
    std::string line;
    std::vector< std::vector< std::string > > data;
    while( getline( outFile, line ) )
    {
        std::vector< std::string > linedata;
        std::istringstream iss( line );
        std::copy( std::istream_iterator< std::string >( iss ),
            std::istream_iterator< std::string >(),
            std::back_inserter< std::vector< std::string > >(linedata) );
        //boost::algorithm::split( linedata, line, boost::algorithm::is_any_of("\t") );
        data.push_back( linedata );
    }

    RegisterDB();

    m_time.clear();
    std::vector< std::vector< std::string > >::iterator itr = data.begin()+1;
    for( ; itr != data.end(); ++itr )
    {
        if( !(*itr).empty() )
        {
            double time = boost::lexical_cast<double>( (*itr).at(0) ); 
            double n2oflux = boost::lexical_cast<double>( (*itr).at(1) );
            double noflux = boost::lexical_cast<double>( (*itr).at(2) );
            double n2flux = boost::lexical_cast<double>( (*itr).at(3) ); 
            double ch4 = boost::lexical_cast<double>( (*itr).at(4) ); 
            double nit = boost::lexical_cast<double>( (*itr).at(5) ); 
            double annppt = boost::lexical_cast<double>( (*itr).at(6) ); 
            Poco::Data::Statement insertstmt = ( *m_session 
                << "INSERT INTO [nitrogen]\n"
                << " VALUES(\n"
                << " :abbr, :mukey, :cokey, :tillreg, :remreg, :rotreg, :yldreg, :name, \n"
                << " :time, :n2oflux, :noflux, :n2flux, :ch4, :nit, :annppt )",
                useRef( abbr ),
                useRef( mukey ),
                useRef( cokey ),
                useRef( tillreg ),
                useRef( remreg ),
                useRef( rotreg ),
                useRef( yldreg ),
                useRef( m_field ),
                useRef( time ),
                useRef( n2oflux ),
                useRef( noflux ),
                useRef( n2flux ),
                useRef( ch4 ),
                useRef( nit ),
                useRef( annppt ),
                now );

            m_time.push_back( time );
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::WriteOutFile(
    std::string const& file,
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg )
{
    std::ifstream outFile;
    outFile.open( file.c_str() , std::ios::out );
    std::string line;
    std::vector< std::vector< std::string > > data;
    while( getline( outFile, line ) )
    {
        std::vector< std::string > linedata;
        std::istringstream iss( line );
        std::copy( std::istream_iterator< std::string >( iss ),
            std::istream_iterator< std::string >(),
            std::back_inserter< std::vector< std::string > >(linedata) );
        //boost::algorithm::split( linedata, line, boost::algorithm::is_any_of("\t") );
        data.push_back( linedata );
    }

    RegisterDB();

    std::string tablename = GetTableName( file );

    m_time.clear();
    std::vector< std::vector< std::string > >::iterator itr = data.begin()+1;
    for( ; itr != data.end(); ++itr )
    {
        if( !(*itr).empty() )
        {
            unsigned int columnCounter(0);
            unsigned int columnCount1( (*itr).size() );
            std::ostringstream insertCommand;

            try
            {
                insertCommand << "INSERT INTO " << tablename << " VALUES('";
                insertCommand << abbr << "','" << mukey << "','" << cokey << "','" << tillreg << "','";
                insertCommand << remreg << "','" << rotreg << "','" << yldreg << "','" << m_field << "',";
            }
            catch( boost::bad_lexical_cast& ex )
            {
                ;
            }
            
            for( size_t i=0; i<(*itr).size(); ++i )
            {
                insertCommand << (*itr).at(i);
                  
                if( columnCounter < columnCount1 - 1 )
                {
                    insertCommand << ",";
                    ++columnCounter;                    
                }
                else
                {
                    insertCommand << ")";

                    std::cout << insertCommand.str() << std::endl;

                    Poco::Data::Statement stmt = ( *m_session
                        << insertCommand.str(), now );
                }
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::WriteVSWCDB(
    std::string const& file,
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg )
{
    std::ifstream outFile;
    outFile.open( file.c_str() , std::ios::out );
    std::string line;
    std::vector< std::vector< std::string > > data;
    std::vector< std::string > tmp;
    while( getline( outFile, line ) )
    {
        std::vector< std::string > linedata;
        std::istringstream iss( line );
        std::copy( std::istream_iterator< std::string >( iss ),
            std::istream_iterator< std::string >(),
            std::back_inserter< std::vector< std::string > >(linedata) );
        data.push_back( linedata );
        tmp = linedata;
    }

    RegisterDB();
    std::cout << data.size() << std::endl;

    fs::path cp( fs::current_path() ); 
    fs::path sitepath( cp );//abbr/tillreg/rotreg/remreg/(abbr + "_" + cokey) );
    fs::current_path( sitepath );
    std::cout << fs::current_path().string() << std::endl;
    std::cout << cp.string() << std::endl;

    Poco::Dynamic::Var var;
    file::F_100&  f100 = *file::F_100::instance();
    assert( f100.OpenFile( SITE, "", abbr ) );
    file::Option const& opt = f100.GetOption();
    opt.GetValue( "NLAYER", var );
    std::cout << var.convert< int >() << std::endl;
    int nlayer = var.convert< int >();
    f100.WriteOptions();
    f100.CloseFile();

    fs::current_path( cp );

    double layer14;
    std::vector< std::vector< std::string > >::iterator itr = data.begin();
    for( ; itr != data.end(); ++itr )
    {
        if( !(*itr).empty() )
        {
            double time = boost::lexical_cast<double>( (*itr).at(0) ); 
            double day = boost::lexical_cast<double>( (*itr).at(1) );
            double layer1 = boost::lexical_cast<double>( (*itr).at(2) );
            double layer2 = boost::lexical_cast<double>( (*itr).at(3) ); 
            double layer3 = boost::lexical_cast<double>( (*itr).at(4) ); 
            double layer4 = boost::lexical_cast<double>( (*itr).at(5) ); 
            double layer5 = boost::lexical_cast<double>( (*itr).at(6) ); 
            double layer6 = boost::lexical_cast<double>( (*itr).at(7) );
            double layer7 = boost::lexical_cast<double>( (*itr).at(8) ); 
            double layer8 = boost::lexical_cast<double>( (*itr).at(9) ); 
            double layer9 = boost::lexical_cast<double>( (*itr).at(10) ); 
            double layer10 = boost::lexical_cast<double>( (*itr).at(11) ); 
            double layer11 = boost::lexical_cast<double>( (*itr).at(12) ); 
            double layer12 = boost::lexical_cast<double>( (*itr).at(13) ); 
            double layer13 = boost::lexical_cast<double>( (*itr).at(14) ); 
            if( nlayer = 8 )
            {
                layer14 = 0;
            }
            else if( nlayer = 9 )
            {
                layer14 = boost::lexical_cast<double>( (*itr).at(15) );//
            }
            //boost::lexical_cast<double>( (*itr).at(15) );

            //Poco::Data::Statement insertstmt = ( *m_session 
            std::ostringstream createCommand;
            createCommand << "INSERT INTO [vswc]\n"
                << " VALUES(\n"
                << " :abbr, :mukey, :cokey, :tillreg, :remreg, :rotreg, :yldreg,\n"
                << " :time, :day, :layer1, :layer2, :layer3, :layer4, :layer5,\n"
                << " :layer6, :layer7, :layer8, :layer9, :layer10, :layer11, :layer12, "
                << " :layer13, :layer14)";
                /*for( size_t i=2; i<(*itr).size(); ++i )
                {
                    if( i != (*itr).size() )
                    {
                        createCommand << ":layer" << i-1 <<",";
                    }
                    else
                    {


                        createCommand << ":layer" << i-1 <<" )";
                    }
                }*/

               // std::cout << createCommand.str() << std::endl;

            Poco::Data::Statement stmt = ( *m_session
                << createCommand.str(),
                useRef( abbr ),
                useRef( mukey ),
                useRef( cokey ),
                useRef( tillreg ),
                useRef( remreg ),
                useRef( rotreg ),
                useRef( yldreg ),
                useRef( time ),
                useRef( day ),
                useRef( layer1 ),
                useRef( layer2 ),
                useRef( layer3 ),
                useRef( layer4 ),
                useRef( layer5 ),
                useRef( layer6 ),
                useRef( layer7 ),
                useRef( layer8 ),
                useRef( layer9 ),
                useRef( layer10 ),
                useRef( layer11 ),
                useRef( layer12 ),
                useRef( layer13 ),
                useRef( layer14 ),
                now );
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::CreateSustDB( )
    /*std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg )*/
{
    RegisterDB();

    m_session = new Poco::Data::Session( "SQLite", m_results );
    
    std::ostringstream createCommand;
    
    createCommand << "CREATE TABLE IF NOT EXISTS [sust]"
        << "( [abbr] VARCHAR( 50 ) NOT NULL,"
        << " [mukey]  VARCHAR( 50 ) NOT NULL,"
        << " [cokey]  VARCHAR( 50 ) NOT NULL,"
        << " [tillreg]  VARCHAR( 50 ) NOT NULL,"
        << " [remreg]  VARCHAR( 50 ) NOT NULL,"
        << " [rotreg]  VARCHAR( 50 ) NOT NULL,"
        << " [yldreg]  VARCHAR( 50 ) NOT NULL,"
        << " [sci] DOUBLE,"
        << " [sciER] DOUBLE,"
        << " [sciOM] DOUBLE,"
        << " [sciFO] DOUBLE,"
        << " [windero] DOUBLE,"
        << " [waterero] DOUBLE,"
        << " [totbiomass] DOUBLE,"
        << " UNIQUE( [abbr], [mukey], [cokey],\n"
        << " [tillreg], [remreg], [rotreg], [yldreg] )\n"
        << "  ON CONFLICT IGNORE )";

    //std::cout << createCommand.str() << std::endl;

    Poco::Data::Statement stmt = ( *m_session
        << createCommand.str(), now );

    stmt.execute();
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::WriteSustDB( 
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg,
    leaf::open::ero::ErosionPtr data )
    /*std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& remreg,
    std::string const& rotreg,
    std::string const& yldreg)*/
{
    RegisterDB();

    //ero::ErosionPtr dataR2 = rusle2::CallRusle2();

    //double time;// = dataR2->GetWindEros().convert< double >();
    double windero = data->GetWindEros().convert< double >();
    double waterero = data->GetWaterEros().convert< double >(); 
    double sci = data->GetSci(); 
    double sciER = data->GetSciER(); 
    double sciOM = data->GetSciOM(); 
    double sciFO = data->GetSciFO();
    double totbiomass = data->GetTotBioRem();

    //Poco::Data::Statement insertstmt = ( *m_session 
    std::ostringstream createCommand;
    createCommand << "INSERT INTO [sust]\n"
        << " VALUES(\n"
        << " :abbr, :mukey, :cokey, :tillreg, :remreg, :rotreg, :yldreg,\n"
        << " :sci, :sciER, :sciOM, :sciFO, :windero, :waterero, :totbiomass)";
       // std::cout << createCommand.str() << std::endl;

    Poco::Data::Statement stmt = ( *m_session
        << createCommand.str(),
        useRef( abbr ),
        useRef( mukey ),
        useRef( cokey ),
        useRef( tillreg ),
        useRef( remreg ),
        useRef( rotreg ),
        useRef( yldreg ),
        useRef( sci ),
        useRef( sciER ),
        useRef( sciOM ),
        useRef( sciFO ),
        useRef( windero ),
        useRef( waterero ),
        useRef( totbiomass),
        now );
}
////////////////////////////////////////////////////////////////////////////////
std::vector< double > PostProcessor::GetTime()
{
    return m_time;
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::SetField( std::string const& field )
{
    m_field = field;
}
////////////////////////////////////////////////////////////////////////////////
void PostProcessor::SetYields( std::pair< std::string, std::string > yields )
{
    m_yields = yields;
}
////////////////////////////////////////////////////////////////////////////////
std::pair< std::string, std::string> PostProcessor::GetYields()
{
    return m_yields;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& PostProcessor::GetTableName( std::string const& file )
{
    std::string temp = file.substr( file.rfind( "/" )+1, file.length() );
    static std::map< std::string, std::string > const fileMap =
        boost::assign::map_list_of
            ( "year_summary.out", "nitrogen" )
            ( "summary", "daily_gas" );

    std::map< std::string, std::string >::const_iterator itr =
        fileMap.find( temp );

    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
}//daycent
}//wrapper
}//leaf