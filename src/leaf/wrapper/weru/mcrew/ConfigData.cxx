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
#include <leaf/wrapper/weru/mcrew/ConfigData.h>

#include <leaf/wrapper/weru/util/ConfigData.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
ConfigData::~ConfigData()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void ConfigData::Initialize()
{
    java::CallStaticMethod< void >( GetJclass(),
        GetJmid( "initialize" ), weru::util::ConfigData::Instance().Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& ConfigData::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/mcrew/ConfigData" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& ConfigData::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "initialize",
          java::GetStaticMethodID(
              GetJclass(), "initialize",
              "(Lusda/weru/util/ConfigData;)V" ) )
        ( "getDirectoryName",
          java::GetStaticMethodID(
              GetJclass(), "getDirectoryName",
              "(Ljava/lang/String;)Ljava/lang/String;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
