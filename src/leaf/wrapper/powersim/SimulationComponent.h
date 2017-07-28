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
#include "Common.h"
#include "SimulationComponentPtr.h"
#include "SimulationPtr.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

class SimulationProject;

class LEAF_POWERSIM_EXPORTS SimulationComponent
{
public:
    ///Constructor
    SimulationComponent(
        ATL::CComPtr< ::ISimulationComponent > const& iSC );

    ///Destructor
    ~SimulationComponent();

    ///
    SimulationPtr GetSimulation(
        std::string const& name ) const;

protected:

private:
    ///Restrict copying from copy constructor
    SimulationComponent(
        SimulationComponent const& );

    ///Restrict copying from equal operator
    SimulationComponent& operator=(
        SimulationComponent const& );

    ///
    ATL::CComPtr< ::ISimulationComponent > m_simulationComponent;

    ///
    ATL::CComPtr< ::ISimulations > m_simulations;

};

} //end powersim
} //end wrapper
} //end leaf
