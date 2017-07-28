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
#include <leaf/wrapper/weru/misc/File.h>

#include <leaf/wrapper/weru/weps/location/Station.h>

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

class FileStation;
typedef boost::shared_ptr< FileStation > FileStationPtr;

class FileStation : public Station
{
public:
    ///
    FileStation(
        jobject const& o );

    ///
    FileStation(
        misc::File const& file );

    ///
    virtual ~FileStation();

    ///
    virtual Station::Type GetType() const;

    ///
    misc::FilePtr GetFile() const;

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
} //end wrapper
} //end leaf
