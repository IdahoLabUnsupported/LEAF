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
#include "SimulationProjectPtr.h"
#include "SimulationComponentPtr.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

class SimulationEngine;

class LEAF_POWERSIM_EXPORTS SimulationProject
{
public:
    ///Constructor
    SimulationProject(
        ATL::CComPtr< ::ISimulationProject > const& iSP );

    ///Destructor
    ~SimulationProject();

    ///
    SimulationComponentPtr GetComponent(
        std::string const& name ) const;

protected:

private:
    ///Restrict copying from copy constructor
    SimulationProject(
        SimulationProject const& );

    ///Restrict copying from equal operator
    SimulationProject& operator=(
        SimulationProject const& );

    ///
    ATL::CComPtr< ::ISimulationProject > m_simulationProject;

    ///
    ATL::CComPtr< ::ISimulationComponents > m_simulationComponents;

};

} //end powersim
} //end wrapper
} //end leaf
