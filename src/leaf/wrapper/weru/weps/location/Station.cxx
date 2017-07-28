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



// --- LEAF Includes --- //
#include <leaf/wrapper/weru/weps/location/Station.h>
#include <leaf/wrapper/weru/weps/location/FileStation.h>
#include <leaf/wrapper/weru/weps/location/CligenStation.h>
#include <leaf/wrapper/weru/weps/location/WindgenStation.h>
#include <leaf/wrapper/weru/weps/location/InterpolatedStation.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{
namespace location
{

////////////////////////////////////////////////////////////////////////////////
Station::Station(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Station::~Station()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
misc::LatLongPtr Station::GetLatLong() const
{
    return Create< misc::LatLong >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getLatLong" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Station::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/location/Station" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Station::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getLatLong",
          java::GetMethodID(
              GetJclass(), "getLatLong",
              "()Lorg/jscience/geography/coordinates/LatLong;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end location
} //end weps
} //end weru

namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
template<>
weru::weps::location::StationPtr
Object::Create< weru::weps::location::Station >(
    jobject const& o )
{
    using namespace weru::weps::location;
    if( IsInstanceOf( o, CligenStation::GetJclass() ) )
    {
        return Object::Create< CligenStation >( o );
    }
    else if( IsInstanceOf( o, FileStation::GetJclass() ) )
    {
        return Object::Create< FileStation >( o );
    }
    else if( IsInstanceOf( o, InterpolatedStation::GetJclass() ) )
    {
        return Object::Create< InterpolatedStation >( o );
    }
    else if( IsInstanceOf( o, WindgenStation::GetJclass() ) )
    {
        return Object::Create< WindgenStation >( o );
    }
    else
    {
        return StationPtr();
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java

} //end wrapper
} //end leaf

