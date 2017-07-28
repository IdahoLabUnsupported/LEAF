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
#include <leaf/wrapper/weru/misc/LatLong.h>
#include <leaf/wrapper/weru/misc/NonSI.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace misc
{

////////////////////////////////////////////////////////////////////////////////
LatLong::LatLong(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
LatLong::LatLong(
    double const& latitude,
    double const& longitude )
    :
    java::lang::Object(
        java::CallStaticMethod< jobject >( GetJclass(), GetJmid( "valueOf" ),
            latitude, longitude, NonSI::DEGREE_ANGLE().Jobj() ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
LatLong::~LatLong()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
double LatLong::LatitudeValue() const
{
    return java::CallMethod< jdouble >( m_jobject,
        GetJmid( "latitudeValue" ), NonSI::DEGREE_ANGLE().Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
double LatLong::LongitudeValue() const
{
    return java::CallMethod< jdouble >( m_jobject,
        GetJmid( "longitudeValue" ), NonSI::DEGREE_ANGLE().Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& LatLong::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "org/jscience/geography/coordinates/LatLong" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& LatLong::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "valueOf",
          java::GetStaticMethodID(
              GetJclass(), "valueOf",
              "(DDLjavax/measure/unit/Unit;)Lorg/jscience/geography/coordinates/LatLong;" ) )
        ( "latitudeValue",
          java::GetMethodID(
              GetJclass(), "latitudeValue",
              "(Ljavax/measure/unit/Unit;)D" ) )
        ( "longitudeValue",
          java::GetMethodID(
              GetJclass(), "longitudeValue",
              "(Ljavax/measure/unit/Unit;)D" ) );
    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end misc
} //end weru
} //end wrapper
} //end leaf
