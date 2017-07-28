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
#include <leaf/wrapper/java/io/PrintWriter.h>

#include <leaf/wrapper/java/lang/Object.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace util
{
namespace windgen
{

class MakeInterpolatedStation;
typedef boost::shared_ptr< MakeInterpolatedStation > MakeInterpolatedStationPtr;

class MakeInterpolatedStation : public java::lang::Object
{
public:
    ///
    MakeInterpolatedStation();

    ///
    MakeInterpolatedStation(
        jobject const& o );

    ///
    virtual ~MakeInterpolatedStation();

    ///
    static int const& INTERPOLATE1_EXE();

    ///
    static int const& INTERPOLATE2_EXE();

    ///
    static int const& IDX_FILE();

    ///
    static int const& POL_FILE();

    ///
    static int const& LAT();

    ///
    static int const& LON();

    ///
    static int const& WDB();

    ///
    static int const& WINDGEN_EXE();

    ///
    static int const& WINFILE_NAME();

    ///
    static int const& SIM_START();

    ///
    static int const& SIM_LENGTH();

    ///
    static int const& WEIGHTSFILE_NAME();

    ///
    void SetParameter(
        unsigned int const& cdx,
        java::lang::String const& val ) const;

    ///
    void SetOut(
        java::io::PrintWriter const& out,
        java::io::PrintWriter const& err ) const;

    ///
    void Run() const;

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

} //end windgen
} //end util
} //end weru
} //end wrapper
} //end leaf
