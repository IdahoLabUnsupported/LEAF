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
#include <leaf/LEAFConfig.h>

#include <leaf/open/soi/Component.h>
#include <leaf/open/scn/Scenario.h>
#include <leaf/open/ero/Erosion.h>

// --- STL Includes --- //
#include <string>
#include <vector>

// --- Boost Includes --- //
//#include <boost/tuple/tuple.h>

namespace leaf
{
namespace wrapper
{
namespace daycent
{

///
typedef boost::tuples::tuple< std::string, std::string, std::string, std::string, std::string, 
    std::vector< std::string >, std::string, std::vector< std::string >, 
    std::vector< std::string >, std::vector< std::string > > LIS;

///
LEAF_DAYCENT_EXPORTS
void Init( std::string const& filePath );

///
LEAF_DAYCENT_EXPORTS
bool CreateSoilFile(
    open::soi::Component const& component,
    std::string const& dir,
    std::string const& siteFile,
    bool const& overwrite = false );

///
LEAF_DAYCENT_EXPORTS
bool CreateClimateFile(
    std::string const& abbr,
    std::string const& wthDC,
    std::string const& cliPath,
    bool const& overwrite = false );

///
LEAF_DAYCENT_EXPORTS
bool UpdateSiteFile(
    std::string const& abbr,
    std::string const& siteFile,
    std::string const& wthPath,
    bool const& overwrite = false );

///
LEAF_DAYCENT_EXPORTS
bool CreateSiteFile(
    std::string const& abbr,
    std::string const& file,
    bool const& overwrite = false );

///
LEAF_DAYCENT_EXPORTS
bool CreateManagementFiles(
    std::string const& abbr,
    std::string const& wepsdir,
    std::string const& dir,
    unsigned int simyears,
    bool const& sim = false,
    bool const& overwrite = false );

///
LEAF_DAYCENT_EXPORTS
void CallDaycent(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& wthDir,
    std::string const& siteDir,
    std::string const& dir,
    unsigned int simyr );

///
LEAF_DAYCENT_EXPORTS
void CallDaycentSpinups(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& dir);

///
LEAF_DAYCENT_EXPORTS
bool CalibrateBaseline(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path);

///
LEAF_DAYCENT_EXPORTS
std::vector< double > GetYields();

///
LEAF_DAYCENT_EXPORTS
void SetScenario( leaf::open::scn::ScenarioPtr scenario );

///
LEAF_DAYCENT_EXPORTS
void SetSustResults(
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& rem,
    std::string const& rotreg,
    std::string const& yield,
    leaf::open::ero::ErosionPtr data);
    /*Poco::Dynamic::Var const& sci,
    Poco::Dynamic::Var const& sciOM,
    Poco::Dynamic::Var const& sciER,
    Poco::Dynamic::Var const& sciFO,
    Poco::Dynamic::Var const& windEro,
    Poco::Dynamic::Var const& waterEro,
    Poco::Dynamic::Var const& totBioRem );*/

///
LEAF_DAYCENT_EXPORTS
void SetInputs( 
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& rem,
    std::vector< std::string > remVec,
    std::string const& rotreg,
    std::vector< std::string > cropVec,
    std::vector< std::string > yldVec,
    std::vector< std::string > eroVec );
LIS GetInputs();

///
LEAF_DAYCENT_EXPORTS
void Exit();

///
LEAF_DAYCENT_EXPORTS
void SetAbbr( std::string abbr );

///
LEAF_DAYCENT_EXPORTS
void SetCMZ( std::string cmz );

std::string GetAbbr();

std::string m_abbr;

///
LEAF_DAYCENT_EXPORTS
void SetInputYield( std::vector< std::string > const& yield );

///
LEAF_DAYCENT_EXPORTS
void SetWindEros( double windEros );

///
LEAF_DAYCENT_EXPORTS
void SetWaterEros( double waterEros );

///
LEAF_DAYCENT_EXPORTS
void SetRemovalRate( std::string removalRate );

///
LEAF_DAYCENT_EXPORTS
void SetCropRotation( std::vector< std::string > rot );

///
LEAF_DAYCENT_EXPORTS
void SetRotation( std::string const& rot );

///
LEAF_DAYCENT_EXPORTS
void SetTillage( std::string tillage );

///
LEAF_DAYCENT_EXPORTS
void SetField( std::string field );

///
LEAF_DAYCENT_EXPORTS
void SetFert( std::string const& abbr,
              std::string const& rot,
              std::string const& fert,
              std::string const& path );

///
LEAF_DAYCENT_EXPORTS
bool CalibrateYield(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path,
    unsigned int simyr,
    bool dist);

///
LEAF_DAYCENT_EXPORTS
bool RunBaseline(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path,
    unsigned int simyrs);

///
LEAF_DAYCENT_EXPORTS
bool DistributedDaycent(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path,
    unsigned int simyrs);

///
LEAF_DAYCENT_EXPORTS
void SetYields(
    std::pair< std::string, std::string > yields);

///
std::vector< double > m_yield;

///
std::string m_filePath;

///
std::vector< std::string > m_erosion;

///
LIS lis;

} //end daycent
} //end wrapper
} //end leaf
