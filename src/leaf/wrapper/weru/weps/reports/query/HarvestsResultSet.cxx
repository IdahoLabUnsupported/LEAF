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
#include <leaf/wrapper/weru/weps/reports/query/HarvestsResultSet.h>

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
HarvestsResultSet::HarvestsResultSet(
    bool const& isStatic )
    :
    WepsResultSet( NULL )
{
    if( !isStatic ) return;

    m_COLUMN_CROP = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_CROP" ) ) );
    m_COLUMN_CROPINDEX = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_CROPINDEX" ) ) );
    m_COLUMN_CYCLENUMBER = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_CYCLENUMBER" ) ) );
    m_COLUMN_DRYYIELD = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_DRYYIELD" ) ) );
    m_COLUMN_HARVESTDATE = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_HARVESTDATE" ) ) );
    m_COLUMN_INDEX = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_INDEX" ) ) );
    m_COLUMN_MOISTURE = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_MOISTURE" ) ) );
    m_COLUMN_RESIDUE = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_RESIDUE" ) ) );
    m_COLUMN_RUNID = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_RUNID" ) ) );
    m_COLUMN_YIELD = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_YIELD" ) ) );
    m_COLUMN_YIELDUNITS = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "COLUMN_YIELDUNITS" ) ) );
    m_NAME = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "NAME" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
HarvestsResultSet::HarvestsResultSet(
    jobject const& o )
    :
    WepsResultSet( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
HarvestsResultSet::~HarvestsResultSet()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
HarvestsResultSet& HarvestsResultSet::Instance(
    bool const& erase )
{
    static HarvestsResultSet* s_sciEnergyResultSet =
        new HarvestsResultSet( true );
    if( erase )
    {
        delete s_sciEnergyResultSet;
        s_sciEnergyResultSet = NULL;
    }

    return *s_sciEnergyResultSet;
}
////////////////////////////////////////////////////////////////////////////////
void HarvestsResultSet::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_CROP()
{
    return *Instance().m_COLUMN_CROP;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_CROPINDEX()
{
    return *Instance().m_COLUMN_CROPINDEX;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_CYCLENUMBER()
{
    return *Instance().m_COLUMN_CYCLENUMBER;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_DRYYIELD()
{
    return *Instance().m_COLUMN_DRYYIELD;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_HARVESTDATE()
{
    return *Instance().m_COLUMN_HARVESTDATE;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_INDEX()
{
    return *Instance().m_COLUMN_INDEX;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_MOISTURE()
{
    return *Instance().m_COLUMN_MOISTURE;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_RESIDUE()
{
    return *Instance().m_COLUMN_RESIDUE;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_RUNID()
{
    return *Instance().m_COLUMN_RUNID;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_YIELD()
{
    return *Instance().m_COLUMN_YIELD;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::COLUMN_YIELDUNITS()
{
    return *Instance().m_COLUMN_YIELDUNITS;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& HarvestsResultSet::NAME()
{
    return *Instance().m_NAME;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& HarvestsResultSet::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/reports/query/HarvestsResultSet" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& HarvestsResultSet::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "COLUMN_CROP",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_CROP",
              "Ljava/lang/String;" ) )
        ( "COLUMN_CROPINDEX",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_CROPINDEX",
              "Ljava/lang/String;" ) )
        ( "COLUMN_CYCLENUMBER",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_CYCLENUMBER",
              "Ljava/lang/String;" ) )
        ( "COLUMN_DRYYIELD",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_DRYYIELD",
              "Ljava/lang/String;" ) )
        ( "COLUMN_HARVESTDATE",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_HARVESTDATE",
              "Ljava/lang/String;" ) )
        ( "COLUMN_INDEX",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_INDEX",
              "Ljava/lang/String;" ) )
        ( "COLUMN_MOISTURE",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_MOISTURE",
              "Ljava/lang/String;" ) )
        ( "COLUMN_RESIDUE",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_RESIDUE",
              "Ljava/lang/String;" ) )
        ( "COLUMN_RUNID",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_RUNID",
              "Ljava/lang/String;" ) )
        ( "COLUMN_YIELD",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_YIELD",
              "Ljava/lang/String;" ) )
        ( "COLUMN_YIELDUNITS",
          java::GetStaticFieldID(
              GetJclass(), "COLUMN_YIELDUNITS",
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
