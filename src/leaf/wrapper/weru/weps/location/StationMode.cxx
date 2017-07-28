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
#include <leaf/wrapper/weru/weps/location/StationMode.h>

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

////////////////////////////////////////////////////////////////////////////////
StationMode::StationMode(
    bool const& isStatic )
    :
    java::lang::Object( NULL )
{
    if( !isStatic ) return;

    m_File = Create< java::lang::Enum >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "File" ) ) );
    m_Interpolated = Create< java::lang::Enum >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "Interpolated" ) ) );
    m_Nearest = Create< java::lang::Enum >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "Nearest" ) ) );
    m_NRCS = Create< java::lang::Enum >(
        java::GetStaticField< jobject >(
            GetJclass(), GetJfid( "NRCS" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
StationMode::StationMode(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
StationMode::~StationMode()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
StationMode& StationMode::Instance(
    bool const& erase )
{
    static StationMode* s_stationMode = new StationMode( true );
    if( erase )
    {
        delete s_stationMode;
        s_stationMode = NULL;
    }

    return *s_stationMode;
}
////////////////////////////////////////////////////////////////////////////////
void StationMode::Static()
{
    Instance( true );
}
////////////////////////////////////////////////////////////////////////////////
java::lang::Enum const& StationMode::File()
{
    return *Instance().m_File;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::Enum const& StationMode::Interpolated()
{
    return *Instance().m_Interpolated;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::Enum const& StationMode::Nearest()
{
    return *Instance().m_Nearest;
}
////////////////////////////////////////////////////////////////////////////////
java::lang::Enum const& StationMode::NRCS()
{
    return *Instance().m_NRCS;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& StationMode::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/location/StationMode" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& StationMode::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "File",
          java::GetStaticFieldID(
              GetJclass(), "File",
              "Lusda/weru/weps/location/StationMode;" ) )
        ( "Interpolated",
          java::GetStaticFieldID(
              GetJclass(), "Interpolated",
              "Lusda/weru/weps/location/StationMode;" ) )
        ( "Nearest",
          java::GetStaticFieldID(
              GetJclass(), "Nearest",
              "Lusda/weru/weps/location/StationMode;" ) )
        ( "NRCS",
          java::GetStaticFieldID(
              GetJclass(), "NRCS",
              "Lusda/weru/weps/location/StationMode;" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end location
} //end weps
} //end weru
} //end wrapper
} //end leaf
