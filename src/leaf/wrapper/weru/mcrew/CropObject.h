// Copyright 2016 Battelle Energy Alliance, LLC
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
#include <leaf/wrapper/weru/mcrew/DataObject.h>
#include <leaf/wrapper/java/lang/String.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

class CropObject;
typedef boost::shared_ptr< CropObject > CropObjectPtr;

class CropObject : public DataObject
{
public:
    ///
    CropObject(
        jobject const& o );

    ///
    virtual ~CropObject();

    ///
    java::lang::StringPtr GetCropName() const;

    ///
    java::lang::StringPtr GetValue(
        java::lang::String const& pColName ) const;

    ///
    java::lang::StringPtr GetValue(
        java::lang::String const& pColName,
        int const& pUnit ) const;

    ///
    void SetValue(
        java::lang::String const& pParamName,
        java::lang::String const& pNewValue,
        int const& pUnit ) const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
