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
#include <leaf/wrapper/weru/weps/reports/ReportPack.h>

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
ReportPack::ReportPack(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ReportPack::~ReportPack()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::StringPtr ReportPack::GetReportName() const
{
    return Create< java::lang::String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getReportName" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
bool ReportPack::IsFilling() const
{
    return ( java::CallMethod< jboolean >(
        m_jobject, GetJmid( "isFilling" ) ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
void ReportPack::Run() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "run" ) );
}
////////////////////////////////////////////////////////////////////////////////
void ReportPack::StartFill() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "startFill" ) );
}
////////////////////////////////////////////////////////////////////////////////
void ReportPack::WaitWhileFilling() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "waitWhileFilling" ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& ReportPack::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/reports/ReportPack" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& ReportPack::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getReportName",
          java::GetMethodID(
              GetJclass(), "getReportName",
              "()Ljava/lang/String;" ) )
        ( "isFilling",
          java::GetMethodID(
              GetJclass(), "isFilling",
              "()Z" ) )
        ( "run",
          java::GetMethodID(
              GetJclass(), "run",
              "()V" ) )
        ( "startFill",
          java::GetMethodID(
              GetJclass(), "startFill",
              "()V" ) )
        ( "waitWhileFilling",
          java::GetMethodID(
              GetJclass(), "waitWhileFilling",
              "()V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end reports
} //end weps
} //end weru
} //end wrapper
} //end leaf
