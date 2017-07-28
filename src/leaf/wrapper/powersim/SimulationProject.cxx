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
#include "SimulationProject.h"
#include "SimulationComponent.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

////////////////////////////////////////////////////////////////////////////////
SimulationProject::SimulationProject(
    ATL::CComPtr< ::ISimulationProject > const& iSP )
    :
    m_simulationProject( iSP )
{
    //Get the SimulationComponents Collection
    HRESULT hr = m_simulationProject->get_Components( &m_simulationComponents );
}
////////////////////////////////////////////////////////////////////////////////
SimulationProject::~SimulationProject()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SimulationComponentPtr SimulationProject::GetComponent(
    std::string const& name ) const
{
    ATL::CComPtr< ::ISimulationComponent > iSC( NULL );
    HRESULT hr = m_simulationComponents->get_Item(
        ATL::CComVariant( name.c_str() ), &iSC );

    return boost::make_shared< SimulationComponent >( iSC );
}
////////////////////////////////////////////////////////////////////////////////

} //end powersim
} //end wrapper
} //end leaf
