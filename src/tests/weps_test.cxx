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
#include "weps_test.h"

// --- LEAF Includes --- //
#include <leaf/util/SoilHelper.h>

#include <leaf/wrapper/weru/api-weps.h>

using namespace leaf::util;
using namespace leaf::open;
using namespace leaf::wrapper;

// --- Boost Includes --- //
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

// --- POCO Includes --- //
#include <Poco/Tuple.h>

#include <Poco/Data/SQLite/Connector.h>

using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

// --- STL Includes --- //
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
    //For debugging purposes
    //std::string junk; std::cin >> junk;

    Init( argc, argv );

    try
    {
        Main();
    }
    catch( Poco::Exception const& e )
    {
        std::cerr << "*** Poco exception caught: "
                  << e.what() << std::endl;
    }
    catch( std::exception const& e )
    {
        std::cerr << "*** STL exception caught: "
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

    //This name has to have .wjr as the extension
    fs::path wepsRunDir( "1" ); wepsRunDir/="weps.wjr";

    SQLite::Connector::registerConnector();

    weru::Init( wepsRunDir.string() );
}
////////////////////////////////////////////////////////////////////////////////
void Main()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    SQLite::Connector::unregisterConnector();

    weru::Exit();

    std::cout << "Exit Complete" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
