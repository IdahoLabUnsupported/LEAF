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
#include <leaf/open/scn/Scenario.h>

#include <leaf/wrapper/java/lang/Object.h>
#include <leaf/wrapper/java/lang/String.h>

#include <leaf/wrapper/weru/misc/File.h>

#include <leaf/wrapper/weru/weps/reports/ReportPack.h>
#include <leaf/wrapper/weru/weps/reports/query/WepsConnection.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{
namespace reports
{

///
enum REPORT_NAMES
{
    NONE = 0x000,
    DETAIL = 0x001,
    SOIL = 0x002,
    DEBUG = 0x004,
    RUN = 0x008,
    MANAGEMENT = 0x010,
    CROP = 0x020,
    STIR = 0x040,
    QUICKPLOT = 0x080,
    DATABASE_CROPS = 0x100,
    DATABASE_OPERATIONS = 0x200,
    DATABASE_MANAGEMENTS = 0x400,
    DEFAULT = RUN | MANAGEMENT | CROP | STIR,
    ALL = DETAIL | SOIL | DEBUG | DEFAULT | QUICKPLOT |
        DATABASE_CROPS | DATABASE_OPERATIONS | DATABASE_MANAGEMENTS
};

///STATIC INTERFACE
class ReportManager : public java::lang::Object
{
public:
    ///
    static void Static();

    ///
    static java::lang::String const& REPORT_DETAIL();

    ///
    static java::lang::String const& REPORT_SOIL();

    ///
    static java::lang::String const& REPORT_DEBUG();

    ///
    static java::lang::String const& REPORT_RUN();

    ///
    static java::lang::String const& REPORT_MANAGEMENT();

    ///
    static java::lang::String const& REPORT_CROP();

    ///
    static java::lang::String const& REPORT_STIR();

    ///
    static java::lang::String const& REPORT_QUICKPLOT();

    ///
    static java::lang::String const& REPORT_DATABASE_CROPS();

    ///
    static java::lang::String const& REPORT_DATABASE_OPERATIONS();

    ///
    static java::lang::String const& REPORT_DATABASE_MANAGEMENTS();

    ///
    static query::WepsConnectionPtr GetConnection(
        misc::File const& file );

    ///
    static void GenerateReportPDFs(
        open::scn::ScenarioPtr const& scenario,
        boost::filesystem::path const& repDir,
        boost::filesystem::path const& runDir,
        unsigned int const& reportNames );

    ///
    static jclass const& GetJclass();

protected:
    ///
    ReportManager();

    ///
    virtual ~ReportManager();

private:
    ///
    static ReportManager& Instance(
        bool const& erase = false );

    ///
    typedef std::map< std::string, ReportPackPtr > REPORTS;
    static std::pair< std::string, ReportPackPtr >
    GetReportPack(
        misc::File const& file,
        java::lang::String const& reportName );

    ///
    static jmethodID const& GetJmid(
        std::string const& name );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    java::lang::String const m_REPORT_DETAIL;

    ///
    java::lang::String const m_REPORT_SOIL;

    ///
    java::lang::String const m_REPORT_DEBUG;

    ///
    java::lang::String const m_REPORT_RUN;

    ///
    java::lang::String const m_REPORT_MANAGEMENT;

    ///
    java::lang::String const m_REPORT_CROP;

    ///
    java::lang::String const m_REPORT_STIR;

    ///
    java::lang::String const m_REPORT_QUICKPLOT;

    ///
    java::lang::String const m_REPORT_DATABASE_CROPS;

    ///
    java::lang::String const m_REPORT_DATABASE_OPERATIONS;

    ///
    java::lang::String const m_REPORT_DATABASE_MANAGEMENTS;

};

} //end reports
} //end weps
} //end weru
} //end wrapper
} //end leaf
