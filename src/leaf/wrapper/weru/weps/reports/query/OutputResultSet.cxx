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
#include <leaf/wrapper/weru/weps/reports/query/OutputResultSet.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{
namespace reports
{
namespace query
{

////////////////////////////////////////////////////////////////////////////////
OutputResultSet::OutputResultSet(
    bool const& isStatic )
    :
    WepsResultSet( NULL )
{
    if( !isStatic ) return;

    m_COLUMN_CROP = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_CROP" ) ) );
    m_COLUMN_PERIOD = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_PERIOD" ) ) );
    m_NAME = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "NAME" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
OutputResultSet::OutputResultSet(
    jobject const& o )
    :
    WepsResultSet( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
OutputResultSet::~OutputResultSet()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
OutputResultSet& OutputResultSet::Instance(
    bool const& erase )
{
    static OutputResultSet* s_outputResultSet = new OutputResultSet( true );
    if( erase )
    {
        delete s_outputResultSet;
        s_outputResultSet = NULL;
    }

    return *s_outputResultSet;
}
////////////////////////////////////////////////////////////////////////////////
void OutputResultSet::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& OutputResultSet::COLUMN_CROP()
{
    return *Instance().m_COLUMN_CROP;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& OutputResultSet::COLUMN_PERIOD()
{
    return *Instance().m_COLUMN_PERIOD;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& OutputResultSet::NAME()
{
    return *Instance().m_NAME;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& OutputResultSet::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/reports/query/OutputResultSet" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& OutputResultSet::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "COLUMN_CROP",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_CROP",
              "Ljava/lang/String;" ) )
        ( "COLUMN_PERIOD",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_PERIOD",
              "Ljava/lang/String;" ) )
        ( "NAME",
          java::GetStaticFieldID(
              GetJclass(), "NAME",
              "Ljava/lang/String;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end query
} //end reports
} //end weps
} //end weru
} //end wrapper
} //end leaf
