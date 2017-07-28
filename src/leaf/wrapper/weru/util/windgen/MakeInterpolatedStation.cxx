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
#include <leaf/wrapper/weru/util/windgen/MakeInterpolatedStation.h>

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

////////////////////////////////////////////////////////////////////////////////
MakeInterpolatedStation::MakeInterpolatedStation(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
MakeInterpolatedStation::MakeInterpolatedStation()
    :
    java::lang::Object(
        java::NewObject( GetJclass(), GetJmid( "<init>" ), NULL ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
MakeInterpolatedStation::~MakeInterpolatedStation()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::INTERPOLATE1_EXE()
{
    static int INTERPOLATE1_EXE = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "INTERPOLATE1_EXE" ) );
    return INTERPOLATE1_EXE;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::INTERPOLATE2_EXE()
{
    static int INTERPOLATE2_EXE = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "INTERPOLATE2_EXE" ) );
    return INTERPOLATE2_EXE;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::IDX_FILE()
{
    static int IDX_FILE = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "IDX_FILE" ) );
    return IDX_FILE;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::POL_FILE()
{
    static int POL_FILE = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "POL_FILE" ) );
    return POL_FILE;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::LAT()
{
    static int LAT = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "LAT" ) );
    return LAT;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::LON()
{
    static int LON = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "LON" ) );
    return LON;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::WDB()
{
    static int WDB = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "WDB" ) );
    return WDB;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::WINDGEN_EXE()
{
    static int WINDGEN_EXE = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "WINDGEN_EXE" ) );
    return WINDGEN_EXE;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::WINFILE_NAME()
{
    static int WINFILE_NAME = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "WINFILE_NAME" ) );
    return WINFILE_NAME;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::SIM_START()
{
    static int SIM_START = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "SIM_START" ) );
    return SIM_START;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::SIM_LENGTH()
{
    static int SIM_LENGTH = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "SIM_LENGTH" ) );
    return SIM_LENGTH;
}
////////////////////////////////////////////////////////////////////////////////
int const& MakeInterpolatedStation::WEIGHTSFILE_NAME()
{
    static int WEIGHTSFILE_NAME = java::GetStaticField< jint >(
        GetJclass(), GetJfid( "WEIGHTSFILE_NAME" ) );
    return WEIGHTSFILE_NAME;
}
////////////////////////////////////////////////////////////////////////////////
void MakeInterpolatedStation::SetParameter(
    unsigned int const& cdx,
    java::lang::String const& val ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setParameter" ), cdx, val.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void MakeInterpolatedStation::SetOut(
    java::io::PrintWriter const& out,
    java::io::PrintWriter const& err ) const
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setOut" ), out.Jobj(), err.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void MakeInterpolatedStation::Run() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "run" ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& MakeInterpolatedStation::GetJclass()
{
    static jclass const clazz =
        java::FindClass( "usda/weru/util/windgen/MakeInterpolatedStation" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& MakeInterpolatedStation::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "(Ljava/awt/Component;)V" ) )
        ( "setParameter",
          java::GetMethodID(
              GetJclass(), "setParameter",
              "(ILjava/lang/String;)V" ) )
        ( "setOut",
          java::GetMethodID(
              GetJclass(), "setOut",
              "(Ljava/io/PrintWriter;Ljava/io/PrintWriter;)V" ) )
        ( "run",
          java::GetMethodID(
              GetJclass(), "run",
              "()V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& MakeInterpolatedStation::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "INTERPOLATE1_EXE",
          java::GetStaticFieldID(
              GetJclass(), "INTERPOLATE1_EXE",
              "I" ) )
        ( "INTERPOLATE2_EXE",
          java::GetStaticFieldID(
              GetJclass(), "INTERPOLATE2_EXE",
              "I" ) )
        ( "IDX_FILE",
          java::GetStaticFieldID(
              GetJclass(), "IDX_FILE",
              "I" ) )
        ( "POL_FILE",
          java::GetStaticFieldID(
              GetJclass(), "POL_FILE",
              "I" ) )
        ( "LAT",
          java::GetStaticFieldID(
              GetJclass(), "LAT",
              "I" ) )
        ( "LON",
          java::GetStaticFieldID(
              GetJclass(), "LON",
              "I" ) )
        ( "WDB",
          java::GetStaticFieldID(
              GetJclass(), "WDB",
              "I" ) )
        ( "WINDGEN_EXE",
          java::GetStaticFieldID(
              GetJclass(), "WINDGEN_EXE",
              "I" ) )
        ( "WINFILE_NAME",
          java::GetStaticFieldID(
              GetJclass(), "WINFILE_NAME",
              "I" ) )
        ( "SIM_START",
          java::GetStaticFieldID(
              GetJclass(), "SIM_START",
              "I" ) )
        ( "SIM_LENGTH",
          java::GetStaticFieldID(
              GetJclass(), "SIM_LENGTH",
              "I" ) )
        ( "WEIGHTSFILE_NAME",
          java::GetStaticFieldID(
              GetJclass(), "WEIGHTSFILE_NAME",
              "I" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end windgen
} //end util
} //end weru
} //end wrapper
} //end leaf
