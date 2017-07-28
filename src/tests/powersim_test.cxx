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


// --- Windows Includes --- //
#if defined( WIN32 ) || defined( WIN64 )
#include <windows.h>
#include <shlwapi.h>
#endif

#include "powersim_test.h"

// --- LEAF Includes --- //
#include <leaf/wrapper/powersim/ComAutoInit.h>
#include <leaf/wrapper/powersim/SimulationEngine.h>
#include <leaf/wrapper/powersim/SimulationProject.h>
#include <leaf/wrapper/powersim/SimulationComponent.h>
#include <leaf/wrapper/powersim/Simulation.h>
#include <leaf/wrapper/powersim/ModelVariable.h>
#include <leaf/wrapper/powersim/Value.h>
#include <leaf/wrapper/powersim/Dimension.h>

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
    catch( ATL::CAtlException const& e )
    {
        std::stringstream ss;
        if( ( e.m_hr & 0xFFFF0000 ) == MAKE_HRESULT(
                SEVERITY_ERROR, FACILITY_WIN32, 0 ) )
        {
            ss << HRESULT_CODE( e.m_hr );
        }

        std::cerr << "*** CAtlException caught: HRESULT_CODE ="
                  << ss.str() << std::endl;
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
    //Get executable path
    char buffer[ MAX_PATH ];
    GetModuleFileNameA( NULL, buffer, sizeof( buffer ) );
    PathRemoveFileSpecA( buffer );
    m_exePath = fs::path( buffer );
    PathRemoveFileSpecA( buffer );
    m_installPath = fs::path( buffer );
    m_leafPath = fs::path( m_installPath/"share"/"leaf" );
    m_sipPath = fs::path( "sip" );

    SetCurrentDirectoryA( m_leafPath.string().c_str() );

    SQLite::Connector::registerConnector();

    powersim::CComAutoInit::Instance();
}
////////////////////////////////////////////////////////////////////////////////
void Main()
{
    powersim::SimulationEnginePtr simulationEngine =
        powersim::SimulationEngine::Create();
    powersim::SimulationProjectPtr simulationProject =
        simulationEngine->OpenProject(
            ( m_sipPath/"Algae Production Module Ver 1.sip" ).string() );
    powersim::SimulationComponentPtr simulationComponent =
        simulationProject->GetComponent( "Production" );
    powersim::SimulationPtr simulation =
        simulationComponent->GetSimulation( "Production Model" );
    powersim::ModelVariablePtr modelVariable =
        simulation->GetModelVariable( "Plant Cost Index" );
    powersim::ValuePtr value = modelVariable->GetValue();
    Poco::Dynamic::Var pdv = value->GetNumber();
    std::cout << pdv.convert< std::string >() << std::endl;
    pdv[ 0 ] = 0.0;
    pdv[ 1 ] = 7.8;
    value->PutNumber( pdv );
    pdv = value->GetNumber();
    std::cout << pdv.convert< std::string >() << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    powersim::CComAutoInit::Instance( true );
    SQLite::Connector::unregisterConnector();
}
////////////////////////////////////////////////////////////////////////////////
