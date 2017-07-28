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
#include <leaf/wrapper/weru/misc/Measurable.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace misc
{

////////////////////////////////////////////////////////////////////////////////
Measurable::Measurable(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Measurable::~Measurable()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
double Measurable::DoubleValue(
    Unit const& unit ) const
{
    return java::CallMethod< jdouble >(
        m_jobject, GetJmid( "doubleValue" ), unit.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Measurable::GetJclass()
{
    static jclass const clazz = java::FindClass( "javax/measure/Measurable" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Measurable::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "doubleValue",
          java::GetMethodID(
              GetJclass(), "doubleValue",
              "(Ljavax/measure/unit/Unit;)D" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end misc
} //end weru
} //end wrapper
} //end leaf
