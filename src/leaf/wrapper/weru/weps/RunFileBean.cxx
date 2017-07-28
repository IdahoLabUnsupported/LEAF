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


// --- LEAF Includes --- //
#include <leaf/wrapper/weru/weps/RunFileBean.h>

#include <leaf/wrapper/java/lang/Thread.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{

////////////////////////////////////////////////////////////////////////////////
RunFileBean_PropertyBoilerPlate::RunFileBean_PropertyBoilerPlate(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
RunFileBean_PropertyBoilerPlate::~RunFileBean_PropertyBoilerPlate()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
location::StationPtr RunFileBean_PropertyBoilerPlate::GetCligenStation() const
{
    return Create< location::Station >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getCligenStation" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
location::StationModePtr
RunFileBean_PropertyBoilerPlate::GetCligenStationMode() const
{
    return Create< location::StationMode >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getCligenStationMode" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
location::StationPtr RunFileBean_PropertyBoilerPlate::GetWindgenStation() const
{
    return Create< location::Station >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getWindgenStation" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
location::StationModePtr
RunFileBean_PropertyBoilerPlate::GetWindgenStationMode() const
{
    return Create< location::StationMode >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getWindgenStationMode" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
misc::LatLongPtr RunFileBean_PropertyBoilerPlate::GetLatLong() const
{
    return Create< misc::LatLong >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getLatLong" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
void RunFileBean_PropertyBoilerPlate::SetSite(
    location::Site const& site ) const
{
    java::CallMethod< void >( m_jobject, GetJmid( "setSite" ), site.Jobj() );
    java::lang::Thread::WaitForEDT();
}
////////////////////////////////////////////////////////////////////////////////
void RunFileBean_PropertyBoilerPlate::SetCligenStationMode(
    location::StationMode const& stationMode ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setCligenStationMode" ), stationMode.Jobj() );
    java::lang::Thread::WaitForEDT();
}
////////////////////////////////////////////////////////////////////////////////
void RunFileBean_PropertyBoilerPlate::SetCligenStation(
    location::Station const& station ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setCligenStation" ), station.Jobj() );
    java::lang::Thread::WaitForEDT();
}
////////////////////////////////////////////////////////////////////////////////
void RunFileBean_PropertyBoilerPlate::SetWindgenStationMode(
    location::StationMode const& stationMode ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setWindgenStationMode" ), stationMode.Jobj() );
    java::lang::Thread::WaitForEDT();
}
////////////////////////////////////////////////////////////////////////////////
void RunFileBean_PropertyBoilerPlate::SetWindgenStation(
    location::Station const& station ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setWindgenStation" ), station.Jobj() );
    java::lang::Thread::WaitForEDT();
}
////////////////////////////////////////////////////////////////////////////////
void RunFileBean_PropertyBoilerPlate::SetLatLong(
    misc::LatLong const& latLong ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setLatLong" ), latLong.Jobj() );
    java::lang::Thread::WaitForEDT();
}
////////////////////////////////////////////////////////////////////////////////
jclass const& RunFileBean_PropertyBoilerPlate::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/RunFileBean_PropertyBoilerPlate" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& RunFileBean_PropertyBoilerPlate::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getCligenStation",
          java::GetMethodID(
              GetJclass(), "getCligenStation",
              "()Lusda/weru/weps/location/Station;" ) )
        ( "getCligenStationMode",
          java::GetMethodID(
              GetJclass(), "getCligenStationMode",
              "()Lusda/weru/weps/location/StationMode;" ) )
        ( "getWindgenStation",
          java::GetMethodID(
              GetJclass(), "getWindgenStation",
              "()Lusda/weru/weps/location/Station;" ) )
        ( "getWindgenStationMode",
          java::GetMethodID(
              GetJclass(), "getWindgenStationMode",
              "()Lusda/weru/weps/location/StationMode;" ) )
        ( "getLatLong",
          java::GetMethodID(
              GetJclass(), "getLatLong",
              "()Lorg/jscience/geography/coordinates/LatLong;" ) )
        ( "setSite",
          java::GetMethodID(
              GetJclass(), "setSite",
              "(Lusda/weru/weps/location/Site;)V" ) )
        ( "setCligenStationMode",
          java::GetMethodID(
              GetJclass(), "setCligenStationMode",
              "(Lusda/weru/weps/location/StationMode;)V" ) )
        ( "setCligenStation",
          java::GetMethodID(
              GetJclass(), "setCligenStation",
              "(Lusda/weru/weps/location/Station;)V" ) )
        ( "setWindgenStationMode",
          java::GetMethodID(
              GetJclass(), "setWindgenStationMode",
              "(Lusda/weru/weps/location/StationMode;)V" ) )
        ( "setWindgenStation",
          java::GetMethodID(
              GetJclass(), "setWindgenStation",
              "(Lusda/weru/weps/location/Station;)V" ) )
        ( "setLatLong",
          java::GetMethodID(
              GetJclass(), "setLatLong",
              "(Lorg/jscience/geography/coordinates/LatLong;)V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
RunFileBean::RunFileBean(
    jobject const& o )
    :
    RunFileBean_PropertyBoilerPlate( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
RunFileBean::~RunFileBean()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void RunFileBean::Clear() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "clear" ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& RunFileBean::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/weps/RunFileBean" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& RunFileBean::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "clear",
          java::GetMethodID(
            GetJclass(), "clear",
            "()V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end weps
} //end weru
} //end wrapper
} //end leaf
