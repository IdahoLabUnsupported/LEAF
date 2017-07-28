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
#include "Simulation.h"
#include "ModelVariable.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

////////////////////////////////////////////////////////////////////////////////
Simulation::Simulation(
    ATL::CComPtr< ::ISimulation > const& iS )
    :
    m_simulation( iS )
{
    //Get the Simulations Collection
    HRESULT hr = m_simulation->get_Variables( &m_modelVariables );
}
////////////////////////////////////////////////////////////////////////////////
Simulation::~Simulation()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ModelVariablePtr Simulation::GetModelVariable(
    std::string const& name ) const
{
    ATL::CComPtr< ::IModelVariable > iMV( NULL );
    HRESULT hr = m_modelVariables->Find(
        ATL::CComBSTR( name.c_str() ), &iMV );

    return boost::make_shared< ModelVariable >( iMV );
}
////////////////////////////////////////////////////////////////////////////////
void Simulation::Reset()
{
    HRESULT hr = m_simulation->Reset();
}
////////////////////////////////////////////////////////////////////////////////
void Simulation::Run(
    std::string const& name )
{
    if( !m_simulation )
    {
        return;
    }

    HRESULT hr;

    //ATL::CComVariant runName;
    //runName.vt = VT_BSTR;
    //runName.bstrVal = ATL::CComBSTR( name );

    //VARIANT_BOOL canActivateRun;
    //hr = m_simulation->CanActivateRun( runName, &canActivateRun );
    //if( !canActivateRun )
    //{
        //return;
    //}

    //hr = m_simulation->ActivateRun( runName );

    unsigned int runCount( 1 );
    //double startTime, stopTime, currTime, percent;
    //hr = m_simulation->get_StartTime( &startTime );
    //hr = m_simulation->get_StopTime( &stopTime );
    SimSimulationState ssState( simInitial );
    do
    {
        hr = m_simulation->Advance( &ssState );
        //hr = m_simulation->get_Time( &currTime );

        //percent = 100.0 * ( currTime - startTime ) / ( stopTime - startTime );

        if( ssState == simRunFinished )
        {
            ++runCount;
        }
    }
    while( ssState != simFinished );
}
////////////////////////////////////////////////////////////////////////////////
void Simulation::RunTimeStep()
{
    if( !m_simulation )
    {
        return;
    }

    HRESULT hr;

    unsigned int runCount( 1 );

    SimSimulationState ssState( simInitialInRun );

    hr = m_simulation->Advance( &ssState );

    if( ssState == simRunFinished )
    {
        ++runCount;
    }
}
////////////////////////////////////////////////////////////////////////////////
} //end powersim
} //end wrapper
} //end leaf
