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
#include <leaf/wrapper/weru/mcrew/XMLConstants.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
XMLConstants::XMLConstants()
    :
    m_soperation( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "soperation" ) ) ),
    m_scrop( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "scrop" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
XMLConstants::~XMLConstants()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
XMLConstants& XMLConstants::Instance(
    bool const& erase )
{
    static XMLConstants* s_xmlConstants = new XMLConstants();
    if( erase )
    {
        delete s_xmlConstants;
        s_xmlConstants = NULL;
    }

    return *s_xmlConstants;
}
////////////////////////////////////////////////////////////////////////////////
void XMLConstants::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& XMLConstants::soperation()
{
    return Instance().m_soperation;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& XMLConstants::scrop()
{
    return Instance().m_scrop;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& XMLConstants::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/mcrew/XMLConstants" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& XMLConstants::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap;

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& XMLConstants::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "soperation",
          java::GetStaticFieldID(
              GetJclass(), "soperation",
              "Ljava/lang/String;" ) )
        ( "scrop",
          java::GetStaticFieldID(
              GetJclass(), "scrop",
              "Ljava/lang/String;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
