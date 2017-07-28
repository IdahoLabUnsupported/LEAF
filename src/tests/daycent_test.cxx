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
#include "daycent_test.h"

// --- LEAF Includes --- //
#include <leaf/util/SoilHelper.h>

#include <leaf/open/soi/Soils.h>

#include <leaf/wrapper/daycent/api-daycent.h>
#include <leaf/wrapper/daycent/list/L_header.h>

using namespace leaf::util;
using namespace leaf::open;
using namespace leaf::wrapper;

// --- Boost Includes --- //
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

#include <boost/math/special_functions/round.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

// --- POCO Includes --- //
#include <Poco/Tuple.h>
#include <Poco/SharedPtr.h>

#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

// --- STL Includes --- //
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>


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
        ( "help,h", "produce help message" )
        ( "name,n", po::value< std::string >(),
          "set bin name" );

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    if( vm.count( "help" ) )
    {
        std::cout << std::endl << desc << std::endl;
        std::cout << std::endl << "Ex." << std::endl;
        std::cout << "  natass -f IA015 19017 -r CG,SB -t RT -m MRH -y 2010YLD"
                  << std::endl;
        exit( 0 );
    }

    //Set county fips
    if( vm.count( "name" ) )
    {
        m_binName = vm[ "name" ].as< std::string >();
    }

    SQLite::Connector::registerConnector();

    daycent::Init( fs::path( "daycent" ).string() );
}
////////////////////////////////////////////////////////////////////////////////
void Main()
{
    daycent::list::L_100& l100 = *daycent::list::L_100::instance();
    l100.Read( m_binName );

    daycent::list::RecordSet const& rs = l100.GetRecordSet();
    daycent::list::RecordByTime::const_iterator itr =
        rs.get< daycent::list::TIME >().begin();
    for( ; itr != rs.get< daycent::list::TIME >().end(); ++itr )
    {
        daycent::list::Record const& rec = **itr;
        std::cout << "time: " << std::setw( 10 ) << l100.GetValue( rec, "time" ) << "    ";
        std::cout << "somsc: " << std::setw( 10 ) << l100.GetValue( rec, "somsc" ) << std::endl;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    SQLite::Connector::unregisterConnector();

    daycent::Exit();

    std::cout << "Exit Complete" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
