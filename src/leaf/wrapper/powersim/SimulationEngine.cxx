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
#include "Helper.h"
#include "SimulationEngine.h"
#include "SimulationProject.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

////////////////////////////////////////////////////////////////////////////////
SimulationEngine::SimulationEngine()
{
    HRESULT hr = m_simulationEngine.CoCreateInstance(
        CLSID_SimulationEngine, NULL, CLSCTX_INPROC_SERVER );
    assert( SUCCEEDED( hr ) );
}
////////////////////////////////////////////////////////////////////////////////
SimulationEngine::~SimulationEngine()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SimulationEnginePtr SimulationEngine::Create()
{
    return boost::make_shared< SimulationEngine >();
}
////////////////////////////////////////////////////////////////////////////////
SimulationProjectPtr SimulationEngine::OpenProject(
    std::string const& fileName,
    std::string const& key,
    std::string const& password,
    bool const& browseForFile ) const
{
    ATL::CComPtr< ::ISimulationProject > iSP( NULL );
    HRESULT hr = m_simulationEngine->OpenProject(
        ATL::CComBSTR( fileName.c_str() ),
        ATL::CComBSTR( key.c_str() ),
        ATL::CComBSTR( password.c_str() ),
        Convert< bool, VARIANT_BOOL >( browseForFile ),
        Convert< bool, VARIANT_BOOL >( true ),
        &iSP );
    if( !SUCCEEDED( hr ) )
    {
        throw std::runtime_error( "*** CAtlException caught: HRESULT_CODE =" );
    }

    return boost::make_shared< SimulationProject >( iSP );
}
////////////////////////////////////////////////////////////////////////////////

} //end powersim
} //end wrapper
} //end leaf
