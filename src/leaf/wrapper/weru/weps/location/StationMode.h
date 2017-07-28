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
#include <leaf/wrapper/java/lang/Enum.h>
#include <leaf/wrapper/java/lang/Object.h>

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

class StationMode;
typedef boost::shared_ptr< StationMode > StationModePtr;

class StationMode : public java::lang::Object
{
public:
    ///
    StationMode(
        jobject const& o );

    ///
    virtual ~StationMode();

    ///
    static void Static();

    ///
    static java::lang::Enum const& File();

    ///
    static java::lang::Enum const& Interpolated();

    ///
    static java::lang::Enum const& Nearest();

    ///
    static java::lang::Enum const& NRCS();

    ///
    static jclass const& GetJclass();

protected:
    ///
    StationMode(
        bool const& isStatic = false );

private:
    ///
    static StationMode& Instance(
        bool const& erase = false );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    java::lang::EnumPtr m_File;

    ///
    java::lang::EnumPtr m_Interpolated;

    ///
    java::lang::EnumPtr m_Nearest;

    ///
    java::lang::EnumPtr m_NRCS;

};

} //end location
} //end weps
} //end weru
} //end wrapper
} //end leaf
