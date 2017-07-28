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
#include <leaf/wrapper/java/lang/Object.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

class DataObject;
typedef boost::shared_ptr< DataObject > DataObjectPtr;

class DataObject : public java::lang::Object
{
public:
    ///
    DataObject(
        jobject const& o );

    ///
    virtual ~DataObject();

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

namespace java
{
namespace lang
{

///Specialization for DataObject inheritance
template<>
weru::mcrew::DataObjectPtr
Object::Create< weru::mcrew::DataObject >(
    jobject const& o );

} //end lang
} //end java

} //end wrapper
} //end leaf
