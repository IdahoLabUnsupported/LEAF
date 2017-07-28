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
#include <leaf/wrapper/weru/weps/location/FileStation.h>

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
FileStation::FileStation(
    jobject const& o )
    :
    Station( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
FileStation::FileStation(
    misc::File const& file )
    :
    Station( java::NewObject( GetJclass(), GetJmid( "<init>" ), file.Jobj() ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
FileStation::~FileStation()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Station::Type FileStation::GetType() const
{
    return Station::FILE;
}
////////////////////////////////////////////////////////////////////////////////
misc::FilePtr FileStation::GetFile() const
{
    return Create< misc::File >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getFile" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& FileStation::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/weps/location/FileStation" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& FileStation::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "(Lde/schlichtherle/io/File;)V" ) )
        ( "getFile",
          java::GetMethodID(
              GetJclass(), "getFile",
              "()Lde/schlichtherle/io/File;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end location
} //end weps
} //end weru
} //end wrapper
} //end leaf
