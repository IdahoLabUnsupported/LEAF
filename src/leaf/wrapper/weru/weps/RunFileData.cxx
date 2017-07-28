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
#include <leaf/wrapper/weru/weps/RunFileData.h>

#include <leaf/wrapper/weru/util/ConfigData.h>

// --- Boost Includes --- //
#include <boost/lexical_cast.hpp>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{

////////////////////////////////////////////////////////////////////////////////
RunFileData::RunFileData(
    bool const& isStatic )
    :
    java::lang::Object( NULL )
{
    if( !isStatic )
    {
        m_jobject = java::NewObject( GetJclass(), GetJmid( "<init>" ), false );
        return;
    }

    m_AverageSlope = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "AverageSlope" ) ) );
    m_CalibFileName = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "CalibFileName" ) ) );
    m_CycleCount = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "CycleCount" ) ) );
    m_ManageFile = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "ManageFile" ) ) );
    m_RotationYears = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "RotationYears" ) ) );
    m_RunTypeDisp = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "RunTypeDisp" ) ) );
    m_SoilFile = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "SoilFile" ) ) );
    m_WaterErosionLoss = Create< java::lang::String >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "WaterErosionLoss" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
RunFileData::RunFileData(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
RunFileData::~RunFileData()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
RunFileData& RunFileData::Instance(
    bool const& erase )
{
    static RunFileData* s_runFileData = new RunFileData( true );
    if( erase )
    {
        delete s_runFileData;
        s_runFileData = NULL;
    }

    return *s_runFileData;
}
////////////////////////////////////////////////////////////////////////////////
void RunFileData::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::AverageSlope()
{
    return *Instance().m_AverageSlope;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::CalibFileName()
{
    return *Instance().m_CalibFileName;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::CycleCount()
{
    return *Instance().m_CycleCount;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::ManageFile()
{
    return *Instance().m_ManageFile;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::RotationYears()
{
    return *Instance().m_RotationYears;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::RunTypeDisp()
{
    return *Instance().m_RunTypeDisp;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::SoilFile()
{
    return *Instance().m_SoilFile;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& RunFileData::WaterErosionLoss()
{
    return *Instance().m_WaterErosionLoss;
}
////////////////////////////////////////////////////////////////////////////////
void RunFileData::FireAll() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "fireAll" ) );
}
////////////////////////////////////////////////////////////////////////////////
RunFileBeanPtr RunFileData::GetBean() const
{
    return Create< RunFileBean >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getBean" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr RunFileData::GetData(
    java::lang::String const& idxstr ) const
{
    return Create< java::lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getData" ), idxstr.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
void RunFileData::Initialize() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "initialize" ) );
}
////////////////////////////////////////////////////////////////////////////////
void RunFileData::SetData(
    java::lang::String const& idxstr,
    java::lang::String const& newData ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setData" ), idxstr.Jobj(), newData.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void RunFileData::SetRotationYears(
    java::lang::String const& manageFile ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setRotationYears" ), manageFile.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void RunFileData::UpdateDates() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "updateDates" ) );
}
////////////////////////////////////////////////////////////////////////////////
void RunFileData::WriteRunFile(
    java::lang::String const& runDirPath ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "writeRunFile" ), runDirPath.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& RunFileData::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/weps/RunFileData" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& RunFileData::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "(Z)V" ) )
        ( "fireAll",
          java::GetMethodID(
              GetJclass(), "fireAll",
              "()V" ) )
        ( "getBean",
          java::GetMethodID(
              GetJclass(), "getBean",
              "()Lusda/weru/weps/RunFileBean;" ) )
        ( "getData",
          java::GetMethodID(
              GetJclass(), "getData",
              "(Ljava/lang/String;)Ljava/lang/String;" ) )
        ( "initialize",
          java::GetMethodID(
              GetJclass(), "initialize",
              "()V" ) )
        ( "setData",
          java::GetMethodID(
              GetJclass(), "setData",
              "(Ljava/lang/String;Ljava/lang/String;)V" ) )
        ( "setRotationYears",
          java::GetMethodID(
              GetJclass(), "setRotationYears",
              "(Ljava/lang/String;)V" ) )
        ( "updateDates",
          java::GetMethodID(
              GetJclass(), "updateDates",
              "()V" ) )
        ( "writeRunFile",
          java::GetMethodID(
              GetJclass(), "writeRunFile",
              "(Ljava/lang/String;)V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& RunFileData::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "AverageSlope",
          java::GetStaticFieldID(
              GetJclass(), "AverageSlope",
              "Ljava/lang/String;" ) )
        ( "CalibFileName",
          java::GetStaticFieldID(
              GetJclass(), "CalibFileName",
              "Ljava/lang/String;" ) )
        ( "CycleCount",
          java::GetStaticFieldID(
              GetJclass(), "CycleCount",
              "Ljava/lang/String;" ) )
        ( "ManageFile",
          java::GetStaticFieldID(
              GetJclass(), "ManageFile",
              "Ljava/lang/String;" ) )
        ( "RotationYears",
          java::GetStaticFieldID(
              GetJclass(), "RotationYears",
              "Ljava/lang/String;" ) )
        ( "RunTypeDisp",
          java::GetStaticFieldID(
              GetJclass(), "RunTypeDisp",
              "Ljava/lang/String;" ) )
        ( "SoilFile",
          java::GetStaticFieldID(
              GetJclass(), "SoilFile",
              "Ljava/lang/String;" ) )
        ( "WaterErosionLoss",
          java::GetStaticFieldID(
              GetJclass(), "WaterErosionLoss",
              "Ljava/lang/String;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end weps
} //end weru
} //end wrapper
} //end leaf
