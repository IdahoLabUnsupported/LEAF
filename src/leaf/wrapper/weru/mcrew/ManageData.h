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
#include <leaf/wrapper/java/lang/Object.h>
#include <leaf/wrapper/java/lang/String.h>

#include <leaf/wrapper/java/util/Vector.h>

#include <leaf/wrapper/weru/mcrew/RowInfo.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

class ManageData;
typedef boost::shared_ptr< ManageData > ManageDataPtr;

class ManageData : public java::lang::Object
{
public:
    ///
    ManageData();

    ///
    ManageData(
        jobject const& o );

    ///
    virtual ~ManageData();

    ///
    static int const& CHECK_PASSED();

    ///
    static int const& CHECK_FAILED();

    ///
    static int const& kSuccess();

    ///
    bool CheckAllConditions(
        java::util::Vector< java::lang::String > const& msgs ) const;

    ///
    int CheckRotationYears() const;

    ///
    int GetRotationYears() const;

    ///
    java::util::Vector< RowInfo >::SPtr GetRows() const;

    ///
    bool ReadDataFile(
        java::lang::String const& pFileName ) const;

    ///
    bool WriteDataFile(
        java::lang::String const& pFileName ) const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

};

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
