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
#include <leaf/wrapper/weru/mcrew/OperationObject.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
OperationObject::OperationObject(
    jobject const& o )
    :
    DataObject( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
OperationObject::~OperationObject()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
bool OperationObject::HasCrop() const
{
    return ( java::CallMethod< jboolean >(
        m_jobject, GetJmid( "hasCrop" ) ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
CropObjectPtr OperationObject::GetCrop() const
{
    return Create< CropObject >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getCrop" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& OperationObject::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/mcrew/OperationObject" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& OperationObject::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "hasCrop",
          java::GetMethodID(
              GetJclass(), "hasCrop",
              "()Z" ) )
        ( "getCrop",
          java::GetMethodID(
              GetJclass(), "getCrop",
              "()Lusda/weru/mcrew/CropObject;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
