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
#include <leaf/wrapper/weru/weps/location/Site.h>

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
Site::Site(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Site::Site(
    java::lang::String const& fips )
    :
    java::lang::Object( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "valueOf" ), fips.Jobj() ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Site::~Site()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
misc::LatLongPtr Site::GetLatLong() const
{
    return Create< misc::LatLong >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getLatLong" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Site::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/location/Site" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Site::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "valueOf",
          java::GetStaticMethodID(
              GetJclass(), "valueOf",
              "(Ljava/lang/String;)Lusda/weru/weps/location/Site;" ) )
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
} //end wrapper
} //end leaf
