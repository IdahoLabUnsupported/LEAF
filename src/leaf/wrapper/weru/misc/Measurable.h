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

#include <leaf/wrapper/weru/misc/Unit.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace misc
{

class Measurable;
typedef boost::shared_ptr< Measurable > MeasurablePtr;

class Measurable : public java::lang::Object
{
public:
    ///
    Measurable(
        jobject const& o );

    ///
    virtual ~Measurable();

    ///
    double DoubleValue(
        Unit const& unit ) const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end misc
} //end weru
} //end wrapper
} //end leaf
