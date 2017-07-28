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
#include <leaf/wrapper/weru/weps/reports/query/WepsResultSet.h>

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

class HarvestsResultSet;
typedef boost::shared_ptr< HarvestsResultSet > HarvestsResultSetPtr;

class HarvestsResultSet : public WepsResultSet
{
public:
    ///
    HarvestsResultSet(
        jobject const& o );

    ///
    virtual ~HarvestsResultSet();

    ///
    static void Static();

    ///
    static java::lang::String const& COLUMN_CROP();

    ///
    static java::lang::String const& COLUMN_CROPINDEX();

    ///
    static java::lang::String const& COLUMN_CYCLENUMBER();

    ///
    static java::lang::String const& COLUMN_DRYYIELD();

    ///
    static java::lang::String const& COLUMN_HARVESTDATE();

    ///
    static java::lang::String const& COLUMN_INDEX();

    ///
    static java::lang::String const& COLUMN_MOISTURE();

    ///
    static java::lang::String const& COLUMN_RESIDUE();

    ///
    static java::lang::String const& COLUMN_RUNID();

    ///
    static java::lang::String const& COLUMN_YIELD();

    ///
    static java::lang::String const& COLUMN_YIELDUNITS();

    ///
    static java::lang::String const& NAME();

    ///
    static jclass const& GetJclass();

protected:
    ///
    HarvestsResultSet(
        bool const& isStatic = false );

private:
    ///
    static HarvestsResultSet& Instance(
        bool const& erase = false );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    java::lang::StringPtr m_COLUMN_CROP;

    ///
    java::lang::StringPtr m_COLUMN_CROPINDEX;

    ///
    java::lang::StringPtr m_COLUMN_CYCLENUMBER;

    ///
    java::lang::StringPtr m_COLUMN_DRYYIELD;

    ///
    java::lang::StringPtr m_COLUMN_HARVESTDATE;

    ///
    java::lang::StringPtr m_COLUMN_INDEX;

    ///
    java::lang::StringPtr m_COLUMN_MOISTURE;

    ///
    java::lang::StringPtr m_COLUMN_RESIDUE;

    ///
    java::lang::StringPtr m_COLUMN_RUNID;

    ///
    java::lang::StringPtr m_COLUMN_YIELD;

    ///
    java::lang::StringPtr m_COLUMN_YIELDUNITS;

    ///
    java::lang::StringPtr m_NAME;

};

} //end query
} //end reports
} //end weps
} //end weru
} //end wrapper
} //end leaf
