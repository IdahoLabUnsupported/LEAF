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
NonSI::NonSI()
    :
    java::lang::Object( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "getInstance" ) ) ),
    m_DEGREE_ANGLE( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "DEGREE_ANGLE" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
NonSI::~NonSI()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
NonSI& NonSI::Instance(
    bool const& erase )
{
    static NonSI* s_nonSI = new NonSI();
    if( erase )
    {
        delete s_nonSI;
        s_nonSI = NULL;
    }

    return *s_nonSI;
}
////////////////////////////////////////////////////////////////////////////////
void NonSI::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
Unit const& NonSI::DEGREE_ANGLE()
{
    return Instance().m_DEGREE_ANGLE;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& NonSI::GetJclass()
{
    static jclass const clazz = java::FindClass( "javax/measure/unit/NonSI" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& NonSI::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getInstance",
          java::GetStaticMethodID(
              GetJclass(), "getInstance",
              "()Ljavax/measure/unit/NonSI;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& NonSI::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "DEGREE_ANGLE",
          java::GetStaticFieldID(
              GetJclass(), "DEGREE_ANGLE",
              "Ljavax/measure/unit/Unit;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end misc
} //end weru
} //end wrapper
} //end leaf
