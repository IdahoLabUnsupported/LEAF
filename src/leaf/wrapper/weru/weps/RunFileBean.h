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

#include <leaf/wrapper/weru/weps/location/Site.h>
#include <leaf/wrapper/weru/weps/location/StationMode.h>
#include <leaf/wrapper/weru/weps/location/Station.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{

class RunFileBean_PropertyBoilerPlate : public java::lang::Object
{
public:
    ///
    RunFileBean_PropertyBoilerPlate(
        jobject const& o );

    ///
    virtual ~RunFileBean_PropertyBoilerPlate();

    ///
    location::StationPtr GetCligenStation() const;

    ///
    location::StationModePtr GetCligenStationMode() const;

    ///
    location::StationPtr GetWindgenStation() const;

    ///
    location::StationModePtr GetWindgenStationMode() const;

    ///
    misc::LatLongPtr GetLatLong() const;

    ///
    void SetSite(
        location::Site const& site ) const;

    ///
    void SetCligenStationMode(
        location::StationMode const& stationMode ) const;

    ///
    void SetCligenStation(
        location::Station const& station ) const;

    ///
    void SetWindgenStationMode(
        location::StationMode const& stationMode ) const;

    ///
    void SetWindgenStation(
        location::Station const& station ) const;

    ///
    void SetLatLong(
        misc::LatLong const& latLong ) const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

class RunFileBean;
typedef boost::shared_ptr< RunFileBean > RunFileBeanPtr;

class RunFileBean : public RunFileBean_PropertyBoilerPlate
{
public:
    ///
    RunFileBean(
        jobject const& o );

    ///
    virtual ~RunFileBean();

    ///
    void Clear() const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end weps
} //end weru
} //end wrapper
} //end leaf
