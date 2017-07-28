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
#include <leaf/wrapper/weru/weps/reports/query/WepsConnection.h>
#include <leaf/wrapper/weru/weps/reports/query/OutputResultSet.h>
#include <leaf/wrapper/weru/weps/reports/query/SciEnergyResultSet.h>

#include <leaf/wrapper/weru/util/ConfigData.h>

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
WepsConnection::WepsConnection(
    jobject const& o )
    :
    java::lang::Object( o )
{
    //Just to be safe
    SetUnits();
}
////////////////////////////////////////////////////////////////////////////////
WepsConnection::~WepsConnection()
{
    Close();
}
////////////////////////////////////////////////////////////////////////////////
void WepsConnection::Close() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "close" ) );
}
////////////////////////////////////////////////////////////////////////////////
void WepsConnection::SetUnits() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "setUnits" ),
        util::ConfigData::GetData( util::ConfigData::Units() )->Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
java::sql::ResultSetPtr WepsConnection::SqlFunctionWeps(
    WepsConnection const& con,
    java::lang::String const& data )
{
    return Create< java::sql::ResultSet >( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "sqlFunctionWeps" ), con.Jobj(), data.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& WepsConnection::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/reports/query/WepsConnection" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& WepsConnection::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "close",
          java::GetMethodID(
            GetJclass(), "close",
            "()V" ) )
        ( "setUnits",
          java::GetMethodID(
            GetJclass(), "setUnits",
            "(Ljava/lang/String;)V" ) )
        ( "sqlFunctionWeps",
          java::GetStaticMethodID(
              GetJclass(), "sqlFunctionWeps",
              "(Ljava/sql/Connection;Ljava/lang/String;)Ljava/sql/ResultSet;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end query
} //end reports
} //end weps
} //end weru
} //end wrapper
} //end leaf
