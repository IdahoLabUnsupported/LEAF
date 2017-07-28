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
#include <leaf/wrapper/weru/misc/SI.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace misc
{

////////////////////////////////////////////////////////////////////////////////
SI::SI()
    :
    java::lang::Object( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "getInstance" ) ) ),
    m_MILLIMETER( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "MILLIMETER" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SI::~SI()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SI& SI::Instance(
    bool const& erase )
{
    static SI* s_si = new SI();
    if( erase )
    {
        delete s_si;
        s_si = NULL;
    }

    return *s_si;
}
////////////////////////////////////////////////////////////////////////////////
void SI::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
Unit const& SI::MILLIMETER()
{
    return Instance().m_MILLIMETER;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& SI::GetJclass()
{
    static jclass const clazz = java::FindClass( "javax/measure/unit/SI" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& SI::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getInstance",
          java::GetStaticMethodID(
              GetJclass(), "getInstance",
              "()Ljavax/measure/unit/SI;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& SI::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "MILLIMETER",
          java::GetStaticFieldID(
              GetJclass(), "MILLIMETER",
              "Ljavax/measure/unit/Unit;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end misc
} //end weru
} //end wrapper
} //end leaf
