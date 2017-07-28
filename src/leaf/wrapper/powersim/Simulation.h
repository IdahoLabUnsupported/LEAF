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
#include "SimulationPtr.h"
#include "ModelVariablePtr.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

class SimulationComponent;

class LEAF_POWERSIM_EXPORTS Simulation
{
public:
    ///Constructor
    Simulation(
        ATL::CComPtr< ::ISimulation > const& iS );

    ///Destructor
    ~Simulation();

    ///
    ModelVariablePtr GetModelVariable(
        std::string const& name ) const;

    ///
    void Reset();

    ///
    void Run(
        std::string const& name = "" );

    ///
    void RunTimeStep();

protected:

private:
    ///Restrict copying from copy constructor
    Simulation(
        Simulation const& );

    ///Restrict copying from equal operator
    Simulation& operator=(
        Simulation const& );

    ///
    ATL::CComPtr< ::ISimulation > m_simulation;

    ///
    ATL::CComPtr< ::IModelVariables > m_modelVariables;
};

} //end powersim
} //end wrapper
} //end leaf
