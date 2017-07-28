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
#include <leaf/wrapper/weru/util/ConfigData.h>
#include <leaf/wrapper/weru/util/Util.h>

#include <leaf/wrapper/weru/misc/SI.h>
#include <leaf/wrapper/weru/misc/Measurable.h>

// --- Boost Includes --- //
#include <boost/lexical_cast.hpp>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace util
{

////////////////////////////////////////////////////////////////////////////////
ConfigData::ConfigData()
    :
    java::lang::Object( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "getDefault" ) ) ),
    m_CliData( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "CliData" ) ) ),
    m_CliExe( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "CliExe" ) ) ),
    m_CurrentProj( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "CurrentProj" ) ) ),
    m_Cycle( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "Cycle" ) ) ),
    m_Dates( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "Dates" ) ) ),
    m_DetailTableFilterFile( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "DetailTableFilterFile" ) ) ),
    m_MCREW( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "MCREW" ) ) ),
    m_McrewDataConfigFile( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "McrewDataConfigFile" ) ) ),
    m_NRCS( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "NRCS" ) ) ),
    m_OMFractionThreshold( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "OMFractionThreshold" ) ) ),
    m_ProjDir( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "ProjDir" ) ) ),
    m_ReportsCustomized( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "ReportsCustomized" ) ) ),
    m_ReportFileName( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "ReportFileName" ) ) ),
    m_SingleProjectMode( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "SingleProjectMode" ) ) ),
    m_Units( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "Units" ) ) ),
    m_WepsExe( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "WepsExe" ) ) ),
    m_WinData( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "WinData" ) ) ),
    m_WinExe( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "WinExe" ) ) ),
    m_WinIndex( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "WinIndex" ) ) ),
    m_WindInterp1EXE( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "WindInterp1EXE" ) ) ),
    m_WindInterp2EXE( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "WindInterp2EXE" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ConfigData::~ConfigData()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ConfigData& ConfigData::Instance(
    bool const& erase )
{
    static ConfigData* s_configData = new ConfigData();
    if( erase )
    {
        delete s_configData;
        s_configData = NULL;
    }

    return *s_configData;
}
////////////////////////////////////////////////////////////////////////////////
void ConfigData::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::CliData()
{
    return Instance().m_CliData;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::CliExe()
{
    return Instance().m_CliExe;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::CurrentProj()
{
    return Instance().m_CurrentProj;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::Cycle()
{
    return Instance().m_Cycle;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::Dates()
{
    return Instance().m_Dates;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::DetailTableFilterFile()
{
    return Instance().m_DetailTableFilterFile;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::MCREW()
{
    return Instance().m_MCREW;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::McrewDataConfigFile()
{
    return Instance().m_McrewDataConfigFile;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::NRCS()
{
    return Instance().m_NRCS;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::OMFractionThreshold()
{
    return Instance().m_OMFractionThreshold;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::ProjDir()
{
    return Instance().m_ProjDir;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::ReportsCustomized()
{
    return Instance().m_ReportsCustomized;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::ReportFileName()
{
    return Instance().m_ReportFileName;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::SingleProjectMode()
{
    return Instance().m_SingleProjectMode;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::Units()
{
    return Instance().m_Units;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::WepsExe()
{
    return Instance().m_WepsExe;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::WinData()
{
    return Instance().m_WinData;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::WinExe()
{
    return Instance().m_WinExe;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::WinIndex()
{
    return Instance().m_WinIndex;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::WindInterp1EXE()
{
    return Instance().m_WindInterp1EXE;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ConfigData::WindInterp2EXE()
{
    return Instance().m_WindInterp2EXE;
}
////////////////////////////////////////////////////////////////////////////////
void ConfigData::FireAll()
{
    java::CallMethod< void >( Instance().Jobj(), GetJmid( "fireAll" ) );
}
////////////////////////////////////////////////////////////////////////////////
double ConfigData::GetOmFractionThreshold()
{
    return boost::lexical_cast< double >(
        GetData( OMFractionThreshold() )->StdString() );
}
////////////////////////////////////////////////////////////////////////////////
bool ConfigData::GetSoilTestOrganic()
{
    return ( java::CallMethod< jboolean >( Instance().Jobj(),
        GetJmid( "getSoilTestOrganic" ) ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
double ConfigData::GetSoilMaxOrganicDepth()
{
    misc::Measurable measurable( java::CallMethod< jobject >(
        Instance().Jobj(), GetJmid( "getSoilMaxOrganicDepth" ) ) );

    return measurable.DoubleValue( misc::SI::MILLIMETER() );
}
////////////////////////////////////////////////////////////////////////////////
bool ConfigData::IsAverageStratifiedSoilLayers()
{
    return ( java::CallMethod< jboolean >( Instance().Jobj(),
        GetJmid( "isAverageStratifiedSoilLayers" ) ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
bool ConfigData::IsSkipOrganicSoilSurfaceLayers()
{
    return ( java::CallMethod< jboolean >( Instance().Jobj(),
        GetJmid( "isSkipOrganicSoilSurfaceLayers" ) ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
void ConfigData::Load(
    misc::File const& main,
    misc::File const& user )
{
    java::CallMethod< void >(
        Instance().Jobj(), GetJmid( "load" ), main.Jobj(), user.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr ConfigData::GetData(
    java::lang::String const& propertyName )
{
    return java::lang::Object::Create< java::lang::String >(
        java::CallMethod< jobject >( Instance().Jobj(),
            GetJmid( "getData" ), propertyName.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr ConfigData::GetDataParsed(
    java::lang::String const& propertyName )
{
    return java::lang::Object::Create< java::lang::String >(
        java::CallMethod< jobject >( Instance().Jobj(),
            GetJmid( "getDataParsed" ), propertyName.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
void ConfigData::SetData(
    java::lang::String const& propertyName,
    java::lang::String const& newData )
{
    java::CallMethod< void >( Instance().Jobj(),
        GetJmid( "setData" ), propertyName.Jobj(), newData.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& ConfigData::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/util/ConfigData" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& ConfigData::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "fireAll",
          java::GetMethodID(
              GetJclass(), "fireAll",
              "()V" ) )
        ( "getData",
          java::GetMethodID(
              GetJclass(), "getData",
              "(Ljava/lang/String;)Ljava/lang/String;" ) )
        ( "getDataParsed",
          java::GetMethodID(
              GetJclass(), "getDataParsed",
              "(Ljava/lang/String;)Ljava/lang/String;" ) )
        ( "getDefault",
          java::GetStaticMethodID(
              GetJclass(), "getDefault",
              "()Lusda/weru/util/ConfigData;" ) )
        ( "load",
          java::GetMethodID(
              GetJclass(), "load",
              "(Lde/schlichtherle/io/File;Lde/schlichtherle/io/File;)V" ) )
        ( "getSoilTestOrganic",
          java::GetMethodID(
              GetJclass(), "getSoilTestOrganic",
              "()Z" ) )
        ( "getSoilMaxOrganicDepth",
          java::GetMethodID(
              GetJclass(), "getSoilMaxOrganicDepth",
              "()Ljavax/measure/Measurable;" ) )
        ( "isAverageStratifiedSoilLayers",
          java::GetMethodID(
              GetJclass(), "isAverageStratifiedSoilLayers",
              "()Z" ) )
        ( "isSkipOrganicSoilSurfaceLayers",
          java::GetMethodID(
              GetJclass(), "isSkipOrganicSoilSurfaceLayers",
              "()Z" ) )
        ( "setData",
          java::GetMethodID(
              GetJclass(), "setData",
              "(Ljava/lang/String;Ljava/lang/String;)V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& ConfigData::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "CliData",
          java::GetStaticFieldID(
              GetJclass(), "CliData",
              "Ljava/lang/String;" ) )
        ( "CliExe",
          java::GetStaticFieldID(
              GetJclass(), "CliExe",
              "Ljava/lang/String;" ) )
        ( "CurrentProj",
          java::GetStaticFieldID(
              GetJclass(), "CurrentProj",
              "Ljava/lang/String;" ) )
        ( "Cycle",
          java::GetStaticFieldID(
              GetJclass(), "Cycle",
              "Ljava/lang/String;" ) )
        ( "Dates",
          java::GetStaticFieldID(
              GetJclass(), "Dates",
              "Ljava/lang/String;" ) )
        ( "DetailTableFilterFile",
          java::GetStaticFieldID(
              GetJclass(), "DetailTableFilterFile",
              "Ljava/lang/String;" ) )
        ( "MCREW",
          java::GetStaticFieldID(
              GetJclass(), "MCREW",
              "Ljava/lang/String;" ) )
        ( "McrewDataConfigFile",
          java::GetStaticFieldID(
              GetJclass(), "McrewDataConfigFile",
              "Ljava/lang/String;" ) )
        ( "NRCS",
          java::GetStaticFieldID(
              GetJclass(), "NRCS",
              "Ljava/lang/String;" ) )
        ( "OMFractionThreshold",
          java::GetStaticFieldID(
              GetJclass(), "OMFractionThreshold",
              "Ljava/lang/String;" ) )
        ( "ProjDir",
          java::GetStaticFieldID(
              GetJclass(), "ProjDir",
              "Ljava/lang/String;" ) )
        ( "ReportsCustomized",
          java::GetStaticFieldID(
              GetJclass(), "ReportsCustomized",
              "Ljava/lang/String;" ) )
        ( "ReportFileName",
          java::GetStaticFieldID(
              GetJclass(), "ReportFileName",
              "Ljava/lang/String;" ) )
        ( "SingleProjectMode",
          java::GetStaticFieldID(
              GetJclass(), "SingleProjectMode",
              "Ljava/lang/String;" ) )
        ( "Units",
          java::GetStaticFieldID(
              GetJclass(), "Units",
              "Ljava/lang/String;" ) )
        ( "WepsExe",
          java::GetStaticFieldID(
              GetJclass(), "WepsExe",
              "Ljava/lang/String;" ) )
        ( "WinData",
          java::GetStaticFieldID(
              GetJclass(), "WinData",
              "Ljava/lang/String;" ) )
        ( "WinExe",
          java::GetStaticFieldID(
              GetJclass(), "WinExe",
              "Ljava/lang/String;" ) )
        ( "WinIndex",
          java::GetStaticFieldID(
              GetJclass(), "WinIndex",
              "Ljava/lang/String;" ) )
        ( "WindInterp1EXE",
          java::GetStaticFieldID(
              GetJclass(), "WindInterp1EXE",
              "Ljava/lang/String;" ) )
        ( "WindInterp2EXE",
          java::GetStaticFieldID(
              GetJclass(), "WindInterp2EXE",
              "Ljava/lang/String;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end util
} //end weru
} //end wrapper
} //end leaf
