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


%pragma( java ) jniclasscode =
%{
    static
    {
        try
        {
            System.loadLibrary( "leaf_wrapper_powersim_jni" );
        }
        catch( UnsatisfiedLinkError e )
        {
            System.err.println( "Native code library failed to load. \n" + e );
            System.exit( 1 );
        }
    }
%}

%include <exception.i>
%exception
{
    try
    {
        $action
    }
    catch( std::exception const& e )
    {
        SWIG_exception( SWIG_RuntimeError, e.what() );
    }
}

%{
    #define SWIG_JAVA_EXTRA_NATIVE_CONTAINERS
%}

%module leaf_wrapper_powersim
%{
    #include "ComAutoInit.h"
    #include "SimulationEngine.h"
    #include "SimulationProject.h"
    #include "SimulationComponent.h"
    #include "Simulation.h"
    #include "ModelVariable.h"
    #include "Value.h"
    #include "Dimension.h"

    using namespace leaf::wrapper::powersim;

    #include "Poco/Dynamic/Var.h"

    using namespace Poco;
    using namespace Poco::Dynamic;
%}

%include <windows.i>
#define __in

%include <std_string.i>
%include <std_vector.i>

%include <boost_shared_ptr.i>
%include "SimulationEnginePtr.h"
%include "SimulationProjectPtr.h"
%include "SimulationComponentPtr.h"
%include "SimulationPtr.h"
%include "ModelVariablePtr.h"
%include "ValuePtr.h"
%include "DimensionPtr.h"
%shared_ptr( leaf::wrapper::powersim::SimulationEngine )
%shared_ptr( leaf::wrapper::powersim::SimulationProject )
%shared_ptr( leaf::wrapper::powersim::SimulationComponent )
%shared_ptr( leaf::wrapper::powersim::Simulation )
%shared_ptr( leaf::wrapper::powersim::ModelVariable )
%shared_ptr( leaf::wrapper::powersim::Value )
%shared_ptr( leaf::wrapper::powersim::Dimension )

//In order of declaration needed
%include "Poco/Config.h"
%include "Poco/Platform.h"
%include "Poco/Platform_WIN32.h"
%include "Poco/Foundation.h"
%include "Poco/Types.h"
%include "Poco/Dynamic/VarHolder.h"

%ignore Poco::Dynamic::operator !;
%ignore Poco::Dynamic::operator !=;
%ignore Poco::Dynamic::operator ==;
%ignore Poco::Dynamic::operator -;
%ignore Poco::Dynamic::operator --;
%ignore Poco::Dynamic::operator -=;
%ignore Poco::Dynamic::operator +;
%ignore Poco::Dynamic::operator ++;
%ignore Poco::Dynamic::operator +=;
%ignore Poco::Dynamic::operator *;
%ignore Poco::Dynamic::operator *=;
%ignore Poco::Dynamic::operator /;
%ignore Poco::Dynamic::operator /=;
%ignore Poco::Dynamic::operator <;
%ignore Poco::Dynamic::operator <=;
%ignore Poco::Dynamic::operator >;
%ignore Poco::Dynamic::operator >=;

%ignore Poco::Dynamic::Var::operator ||;
%ignore Poco::Dynamic::Var::operator &&;
%ignore Poco::Dynamic::Var::operator !;
%ignore Poco::Dynamic::Var::operator !=;
%ignore Poco::Dynamic::Var::operator ==;
%ignore Poco::Dynamic::Var::operator -;
%ignore Poco::Dynamic::Var::operator --;
%ignore Poco::Dynamic::Var::operator -=;
%ignore Poco::Dynamic::Var::operator +;
%ignore Poco::Dynamic::Var::operator ++;
%ignore Poco::Dynamic::Var::operator +=;
%ignore Poco::Dynamic::Var::operator *;
%ignore Poco::Dynamic::Var::operator *=;
%ignore Poco::Dynamic::Var::operator /;
%ignore Poco::Dynamic::Var::operator /=;
%ignore Poco::Dynamic::Var::operator <;
%ignore Poco::Dynamic::Var::operator <=;
%ignore Poco::Dynamic::Var::operator >;
%ignore Poco::Dynamic::Var::operator >=;

%ignore Poco::Dynamic::Var::operator []( int ) const;
%include "Poco/Dynamic/Var.h"
%template( At ) Poco::Dynamic::Var::operator []< int >;
%template( Set ) Poco::Dynamic::Var::operator =< bool >;
%template( Set ) Poco::Dynamic::Var::operator =< int >;
%template( Set ) Poco::Dynamic::Var::operator =< double >;
%template( Set ) Poco::Dynamic::Var::operator =< Poco::Dynamic::Var >;
%template( ExtractBool ) Poco::Dynamic::Var::extract< bool >;
%template( ExtractInt ) Poco::Dynamic::Var::extract< int >;
%template( ExtractDouble ) Poco::Dynamic::Var::extract< double >;
%template( ConvertBool ) Poco::Dynamic::Var::convert< bool >;
%template( ConvertInt ) Poco::Dynamic::Var::convert< int >;
%template( ConvertDouble ) Poco::Dynamic::Var::convert< double >;
%template( ConvertString ) Poco::Dynamic::Var::convert< std::string >;

%include <leaf/LEAFConfig.h>

%ignore leaf::wrapper::powersim::Dimension::Dimension(
    ATL::CComPtr< ::IDimension > const& );
%include "Dimension.h"
%ignore leaf::wrapper::powersim::Value::Value(
    ATL::CComPtr< ::IValue > const& );
%include "Value.h"
%ignore leaf::wrapper::powersim::ModelVariable::ModelVariable(
    ATL::CComPtr< ::IModelVariable > const& );
%include "ModelVariable.h"
%ignore leaf::wrapper::powersim::Simulation::Simulation(
    ATL::CComPtr< ::ISimulation > const& );
%include "Simulation.h"
%ignore leaf::wrapper::powersim::SimulationComponent::SimulationComponent(
    ATL::CComPtr< ::ISimulationComponent > const& );
%include "SimulationComponent.h"
%ignore leaf::wrapper::powersim::SimulationProject::SimulationProject(
    ATL::CComPtr< ::ISimulationProject > const& );
%include "SimulationProject.h"
%ignore leaf::wrapper::powersim::SimulationEngine::SimulationEngine();
%include "SimulationEngine.h"
%include "ComAutoInit.h"
