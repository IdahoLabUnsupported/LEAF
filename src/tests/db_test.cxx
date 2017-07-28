// Copyright 2017 Battelle Energy Alliance, LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



// --- App Includes --- //
#include "db_test.h"

// --- LEAF Includes --- //
#include <leaf/open/ero/Erosion.h>
#include <leaf/open/scn/Scenario.h>

#include <leaf/open/soi/Soils.h>

#include <leaf/wrapper/weru/Calibration.h>
#include <leaf/wrapper/weru/Harvest.h>

using namespace leaf::util;
using namespace leaf::open;
using namespace leaf::wrapper;

// --- Boost Includes --- //
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

// --- POCO Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

// --- STL Includes --- //
#include <iostream>

//http://psqlodbc.projects.pgfoundry.org/faq.html
//"Driver={PostgreSQL Unicode(x64)};Server=localhost;Port=5432;Uid=postgres;Pwd=postgres;Database=ssurgo;"
//"Driver={PostgreSQL Unicode(x64)};Server=localhost;Port=5432;Uid=postgres;Pwd=postgres;Database=leaf;"

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
    //For debugging purposes
    std::string junk; std::cin >> junk;

    Init( argc, argv );

    try
    {
        Main();
    }
#ifdef POCO_MYSQL_API
    catch( Poco::Data::MySQL::MySQLException const& e )
    {
        std::cerr << "*** Poco::Data::MySQL::MySQLException caught: "
                  << e.what() << std::endl;
    }
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
    catch( Poco::Data::ODBC::ConnectionException const& e )
    {
        std::cerr << "*** Poco::Data::ODBC::ConnectionException caught: "
                  << e.toString() << std::endl;
    }
    catch( Poco::Data::ODBC::DescriptorException const& e )
    {
        std::cerr << "*** Poco::Data::ODBC::DescriptorException caught: "
                  << e.toString() << std::endl;
    }
    catch( Poco::Data::ODBC::EnvironmentException const& e )
    {
        std::cerr << "*** Poco::Data::ODBC::EnvironmentException caught: "
                  << e.toString() << std::endl;
    }
    catch( Poco::Data::ODBC::StatementException const& e )
    {
        std::cerr << "*** Poco::Data::ODBC::StatementException caught: "
                  << e.toString() << std::endl;
    }
