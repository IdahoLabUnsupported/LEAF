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
#include <leaf/wrapper/weru/mcrew/ManageData.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
ManageData::ManageData()
    :
    java::lang::Object( java::NewObject( GetJclass(), GetJmid( "<init>" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ManageData::ManageData(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ManageData::~ManageData()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
int const& ManageData::CHECK_PASSED()
{
    static int const CHECK_PASSED = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "CHECK_PASSED" ) );
    return CHECK_PASSED;
}
////////////////////////////////////////////////////////////////////////////////
int const& ManageData::CHECK_FAILED()
{
    static int const CHECK_FAILED = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "CHECK_FAILED" ) );
    return CHECK_FAILED;
}
////////////////////////////////////////////////////////////////////////////////
int const& ManageData::kSuccess()
{
    static int const kSuccess = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "kSuccess" ) );
    return kSuccess;
}
////////////////////////////////////////////////////////////////////////////////
bool ManageData::CheckAllConditions(
    java::util::Vector< java::lang::String > const& msgs ) const
{
    int check = java::CallMethod< jint >(
        m_jobject, GetJmid( "checkAllConditions" ), msgs.Jobj() );
    return ( check == CHECK_PASSED() );
}
////////////////////////////////////////////////////////////////////////////////
int ManageData::CheckRotationYears() const
{
    return java::CallMethod< jint >(
        m_jobject, GetJmid( "checkRotationYears" ) );
}
////////////////////////////////////////////////////////////////////////////////
int ManageData::GetRotationYears() const
{
    return java::CallMethod< jint >(
        m_jobject, GetJmid( "getRotationYears" ) );
}
////////////////////////////////////////////////////////////////////////////////
java::util::Vector< RowInfo >::SPtr ManageData::GetRows() const
{
    return Create< java::util::Vector< RowInfo > >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getRows" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
bool ManageData::ReadDataFile(
    java::lang::String const& pFileName ) const
{
    int success = java::CallMethod< jint >(
        m_jobject, GetJmid( "readDataFile" ), pFileName.Jobj() );
    return ( success == kSuccess() );
}
////////////////////////////////////////////////////////////////////////////////
bool ManageData::WriteDataFile(
    java::lang::String const& pFileName ) const
{
    int success = java::CallMethod< jint >(
        m_jobject, GetJmid( "writeDataFile" ), pFileName.Jobj() );
    return ( success == kSuccess() );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& ManageData::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/mcrew/ManageData" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& ManageData::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "()V" ) )
        ( "checkAllConditions",
          java::GetMethodID(
            GetJclass(), "checkAllConditions",
              "(Ljava/util/List;)I" ) )
        ( "checkRotationYears",
          java::GetMethodID(
            GetJclass(), "checkRotationYears",
              "()I" ) )
        ( "getRotationYears",
          java::GetMethodID(
            GetJclass(), "getRotationYears",
              "()I" ) )
        ( "getRows",
          java::GetMethodID(
            GetJclass(), "getRows",
              "()Ljava/util/Vector;" ) )
        ( "readDataFile",
          java::GetMethodID(
              GetJclass(), "readDataFile",
              "(Ljava/lang/String;)I" ) )
        ( "writeDataFile",
          java::GetMethodID(
            GetJclass(), "writeDataFile",
              "(Ljava/lang/String;)I" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& ManageData::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "CHECK_PASSED",
          java::GetStaticFieldID(
              GetJclass(), "CHECK_PASSED",
              "I" ) )
        ( "CHECK_FAILED",
          java::GetStaticFieldID(
              GetJclass(), "CHECK_FAILED",
              "I" ) )
        ( "kSuccess",
          java::GetStaticFieldID(
              GetJclass(), "kSuccess",
              "I" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
