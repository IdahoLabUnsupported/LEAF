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
#include <leaf/wrapper/weru/Common.h>
#include <leaf/wrapper/weru/Calibration.h>

#include <leaf/wrapper/weru/mcrew/ManageData.h>

#include <leaf/wrapper/weru/weps/RunFileData.h>

#include <leaf/wrapper/weru/weps/location/LocationPanel.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

namespace leaf
{
namespace wrapper
{
namespace weru
{

class WepsWrapper : public boost::noncopyable
{
public:
    ///
    static WepsWrapper& Instance(
        bool const& erase = false );

    ///
    weps::RunFileDataPtr rfd() const;

    ///
    void Initialize(
        boost::filesystem::path const& wepsPath,
        boost::filesystem::path const& runDirPath );

    ///
    CalibrationPtr GetCalibration(
        open::scn::ScenarioPtr const& scenario,
        Poco::Dynamic::Var const& date,
        Poco::Dynamic::Var const& crop,
        bool const& mustExist = true ) const;

    ///
    Calibrations const& GetCalibrations() const;

    ///
    mcrew::ManageDataPtr GetManageData() const;

    ///
    void Reset();

    ///
    void RunWeps(
        open::scn::ScenarioPtr const& scenario,
        unsigned int const& numCropCalItrs = 0,
        unsigned int const& maxCalCycles = 0 );

    ///
    void SetMaxRotYears(
        unsigned int const& maxRotYears );

    ///
    bool SetRfdFileName(
        Rfd::File const& rfdFile,
        std::string const& fileName );

    ///
    bool WriteManFile();

protected:
    ///
    WepsWrapper();

    ///
    ~WepsWrapper();

private:
    ///
    std::string const& GetRfdFileName(
        Rfd::File const& rfdFile ) const;

    ///
    void RunCligen(
        unsigned int const& startYear,
        unsigned int const& totalYears );

    ///
    void RunWindgen(
        unsigned int const& startYear,
        unsigned int const& totalYears );

    ///
    bool ReadCalibFile(
        open::scn::ScenarioPtr const& scenario );

    ///
    void RemoveOldFiles();

    ///
    unsigned int m_maxRotYears;

    ///
    mcrew::ManageDataPtr m_manageData;

    ///
    weps::RunFileDataPtr m_rfd;

    ///
    weps::location::LocationPanelPtr m_locationPanel;

    ///
    mutable Calibrations m_calibs;

    ///
    boost::filesystem::path m_wepsPath;

    ///
    boost::filesystem::path m_runDirPath;

    ///
    typedef std::map< Rfd::File, std::string > FileMap;
    FileMap m_fileMap;

};

} //end weru
} //end wrapper
} //end leaf
