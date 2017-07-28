// Copyright 2016 Battelle Energy Alliance, LLC
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
#include <leaf/wrapper/weru/mcrew/JulianCalendar.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
JulianCalendar::JulianCalendar(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
JulianCalendar::JulianCalendar(
    java::lang::String const& pCalStr )
    :
    java::lang::Object( java::NewObject(
        GetJclass(), GetJmid( "<init>" ), pCalStr.Jobj(), pCalStr.Jobj() ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
JulianCalendar::~JulianCalendar()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr JulianCalendar::ToString()
{
    return Create< java::lang::String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "toString" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& JulianCalendar::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/mcrew/JulianCalendar" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& JulianCalendar::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "(Ljava/lang/String;Ljava/lang/String;)V" ) )
        ( "toString",
          java::GetMethodID(
              GetJclass(), "toString",
              "()Ljava/lang/String;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