#endif //POCO_ODBC_API
    catch( Poco::Data::SQLite::SQLiteException const& e )
    {
        std::cerr << "*** Poco::Data::SQLite::SQLiteException caught: "
                  << e.what() << std::endl;
    }
    catch( Poco::Data::DataException const& e )
    {
        std::cerr << "*** Poco::Data::DataException caught: "
                  << e.what() << std::endl;
    }
    catch( Poco::Exception const& e )
    {
        std::cerr << "*** Poco::Exception caught: "
                  << e.what() << std::endl;
    }
    catch( std::exception const& e )
    {
        std::cerr << "*** std::exception caught: "
                  << e.what() << std::endl;
    }

    Exit();

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
void Init(
    int argc,
    char* argv[] )
{
    fs::path ldd( getenv( "LEAF_DATA_DIR" ) );

    //Declare the supported options
    po::options_description desc( "Allowed options" );
    desc.add_options()
        ( "help,h", "produce help message" )
        ( "connector,c", po::value< std::string >(),
          "set the database connector type:\n"
#ifdef POCO_MYSQL_API
          "    MYSQL\n"
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
          "    ODBC\n"
#endif //POCO_ODBC_API
          "    SQLITE" )
        ( "ssurgoconnstr,s", po::value< std::string >(),
          "set the ssurgo connection string" )
        ( "leafconnstr,l", po::value< std::string >(),
          "set the leaf connection string" );

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    if( vm.count( "help" ) )
    {
        std::cout << std::endl << desc << std::endl << std::endl;
        Drivers();
        std::cout << std::endl;
        DataSources();

        exit( 0 );
    }

    //Set the connector type
    ConnectorEnum connEnum;
    if( vm.count( "connector" ) )
    {
        std::string connector = vm[ "connector" ].as< std::string >();
        boost::algorithm::to_upper( connector );

        if( connector == "SQLITE" ) connEnum = CONN_SQLITE;
#ifdef POCO_MYSQL_API
        else if( connector == "MYSQL" ) connEnum = CONN_MYSQL;
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
        else if( connector == "ODBC" ) connEnum = CONN_ODBC;
#endif //POCO_ODBC_API
        else
        {
            std::cout << "Please enter a valid connector type" << std::endl;

            exit( 0 );
        }
    }
    else
    {
        connEnum = CONN_SQLITE;
    }

    //Set the ssurgo connection string
    std::string ssurgoConnStr;
    if( vm.count( "ssurgoconnstr" ) )
    {
        ssurgoConnStr = vm[ "ssurgoconnstr" ].as< std::string >();
    }
    else if( connEnum == CONN_SQLITE )
    {
        ssurgoConnStr = fs::path( ldd/"db"/"ssurgo.db" ).string();
    }
    else
    {
        std::cout << "Please enter a valid connection string for ssurgo" << std::endl;

        exit( 0 );
    }

    //Set the connection string
    std::string leafConnStr;
    if( vm.count( "leafconnstr" ) )
    {
        leafConnStr = vm[ "leafconnstr" ].as< std::string >();
    }
    else if( connEnum == CONN_SQLITE )
    {
        leafConnStr = fs::path( ldd/"db"/"leaf.db" ).string();
    }
    else
    {
        std::cout << "Please enter a valid connection string for leaf" << std::endl;

        exit( 0 );
    }

    ConnectionVector connections = boost::assign::list_of
        ( ConnectionTuple( DB_SSURGO, connEnum, ssurgoConnStr ) )
        ( ConnectionTuple( DB_LEAF, connEnum, leafConnStr ) );

    RegisterConnectors( connections );
}
////////////////////////////////////////////////////////////////////////////////
void Main()
{
    std::string mukey = "403893";
    soi::MapUnitPtr mapunit = soi::MapUnit::Create( mukey );
    std::cout << mapunit << std::endl;

    std::string cokey = "403893:544518";
    soi::ComponentPtr component = mapunit->GetComponent( cokey );
    std::cout << component << std::endl;

    std::string corestrictkey = "";
    //soi::CorestrictionPtr corestriction =
    //    component->GetCorestriction( corestrictkey );
    //std::cout << corestriction << std::endl;

    std::string chkey = "403893:8130947";
    soi::CHorizonPtr horizon = component->GetHorizon( chkey );
    std::cout << horizon << std::endl;

    std::string chfragskey = "403893:14612309";
    soi::ChFragPtr chfrag = horizon->GetFrag( chfragskey );
    std::cout << chfrag << std::endl;

    std::string chtgkey = "403893:16507538";
    soi::ChTextureGrpPtr chtexturegrp = horizon->GetTextureGrp( chtgkey );
    std::cout << chtexturegrp << std::endl;

    std::string chtkey = "403893:16865067";
    soi::ChTexturePtr chtexture = chtexturegrp->GetTexture( chtkey );
    std::cout << chtexture << std::endl;

    Var spatial = 2;
    Var id = "cc835166-8466-486f-b2ef-86626454cc32";
    scn::ScenarioPtr scenario = scn::Scenario::Extract( id );
    if( scenario == scn::ScenarioPtr() )
    {
        scenario = scn::Scenario::Create(
            "TEST3", cokey, "CG", "RT", "HRH", "2010TEST", spatial,
            Keywords::null, Keywords::null, Keywords::null, id );
    }
    std::cout << scenario << std::endl;

    ero::ErosionPtr erosion = ero::Erosion::Extract( ero::WATER, scenario );
    ero::ErosionByCropPtr eroByCrop1, eroByCrop2;
    if( erosion == ero::ErosionPtr() )
    {
        erosion = ero::Erosion::Create( ero::WATER, scenario );
        erosion->SetSci( 0.1 );
        erosion->SetSciOM( 0.2 );
        erosion->SetSciER( 0.3 );
        erosion->SetSciFO( 0.4 );
        erosion->SetWindEros( 0.1 );
        erosion->SetWaterEros( 1.0 );
        erosion->SetTotBioRem( 5000 );

        eroByCrop1 = ero::ErosionByCrop::Create( erosion, "CG", 7500, 1 );
        eroByCrop2 = ero::ErosionByCrop::Create( erosion, "CG", 2500, 2 );

        erosion->WriteToDB();
    }
    else
    {
        eroByCrop1 = (*erosion)[ 0 ];
        eroByCrop2 = (*erosion)[ 1 ];
    }
    std::cout << erosion << std::endl;
    std::cout << eroByCrop1 << std::endl;
    std::cout << eroByCrop2 << std::endl;

    weru::CalibrationPtr calibration =
        weru::Calibration::Create( scenario, "01/01/01", "CG", 0.7676 );
    calibration->SetTgtYld( 177 );
    std::cout << calibration << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    UnregisterConnectors();
}
////////////////////////////////////////////////////////////////////////////////
