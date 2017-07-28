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
#include <leaf/util/RotationHelper.h>
#include <leaf/util/SoilHelper.h>

#include <leaf/open/soi/Component.h>
#include <leaf/open/ero/Erosion.h>

#include <leaf/wrapper/weru/Common.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{

///
LEAF_WEPS_EXPORTS
void Init(
    std::string const& runDir = "weps.wjr" );

///
LEAF_WEPS_EXPORTS
void UseCycleCount(
    unsigned int const& cycleCount );

///
LEAF_WEPS_EXPORTS
void SetLocation(
    std::string const& fips );

///
LEAF_WEPS_EXPORTS
void SetLocation(
    double const& latitude,
    double const& longitude );

///
LEAF_WEPS_EXPORTS
util::SoilError CreateSoilFile(
    open::soi::ComponentPtr const& component,
    std::string const& fileName,
    bool const& overwrite = false );

///
LEAF_WEPS_EXPORTS
bool CreateManFile(
    std::string const& fileName,
    bool const& overwrite = false );

///This function checks management years
LEAF_WEPS_EXPORTS
bool CheckDates();

///This function checks that a management is correct
LEAF_WEPS_EXPORTS
bool CheckManagement(
    std::vector< std::string >* msgs = NULL );

///
LEAF_WEPS_EXPORTS
std::string GetRotationString(
    std::string const& delimiter = "," );

///
LEAF_WEPS_EXPORTS
std::vector< std::string > GetRotation();

///
LEAF_WEPS_EXPORTS
std::string GetRunDir();

///
LEAF_WEPS_EXPORTS
void SetMaxRotYears(
    unsigned int const& maxRotYears );

///
LEAF_WEPS_EXPORTS
bool SetRfdFilePtr(
    Rfd::File const& rfdFile,
    std::string const& fileName );

///
LEAF_WEPS_EXPORTS
bool AssignSlopeVals(
    Poco::Dynamic::Var const& slope,
    Poco::Dynamic::Var const& slopeLen );

///
LEAF_WEPS_EXPORTS
std::vector< unsigned int > GetCropIndices(
    bool const& onlyCal = false );

///
LEAF_WEPS_EXPORTS
bool AssignYields(
    util::CROP_MAP const& yields,
    bool const& setCvrCrpYlds = false );

///
LEAF_WEPS_EXPORTS
bool AssignYields(
    util::CROPYEAR_MAP const& yields,
    bool const& setCvrCrpYlds = false );

///
LEAF_WEPS_EXPORTS
bool AssignYields(
    util::CROPVAR_VEC& yields,
    unsigned int const& offset = 0,
    bool const& setCvrCrpYlds = false );

///
#ifndef SWIG
bool SaveCalibrations(
    open::scn::ScenarioPtr const& scenario );
#endif //SWIG

///
LEAF_WEPS_EXPORTS
bool SetWaterErosionLoss(
    Poco::Dynamic::Var const& waterEros );

///
LEAF_WEPS_EXPORTS
open::ero::ErosionPtr CallWeps(
    open::scn::ScenarioPtr const& scenario,
    unsigned int const& numCropCalItrs = 0,
    unsigned int const& maxCalCycles = 0,
    unsigned int const& reportNames = 0 );

///
LEAF_WEPS_EXPORTS
void Exit();

} //end weru
} //end wrapper
} //end leaf
