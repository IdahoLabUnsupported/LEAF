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

class SciEnergyResultSet;
typedef boost::shared_ptr< SciEnergyResultSet > SciEnergyResultSetPtr;

class SciEnergyResultSet : public WepsResultSet
{
public:
    ///
    SciEnergyResultSet(
        jobject const& o );

    ///
    virtual ~SciEnergyResultSet();

    ///
    static void Static();

    ///
    static java::lang::String const& COLUMN_SCI();

    ///
    static java::lang::String const& COLUMN_OMFACTOR();

    ///
    static java::lang::String const& COLUMN_ERFACTOR();

    ///
    static java::lang::String const& COLUMN_FOFACTOR();

    ///
    static java::lang::String const& COLUMN_WINDEROS();

    ///
    static java::lang::String const& COLUMN_WATEREROS();

    ///
    static java::lang::String const& NAME();

    ///
    static jclass const& GetJclass();

protected:
    ///
    SciEnergyResultSet(
        bool const& isStatic = false );

private:
    ///
    static SciEnergyResultSet& Instance(
        bool const& erase = false );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    java::lang::StringPtr m_COLUMN_SCI;

    ///
    java::lang::StringPtr m_COLUMN_OMFACTOR;

    ///
    java::lang::StringPtr m_COLUMN_ERFACTOR;

    ///
    java::lang::StringPtr m_COLUMN_FOFACTOR;

    ///
    java::lang::StringPtr m_COLUMN_WINDEROS;

    ///
    java::lang::StringPtr m_COLUMN_WATEREROS;

    ///
    java::lang::StringPtr m_NAME;

};

} //end query
} //end reports
} //end weps
} //end weru
} //end wrapper
} //end leaf
