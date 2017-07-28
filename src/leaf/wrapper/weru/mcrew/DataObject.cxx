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


// --- LEAF Includes --- //
#include <leaf/wrapper/weru/mcrew/DataObject.h>

#include <leaf/wrapper/weru/mcrew/CropObject.h>
#include <leaf/wrapper/weru/mcrew/OperationObject.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

////////////////////////////////////////////////////////////////////////////////
DataObject::DataObject(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DataObject::~DataObject()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& DataObject::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/mcrew/DataObject" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& DataObject::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap;

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end mcrew
} //end weru

namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
template<>
weru::mcrew::DataObjectPtr
Object::Create< weru::mcrew::DataObject >(
    jobject const& o )
{
    using namespace weru::mcrew;
    if( IsInstanceOf( o, CropObject::GetJclass() ) )
    {
        return Object::Create< CropObject >( o );
    }
    else if( IsInstanceOf( o, OperationObject::GetJclass() ) )
    {
        return Object::Create< OperationObject >( o );
    }
    else
    {
        return DataObjectPtr();
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java

} //end wrapper
} //end leaf

