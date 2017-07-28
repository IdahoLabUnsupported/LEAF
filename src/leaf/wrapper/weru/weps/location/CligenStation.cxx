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
#include <leaf/wrapper/weru/weps/location/CligenStation.h>

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
CligenStation::CligenStation(
    jobject const& o )
    :
    Station( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
CligenStation::~CligenStation()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Station::Type CligenStation::GetType() const
{
    return Station::CLIGEN;
}
////////////////////////////////////////////////////////////////////////////////
long CligenStation::GetState() const
{
    //This conversion may be incorrect
    return static_cast< long >(
        java::CallMethod< jlong >( m_jobject, GetJmid( "getState" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
long CligenStation::GetId() const
{
    //This conversion may be incorrect
    return static_cast< long >(
        java::CallMethod< jlong >( m_jobject, GetJmid( "getId" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& CligenStation::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/location/CligenStation" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& CligenStation::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getState",
          java::GetMethodID(
              GetJclass(), "getState",
              "()J" ) )
        ( "getId",
          java::GetMethodID(
              GetJclass(), "getId",
              "()J" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end location
} //end weps
} //end weru
} //end wrapper
} //end leaf
