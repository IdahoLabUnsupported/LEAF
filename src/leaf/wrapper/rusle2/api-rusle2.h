/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2013 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/


#pragma once

// --- LEAF Includes --- //
#include <leaf/util/RotationHelper.h>
#include <leaf/util/SoilHelper.h>

#include <leaf/open/ero/Erosion.h>

#include <leaf/open/man/Management.h>

#include <leaf/open/soi/Component.h>

#include <leaf/wrapper/rusle2/GdbHelper.h>

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

///
LEAF_RUSLE2_EXPORTS
void Init(
    std::string const& dbName,
    std::string const& runDir = "" );

///
LEAF_RUSLE2_EXPORTS
util::SoilError CreateSoilFile(
    open::soi::ComponentPtr const& component,
    std::string const& fileName,
    bool const& overwrite = false );

///
LEAF_RUSLE2_EXPORTS
void CreateManFile(
    open::man::ManagementPtr const& management,
    std::string const& fileName,
    bool const& overwrite = false );

///This function corrects dates so that they start from year one
LEAF_RUSLE2_EXPORTS
bool CheckDates();

///
LEAF_RUSLE2_EXPORTS
std::string GetRotationString(
    std::string const& delimiter = "," );

///
LEAF_RUSLE2_EXPORTS
std::vector< std::string > GetRotation();

///
LEAF_RUSLE2_EXPORTS
std::string GetRunDir();

///
LEAF_RUSLE2_EXPORTS
bool SetObjPtr(
    ObjectType const& objType,
    std::string const& fileName,
    FilePrefix const& filePrefix = HASHTAG_DB );

///
LEAF_RUSLE2_EXPORTS
bool ResetObjPtr(
    ObjectType const& objType );

///
LEAF_RUSLE2_EXPORTS
bool SaveObjPtr(
    ObjectType const& objType,
    std::string const& fileName,
    FilePrefix const& filePrefix = HASHTAG_DB );

///
LEAF_RUSLE2_EXPORTS
std::string GetAttrVal(
    ObjectType const& objType,
    std::string const& attrName,
    unsigned int const& index = 0 );

///
LEAF_RUSLE2_EXPORTS
unsigned int GetAttrSize(
    ObjectType const& objType,
    std::string const& attrName );

///
LEAF_RUSLE2_EXPORTS
bool SetAttrVal(
    ObjectType const& objType,
    std::string const& attrName,
    std::string const& attrVal,
    unsigned int const& index = 0 );

///
LEAF_RUSLE2_EXPORTS
bool AssignSlopeVals(
    Poco::Dynamic::Var const& slope,
    Poco::Dynamic::Var const& slopeLen );

///
LEAF_RUSLE2_EXPORTS
std::vector< unsigned int > GetCropIndices();

///
LEAF_RUSLE2_EXPORTS
bool AssignYields(
    util::CROP_MAP const& yields,
    bool const& setCvrCrpYlds = false );

///
LEAF_RUSLE2_EXPORTS
bool AssignYields(
    util::CROPYEAR_MAP const& yields,
    bool const& setCvrCrpYlds = false );

///
LEAF_RUSLE2_EXPORTS
bool AssignYields(
    util::CROPVAR_VEC& yields,
    unsigned int const& offset = 0,
    bool const& setCvrCrpYlds = false );

///
LEAF_RUSLE2_EXPORTS
bool SetWindErosionLoss(
    Poco::Dynamic::Var const& windEros );

///
LEAF_RUSLE2_EXPORTS
void CallRusle2(
    open::ero::ErosionPtr& results,
    bool const& reset = true );

///
LEAF_RUSLE2_EXPORTS
open::ero::ErosionPtr CallRusle2(
    open::scn::ScenarioPtr const& scenario,
    bool const& reset = true );

///
LEAF_RUSLE2_EXPORTS
std::string RevSimVegLu(
    unsigned int segIndex,
    unsigned int numSimDays );

///
LEAF_RUSLE2_EXPORTS
void Exit();

} //end rusle2
} //end wrapper
} //end leaf
