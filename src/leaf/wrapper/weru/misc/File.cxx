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
#include <leaf/wrapper/weru/misc/File.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace misc
{

////////////////////////////////////////////////////////////////////////////////
File::File(
    jobject const& o )
    :
    java::io::File( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
File::File(
    java::lang::String const& path )
    :
    java::io::File( NULL )
{
    m_jobject = java::NewObject(
        GetJclass(), GetJmid( "<init>" ), path.Jobj() );
    if( Exists() ) return;

    java::lang::StringPtr abspath = GetAbsolutePath();
    java::DeleteGlobalRef( m_jobject );
    m_jobject = java::NewObject(
        GetJclass(), GetJmid( "<init>" ), abspath->Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
File::~File()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr File::GetName() const
{
    return Create< java::lang::String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getName" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
java::io::FilePtr File::GetParentFile() const
{
    return Create< java::io::File >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getParentFile" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr File::GetPath() const
{
    return Create< java::lang::String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getPath" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& File::GetJclass()
{
    static jclass const clazz = java::FindClass( "de/schlichtherle/io/File" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& File::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "(Ljava/lang/String;)V" ) )
        ( "getName",
          java::GetMethodID(
              GetJclass(), "getName",
              "()Ljava/lang/String;" ) )
        ( "getParentFile",
          java::GetMethodID(
              GetJclass(), "getParentFile",
              "()Ljava/io/File;" ) )
        ( "getPath",
          java::GetMethodID(
              GetJclass(), "getPath",
              "()Ljava/lang/String;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end misc
} //end weru
} //end wrapper
} //end leaf
