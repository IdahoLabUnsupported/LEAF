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

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace util
{

///STATIC INTERFACE
class About : public boost::noncopyable
{
public:
    ///
    static void SetUserWepsRoot(
        misc::File const& file );

    ///
    static jclass const& GetJclass();

protected:
    ///
    ~About();

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end util
} //end weru
} //end wrapper
} //end leaf
