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
#include <leaf/wrapper/java/lang/String.h>

#include <leaf/wrapper/weru/misc/File.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{

namespace mcrew
{
class ConfigData;
}

namespace util
{

///STATIC INTERFACE
class ConfigData : public java::lang::Object
{
public:
    ///
    static void Static();

    ///
    static java::lang::String const& CliData();

    ///
    static java::lang::String const& CliExe();

    ///
    static java::lang::String const& CurrentProj();

    ///
    static java::lang::String const& Cycle();

    ///
    static java::lang::String const& Dates();

    ///
    static java::lang::String const& DetailTableFilterFile();

    ///
    static java::lang::String const& MCREW();

    ///
    static java::lang::String const& McrewDataConfigFile();

    ///
    static java::lang::String const& NRCS();

    ///
    static java::lang::String const& OMFractionThreshold();

    ///
    static java::lang::String const& ProjDir();

    ///
    static java::lang::String const& ReportsCustomized();

    ///
    static java::lang::String const& ReportFileName();

    ///
    static java::lang::String const& SingleProjectMode();

    ///
    static java::lang::String const& Units();

    ///
    static java::lang::String const& WepsExe();

    ///
    static java::lang::String const& WinData();

    ///
    static java::lang::String const& WinExe();

    ///
    static java::lang::String const& WinIndex();

    ///
    static java::lang::String const& WindInterp1EXE();

    ///
    static java::lang::String const& WindInterp2EXE();

    ///
    static void FireAll();

    ///
    static java::lang::StringPtr GetData(
        java::lang::String const& propertyName );

    ///
    static java::lang::StringPtr GetDataParsed(
        java::lang::String const& propertyName );

    ///
    static double GetOmFractionThreshold();

    ///
    static bool GetSoilTestOrganic();

    ///
    static double GetSoilMaxOrganicDepth();

    ///
    static bool IsAverageStratifiedSoilLayers();

    ///
    static bool IsSkipOrganicSoilSurfaceLayers();

    ///
    static void Load(
        misc::File const& main,
        misc::File const& user );

    ///
    static void SetData(
        java::lang::String const& propertyName,
        java::lang::String const& newData );

    ///
    static jclass const& GetJclass();

protected:
    ///
    ConfigData();

    ///
    virtual ~ConfigData();

private:
    ///
    friend class mcrew::ConfigData;

    ///
    static ConfigData& Instance(
        bool const& erase = false );

    ///
    static jmethodID const& GetJmid(
        std::string const& name );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    java::lang::String const m_CliData;

    ///
    java::lang::String const m_CliExe;

    ///
    java::lang::String const m_CurrentProj;

    ///
    java::lang::String const m_Cycle;

    ///
    java::lang::String const m_Dates;

    ///
    java::lang::String const m_DetailTableFilterFile;

    ///
    java::lang::String const m_MCREW;

    ///
    java::lang::String const m_McrewDataConfigFile;

    ///
    java::lang::String const m_NRCS;

    ///
    java::lang::String const m_OMFractionThreshold;

    ///
    java::lang::String const m_ProjDir;

    ///
    java::lang::String const m_ReportsCustomized;

    ///
    java::lang::String const m_ReportFileName;

    ///
    java::lang::String const m_SingleProjectMode;

    ///
    java::lang::String const m_Units;

    ///
    java::lang::String const m_WepsExe;

    ///
    java::lang::String const m_WinData;

    ///
    java::lang::String const m_WinExe;

    ///
    java::lang::String const m_WinIndex;

    ///
    java::lang::String const m_WindInterp1EXE;

    ///
    java::lang::String const m_WindInterp2EXE;

};

} //end util
} //end weru
} //end wrapper
} //end leaf
