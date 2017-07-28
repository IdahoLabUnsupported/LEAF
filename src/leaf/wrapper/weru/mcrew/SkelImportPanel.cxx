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
#include <leaf/wrapper/weru/mcrew/SkelImportPanel.h>
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
SkelImportPanel::SkelImportPanel()
    :
    java::lang::Object( java::NewObject( GetJclass(), GetJmid( "<init>" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SkelImportPanelPtr SkelImportPanel::Create()
{
    return boost::make_shared< SkelImportPanel >();
}
////////////////////////////////////////////////////////////////////////////////
SkelImportPanel::~SkelImportPanel()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
bool SkelImportPanel::SetDatabases(
    java::lang::String const& opdb,
    java::lang::String const& vegdb )
{
    return ( java::CallMethod< jboolean >( m_jobject,
        GetJmid( "setDatabases" ), opdb.Jobj(), vegdb.Jobj() ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
bool SkelImportPanel::SkelToMan(
    java::lang::String const& filename,
    java::lang::String const& manfile )
{
    return ( java::CallMethod< jboolean >( m_jobject,
        GetJmid( "skel2man" ), filename.Jobj(), manfile.Jobj() ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& SkelImportPanel::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/mcrew/SkelImportPanel" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& SkelImportPanel::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "()V" ) )
        ( "setDatabases",
          java::GetMethodID(
              GetJclass(), "setDatabases",
              "(Ljava/lang/String;Ljava/lang/String;)Z" ) )
        ( "skel2man",
          java::GetMethodID(
              GetJclass(), "skel2man",
              "(Ljava/lang/String;Ljava/lang/String;)Z" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
