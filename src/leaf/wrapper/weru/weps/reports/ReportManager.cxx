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
#include <leaf/wrapper/weru/weps/reports/ReportManager.h>

using namespace leaf::util;

// --- Boost Includes --- //
namespace fs = boost::filesystem;

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

////////////////////////////////////////////////////////////////////////////////
ReportManager::ReportManager()
    :
    java::lang::Object( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "getDefault" ) ) ),
    m_REPORT_DETAIL( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_DETAIL" ) ) ),
    m_REPORT_SOIL( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_SOIL" ) ) ),
    m_REPORT_DEBUG( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_DEBUG" ) ) ),
    m_REPORT_RUN( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_RUN" ) ) ),
    m_REPORT_MANAGEMENT( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_MANAGEMENT" ) ) ),
    m_REPORT_CROP( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_CROP" ) ) ),
    m_REPORT_STIR( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_STIR" ) ) ),
    m_REPORT_QUICKPLOT( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_QUICKPLOT" ) ) ),
    m_REPORT_DATABASE_CROPS( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_DATABASE_CROPS" ) ) ),
    m_REPORT_DATABASE_OPERATIONS( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_DATABASE_OPERATIONS" ) ) ),
    m_REPORT_DATABASE_MANAGEMENTS( java::GetStaticField< jobject >(
        GetJclass(), GetJfid( "REPORT_DATABASE_MANAGEMENTS" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ReportManager::~ReportManager()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ReportManager& ReportManager::Instance(
    bool const& erase )
{
    static ReportManager* s_reportManager = new ReportManager();
    if( erase )
    {
        delete s_reportManager;
        s_reportManager = NULL;
    }

    return *s_reportManager;
}
////////////////////////////////////////////////////////////////////////////////
void ReportManager::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_DETAIL()
{
    return Instance().m_REPORT_DETAIL;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_SOIL()
{
    return Instance().m_REPORT_SOIL;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_DEBUG()
{
    return Instance().m_REPORT_DEBUG;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_RUN()
{
    return Instance().m_REPORT_RUN;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_MANAGEMENT()
{
    return Instance().m_REPORT_MANAGEMENT;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_CROP()
{
    return Instance().m_REPORT_CROP;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_STIR()
{
    return Instance().m_REPORT_STIR;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_QUICKPLOT()
{
    return Instance().m_REPORT_QUICKPLOT;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_DATABASE_CROPS()
{
    return Instance().m_REPORT_DATABASE_CROPS;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_DATABASE_OPERATIONS()
{
    return Instance().m_REPORT_DATABASE_OPERATIONS;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::String const& ReportManager::REPORT_DATABASE_MANAGEMENTS()
{
    return Instance().m_REPORT_DATABASE_MANAGEMENTS;
}
////////////////////////////////////////////////////////////////////////////////
std::pair< std::string, ReportPackPtr >
ReportManager::GetReportPack(
    misc::File const& file,
    java::lang::String const& reportName )
{
    ReportPackPtr reportPack = java::lang::Object::Create< ReportPack >(
        java::CallMethod< jobject >( Instance().Jobj(),
            GetJmid( "getReportPack" ), file.Jobj(), reportName.Jobj() ) );
    return std::make_pair( reportName.StdString(), reportPack );
}
////////////////////////////////////////////////////////////////////////////////
query::WepsConnectionPtr ReportManager::GetConnection(
    misc::File const& file )
{
    return java::lang::Object::Create< query::WepsConnection >(
        java::CallMethod< jobject >( Instance().Jobj(),
            GetJmid( "getConnection" ), file.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
void ReportManager::GenerateReportPDFs(
    open::scn::ScenarioPtr const& scenario,
    fs::path const& repDir,
    fs::path const& runDir,
    unsigned int const& reportNames )
{
    if( reportNames == NONE ) return;

    REPORTS reports;
    misc::File runFile( runDir.string() );
    if( reportNames & DETAIL ) reports.insert(
        GetReportPack( runFile, REPORT_DETAIL() ) );
    if( reportNames & SOIL ) reports.insert(
        GetReportPack( runFile, REPORT_SOIL() ) );
    if( reportNames & DEBUG ) reports.insert(
        GetReportPack( runFile, REPORT_DEBUG() ) );
    if( reportNames & RUN ) reports.insert(
        GetReportPack( runFile, REPORT_RUN() ) );
    if( reportNames & MANAGEMENT ) reports.insert(
        GetReportPack( runFile, REPORT_MANAGEMENT() ) );
    if( reportNames & CROP ) reports.insert(
        GetReportPack( runFile, REPORT_CROP() ) );
    if( reportNames & STIR ) reports.insert(
        GetReportPack( runFile, REPORT_STIR() ) );
    if( reportNames & QUICKPLOT ) reports.insert(
        GetReportPack( runFile, REPORT_QUICKPLOT() ) );
    if( reportNames & DATABASE_CROPS ) reports.insert(
        GetReportPack( runFile, REPORT_DATABASE_CROPS() ) );
    if( reportNames & DATABASE_OPERATIONS ) reports.insert(
        GetReportPack( runFile, REPORT_DATABASE_OPERATIONS() ) );
    if( reportNames & DATABASE_MANAGEMENTS ) reports.insert(
        GetReportPack( runFile, REPORT_DATABASE_MANAGEMENTS() ) );

    REPORTS::const_iterator itr = reports.begin();
    for( itr = reports.begin(); itr != reports.end(); ++itr )
    {
        ReportPackPtr const& reportPack = itr->second;
        java::CallMethod< void >(
            Instance().Jobj(), GetJmid( "generateReportPDF" ),
            reportPack->Jobj(), runFile.Jobj() );
    }

    //Create report dir if not exists
    std::string scnid = Convert< std::string >( scenario->GetId() );
    fs::path pdfdir( repDir/scnid );
    fs::create_directories( pdfdir );

    //HACK: temporary for DDCE_2012 analysis
    std::string scnname = Convert< std::string >( scenario->GetName() );
    assert( reports.size() == 1 );
    for( itr = reports.begin(); itr != reports.end(); ++itr )
    {
        fs::path reportFile = runDir/( itr->first + ".pdf" );
        fs::path newFile( pdfdir/( scnname + ".pdf" ) );
        if( fs::exists( newFile ) ) fs::remove( newFile );
        fs::rename( reportFile, newFile );
    }
}
////////////////////////////////////////////////////////////////////////////////
jclass const& ReportManager::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/reports/ReportManager" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& ReportManager::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getDefault",
          java::GetStaticMethodID(
              GetJclass(), "getDefault",
              "()Lusda/weru/weps/reports/ReportManager;" ) )
        ( "getConnection",
          java::GetMethodID(
              GetJclass(), "getConnection",
              "(Lde/schlichtherle/io/File;)Lusda/weru/weps/reports/query/WepsConnection;" ) )
        ( "getReportPack",
          java::GetMethodID(
              GetJclass(), "getReportPack",
              "(Lde/schlichtherle/io/File;Ljava/lang/String;)Lusda/weru/weps/reports/ReportPack;" ) )
        ( "generateReportPDF",
          java::GetMethodID(
              GetJclass(), "generateReportPDF",
              "(Lusda/weru/weps/reports/ReportPack;Lde/schlichtherle/io/File;)V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& ReportManager::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "REPORT_DETAIL",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_DETAIL",
              "Ljava/lang/String;" ) )
        ( "REPORT_SOIL",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_SOIL",
              "Ljava/lang/String;" ) )
        ( "REPORT_DEBUG",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_DEBUG",
              "Ljava/lang/String;" ) )
        ( "REPORT_RUN",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_RUN",
              "Ljava/lang/String;" ) )
        ( "REPORT_MANAGEMENT",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_MANAGEMENT",
              "Ljava/lang/String;" ) )
        ( "REPORT_CROP",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_CROP",
              "Ljava/lang/String;" ) )
        ( "REPORT_STIR",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_STIR",
              "Ljava/lang/String;" ) )
        ( "REPORT_QUICKPLOT",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_QUICKPLOT",
              "Ljava/lang/String;" ) )
        ( "REPORT_DATABASE_CROPS",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_DATABASE_CROPS",
              "Ljava/lang/String;" ) )
        ( "REPORT_DATABASE_OPERATIONS",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_DATABASE_OPERATIONS",
              "Ljava/lang/String;" ) )
        ( "REPORT_DATABASE_MANAGEMENTS",
          java::GetStaticFieldID(
              GetJclass(), "REPORT_DATABASE_MANAGEMENTS",
              "Ljava/lang/String;" ) );
    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end reports
} //end weps
} //end weru
} //end wrapper
} //end leaf
