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
#include <leaf/wrapper/weru/mcrew/CropObject.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
CropObject::CropObject(
    jobject const& o )
    :
    DataObject( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
CropObject::~CropObject()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr CropObject::GetCropName() const
{
    return Create< java::lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getCropName" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr CropObject::GetValue(
    java::lang::String const& pColName ) const
{
    return Create< java::lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getValue" ), pColName.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr CropObject::GetValue(
    java::lang::String const& pColName,
    int const& pUnit ) const
{
    return Create< java::lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getValueI" ), pColName.Jobj(), pUnit ) );
}
////////////////////////////////////////////////////////////////////////////////
void CropObject::SetValue(
    java::lang::String const& pParamName,
    java::lang::String const& pNewValue,
    int const& pUnit ) const
{
    java::CallMethod< jobject >( m_jobject,
        GetJmid( "setValue" ), pParamName.Jobj(), pNewValue.Jobj(), pUnit );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& CropObject::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/mcrew/CropObject" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& CropObject::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getCropName",
          java::GetMethodID(
              GetJclass(), "getCropName",
              "()Ljava/lang/String;" ) )
        ( "getValue",
          java::GetMethodID(
              GetJclass(), "getValue",
              "(Ljava/lang/String;)Ljava/lang/String;" ) )
        ( "getValueI",
          java::GetMethodID(
              GetJclass(), "getValue",
              "(Ljava/lang/String;I)Ljava/lang/String;" ) )
        ( "setValue",
          java::GetMethodID(
              GetJclass(), "setValue",
              "(Ljava/lang/String;Ljava/lang/String;I)V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
