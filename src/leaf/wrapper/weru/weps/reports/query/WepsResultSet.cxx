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
#include <leaf/wrapper/weru/weps/reports/query/WepsResultSet.h>
#include <leaf/wrapper/weru/weps/reports/query/OutputResultSet.h>
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
WepsResultSet::WepsResultSet(
    jobject const& o )
    :
    java::sql::ResultSet( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
WepsResultSet::~WepsResultSet()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& WepsResultSet::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/reports/query/WepsResultSet" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////

} //end query
} //end reports
} //end weps
} //end weru

namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
template<>
weru::weps::reports::query::WepsResultSetPtr
Object::Create< weru::weps::reports::query::WepsResultSet >(
    jobject const& o )
{
    using namespace weru::weps::reports::query;
    if( IsInstanceOf( o, OutputResultSet::GetJclass() ) )
    {
        return Object::Create< OutputResultSet >( o );
    }
    else if( IsInstanceOf( o, SciEnergyResultSet::GetJclass() ) )
    {
        return Object::Create< SciEnergyResultSet >( o );
    }
    else
    {
        return WepsResultSetPtr();
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java

} //end wrapper
} //end leaf

