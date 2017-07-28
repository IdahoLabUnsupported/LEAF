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

#include <leaf/wrapper/weru/soil/NASIS.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace soil
{

class IFC;
typedef boost::shared_ptr< IFC > IFCPtr;

class IFC : public java::lang::Object
{
public:
    ///
    IFC(
        jobject const& o );

    ///
    IFC(
        NASIS const& nasis );

    ///
    virtual ~IFC();

    ///
    void WriteNewIfc(
        java::lang::String const& filePath ) const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end soil
} //end weru
} //end wrapper
} //end leaf
