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
#include <leaf/wrapper/weru/util/Util.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace util
{

////////////////////////////////////////////////////////////////////////////////
Util::Util()
    :
    m_USUnits( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "USUnits" ) ) ),
    m_SIUnits( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "SIUnits" ) ) ),
    m_NoUnits( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "NoUnits" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Util::~Util()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Util& Util::Instance(
    bool const& erase )
{
    static Util* s_util = new Util();
    if( erase )
    {
        delete s_util;
        s_util = NULL;
    }

    return *s_util;
}
////////////////////////////////////////////////////////////////////////////////
void Util::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& Util::USUnits()
{
    return Instance().m_USUnits;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& Util::SIUnits()
{
    return Instance().m_SIUnits;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& Util::NoUnits()
{
    return Instance().m_NoUnits;
}
////////////////////////////////////////////////////////////////////////////////
misc::FilePtr Util::FindWindgenProjectionFile()
{
    return java::lang::Object::Create< misc::File >(
        java::CallStaticMethod< jobject >(
            GetJclass(), GetJmid( "findWindgenProjectionFile" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Util::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/util/Util" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Util::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "findWindgenProjectionFile",
          java::GetStaticMethodID(
              GetJclass(), "findWindgenProjectionFile",
              "()Lde/schlichtherle/io/File;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& Util::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "USUnits",
          java::GetStaticFieldID(
              GetJclass(), "USUnits",
              "Ljava/lang/String;" ) )
        ( "SIUnits",
          java::GetStaticFieldID(
              GetJclass(), "SIUnits",
              "Ljava/lang/String;" ) )
        ( "NoUnits",
          java::GetStaticFieldID(
              GetJclass(), "NoUnits",
              "Ljava/lang/String;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end util
} //end weru
} //end wrapper
} //end leaf
