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
#include <leaf/open/man/Management.h>
#include <leaf/open/scn/Scenario.h>
#include <leaf/open/soi/Soils.h>

#include <leaf/wrapper/rusle2/api-rusle2.h>

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

////////////////////////////////////////////////////////////////////////////////
int main(
    int argc,
    char** argv )
{
    //For debugging purposes
    std::string junk; std::cin >> junk;

    Init( argc, argv );

    try
    {
        Main();
    }
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
        ( "help,h", "produce help message" );

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    if( vm.count( "help" ) )
    {
        std::cout << std::endl << desc << std::endl;

        exit( 0 );
    }

    RegisterConnectors();

    rusle2::Init( "nelson_2013.gdb" );
}
////////////////////////////////////////////////////////////////////////////////
void Main()
{
    man::ManagementPtr man = man::Management::Create();
    man->SetName( "THE_MAN" );
    man::Operation op;
    man::Crop crop;

    op.SetDate( "5/8/1" );
    op.SetName( "Cultivator, field 6-12 in sweeps" );
    man->AddOperation( op );

    op.SetDate( "5/11/1" );
    op.SetName( "Planter, double disk opnr w/fluted coulter " );
    crop.SetName( "Corn, grain" );
    op.SetCrop( crop );
    man->AddOperation( op );

    op.SetDate( "5/14/1" );
    op.SetName( "Fert applic. surface broadcast" );
    man->AddOperation( op );

    op.SetDate( "10/9/1" );
    op.SetName( "Harvest, killing crop 50pct standing stubble" );
    man->AddOperation( op );

    //op.SetDate( "10/30/1" );
    //op.SetName( "Bale, Corn Stover Direct 50% Flat Res Removed" );
    //man->AddOperation( op );

    op.SetDate( "11/2/1" );
    op.SetName( "Fert applic. surface broadcast" );
    man->AddOperation( op );

    op.SetDate( "11/4/1" );
    op.SetName( "Chisel plow, disk, st. pts." );
    man->AddOperation( op );

    op.SetDate( "11/15/1" );
    op.SetName( "Fert. applic. anhyd knife 30 in" );
    man->AddOperation( op );

    op.SetDate( "5/8/2" );
    op.SetName( "Cultivator, field 6-12 in sweeps" );
    man->AddOperation( op );

    op.SetDate( "5/11/2" );
    op.SetName( "Planter, double disk opnr w/fluted coulter " );
    crop.SetName( "Corn, grain" );
    op.SetCrop( crop );
    man->AddOperation( op );

    op.SetDate( "5/14/2" );
    op.SetName( "Fert applic. surface broadcast" );
    man->AddOperation( op );

    op.SetDate( "10/9/2" );
    op.SetName( "Harvest, killing crop 50pct standing stubble" );
    man->AddOperation( op );

    //op.SetDate( "10/30/2" );
    //op.SetName( "Bale, Corn Stover Direct 50% Flat Res Removed" );
    //man->AddOperation( op );

    op.SetDate( "11/2/2" );
    op.SetName( "Fert applic. surface broadcast" );
    man->AddOperation( op );

    op.SetDate( "11/4/2" );
    op.SetName( "Chisel plow, disk, st. pts." );
    man->AddOperation( op );

    op.SetDate( "11/15/2" );
    op.SetName( "Fert. applic. anhyd knife 30 in" );
    man->AddOperation( op );

    rusle2::CreateManFile( man, "test.skel", true );
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    rusle2::Exit();

    UnregisterConnectors();
}
////////////////////////////////////////////////////////////////////////////////
