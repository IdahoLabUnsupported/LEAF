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

#include <leaf/wrapper/weru/misc/LatLong.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{
namespace location
{

class Station;
typedef boost::shared_ptr< Station > StationPtr;

class Station : public java::lang::Object
{
public:
    ///
    enum Type
    {
        NONE = 0,
        CLIGEN = 1,
        FILE = 2,
        INTERPOLATED = 3,
        WINDGEN = 4
    };

    ///
    Station(
        jobject const& o );

    ///
    virtual ~Station();

    ///
    virtual Type GetType() const = 0;

    ///
    misc::LatLongPtr GetLatLong() const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end location
} //end weps
} //end weru

namespace java
{
namespace lang
{

///Specialization for Station inheritance
template<>
weru::weps::location::StationPtr
Object::Create< weru::weps::location::Station >(
    jobject const& o );

} //end lang
} //end java

} //end wrapper
} //end leaf
