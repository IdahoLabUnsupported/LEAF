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
#include <leaf/wrapper/weru/weps/reports/query/SciEnergyResultSet.h>

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
SciEnergyResultSet::SciEnergyResultSet(
    bool const& isStatic )
    :
    WepsResultSet( NULL )
{
    if( !isStatic ) return;

    m_COLUMN_SCI = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_SCI" ) ) );
    m_COLUMN_OMFACTOR = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_OMFACTOR" ) ) );
    m_COLUMN_ERFACTOR = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_ERFACTOR" ) ) );
    m_COLUMN_FOFACTOR = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_FOFACTOR" ) ) );
    m_COLUMN_WINDEROS = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_WINDEROS" ) ) );
    m_COLUMN_WATEREROS = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_WATEREROS" ) ) );
    m_NAME = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "NAME" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
SciEnergyResultSet::SciEnergyResultSet(
    jobject const& o )
    :
    WepsResultSet( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SciEnergyResultSet::~SciEnergyResultSet()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SciEnergyResultSet& SciEnergyResultSet::Instance(
    bool const& erase )
{
    static SciEnergyResultSet* s_sciEnergyResultSet =
        new SciEnergyResultSet( true );
    if( erase )
    {
        delete s_sciEnergyResultSet;
        s_sciEnergyResultSet = NULL;
    }

    return *s_sciEnergyResultSet;
}
////////////////////////////////////////////////////////////////////////////////
void SciEnergyResultSet::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& SciEnergyResultSet::COLUMN_SCI()
{
    return *Instance().m_COLUMN_SCI;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& SciEnergyResultSet::COLUMN_OMFACTOR()
{
    return *Instance().m_COLUMN_OMFACTOR;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& SciEnergyResultSet::COLUMN_ERFACTOR()
{
    return *Instance().m_COLUMN_ERFACTOR;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& SciEnergyResultSet::COLUMN_FOFACTOR()
{
    return *Instance().m_COLUMN_FOFACTOR;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& SciEnergyResultSet::COLUMN_WINDEROS()
{
    return *Instance().m_COLUMN_WINDEROS;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& SciEnergyResultSet::COLUMN_WATEREROS()
{
    return *Instance().m_COLUMN_WATEREROS;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& SciEnergyResultSet::NAME()
{
    return *Instance().m_NAME;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& SciEnergyResultSet::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/reports/query/SciEnergyResultSet" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& SciEnergyResultSet::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "NAME",
          java::GetStaticFieldID(
              GetJclass(), "NAME",
              "Ljava/lang/String;" ) )
        ( "COLUMN_SCI",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_SCI",
              "Ljava/lang/String;" ) )
        ( "COLUMN_OMFACTOR",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_OMFACTOR",
              "Ljava/lang/String;" ) )
        ( "COLUMN_ERFACTOR",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_ERFACTOR",
              "Ljava/lang/String;" ) )
        ( "COLUMN_FOFACTOR",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_FOFACTOR",
              "Ljava/lang/String;" ) )
        ( "COLUMN_WINDEROS",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_WINDEROS",
              "Ljava/lang/String;" ) )
        ( "COLUMN_WATEREROS",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_WATEREROS",
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
