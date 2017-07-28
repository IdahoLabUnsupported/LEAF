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
#include <leaf/wrapper/weru/mcrew/RowInfo.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
RowInfo::RowInfo(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
RowInfo::~RowInfo()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
java::util::Map< java::lang::String, DataObject >::SPtr
RowInfo::GetAllDataObjects() const
{
    return Create< java::util::Map< java::lang::String, DataObject > >(
        java::CallMethod< jobject >(
            m_jobject, GetJmid( "getAllDataObjects" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
JulianCalendarPtr RowInfo::GetDate() const
{
    return Create< JulianCalendar >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getDate" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
OperationObjectPtr RowInfo::GetOperation(
    java::util::Map< java::lang::String, DataObject > const& map )
{
    return boost::static_pointer_cast< OperationObject >(
        map.Get( "operation" ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& RowInfo::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/mcrew/RowInfo" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& RowInfo::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getDate",
          java::GetMethodID(
              GetJclass(), "getDate",
              "()Lusda/weru/mcrew/JulianCalendar;" ) )
        ( "getAllDataObjects",
          java::GetMethodID(
              GetJclass(), "getAllDataObjects",
              "()Ljava/util/Map;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
