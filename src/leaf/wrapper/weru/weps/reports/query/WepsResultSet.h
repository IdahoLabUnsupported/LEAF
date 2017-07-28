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


#pragma once

// --- LEAF Includes --- //
#include <leaf/wrapper/java/sql/ResultSet.h>

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

class WepsResultSet;
typedef boost::shared_ptr< WepsResultSet > WepsResultSetPtr;

class WepsResultSet : public java::sql::ResultSet
{
public:
    ///
    WepsResultSet(
        jobject const& o );

    ///
    virtual ~WepsResultSet();

    ///
    static jclass const& GetJclass();

protected:

private:

};

} //end query
} //end reports
} //end weps
} //end weru

namespace java
{
namespace lang
{

///Specialization for WepsResultSet inheritance
template<>
weru::weps::reports::query::WepsResultSetPtr
Object::Create< weru::weps::reports::query::WepsResultSet >(
    jobject const& o );

} //end lang
} //end java

} //end wrapper
} //end leaf
