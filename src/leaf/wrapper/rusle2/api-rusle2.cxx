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

// --- ATL Includes --- //
#include <atlstr.h>
using namespace ATL;

// --- LEAF Includes --- //
#include <leaf/wrapper/rusle2/api-rusle2.h>
#include <leaf/wrapper/rusle2/RomeWrapper.h>
#include <leaf/wrapper/rusle2/R2ImportNasis.h>

// --- ROME Includes --- //
#include <cstdlib> //This must be included before api-rome.h
#if __RUSLE2_version <= 2000004
#define AFX_API_IMPORT
#endif //__RUSLE2_version
#include <api-rome.h>
#include <rxencode.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

// --- POCO Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;

// --- STL Includes --- //
#include <sstream>

using namespace leaf::open;
using namespace leaf::util;

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

boost::filesystem::path R2Dir;
boost::filesystem::path RunDir;

////////////////////////////////////////////////////////////////////////////////
void Init(
    std::string const& dbName,
    std::string const& runDir )
{
    fs::path leafDataDir( getenv( "LEAF_DATA_DIR" ) );
    R2Dir = leafDataDir/"rusle2";
    RunDir = R2Dir/runDir;
    fs::path dbPath( RunDir/dbName );

    RomeWrapper::Instance().Init(
        FixUpPath( RunDir.string() ),
        FixUpPath( dbPath.string() ) );
}
////////////////////////////////////////////////////////////////////////////////
SoilError CreateSoilFile(
    soi::ComponentPtr const& component,
    std::string const& fileName,
    bool const& overwrite )
{
    fs::path filePath( RunDir/fileName );
    if( !overwrite && fs::exists( filePath ) )
    {
        return SOIERR_NONE;
    }

    return ReadNASISSoilData( component, FixUpPath( filePath.string() ) );
}
////////////////////////////////////////////////////////////////////////////////
void CreateManFile(
    man::ManagementPtr const& management,
    std::string const& fileName,
    bool const& overwrite )
{
    fs::path filePath( RunDir/fileName );
    if( !overwrite && fs::exists( filePath ) )
    {
        return;
    }

    RomeWrapper& rw = RomeWrapper::Instance();
    RT_FileObj* newMan = rw.OpenFile(
        FixUpPath( "#ENTRY_DEFAULT", OBJTYP_MANAGEMENT ),
        RX_FILESOPEN_INTERNAL );
    assert( newMan != NULL );

    unsigned int nFlags = (ENCODE_XML | ENCODE_SQLITE | ENCODE_FILENAME);
    man::Operations const& ops = management->GetOperations();
    man::OpsBySeq::const_iterator itr = ops.get< man::OP_SEQ >().begin();
    for( int idx = 0; itr != ops.get< man::OP_SEQ >().end(); ++idx, ++itr )
    {
        //
        man::Operation const& op = *itr;

        //Set the operation builder
        rw.SetAttrVal( newMan,
            "OPERATION_BUILDER_PTR",
            "OPERATION_BUILDER", idx );

        //Set the operation
        std::string opname = op.GetName();
        opname = EncodeXML( opname, nFlags );
        opname = FixUpPath( opname, OBJTYP_OPERATION );
        opname = DecodeXML( opname, nFlags );
        rw.SetAttrVal( newMan,
            PTR( OBJTYP_OPERATION ),
            opname, idx );

        //Set the date
        std::string const& date = op.GetDate();
        rw.SetAttrVal( newMan,
            "OP_DATE", date, idx );

        //Set the vegetation
        std::string veg( "" );
        if( op.HasCrop() )
        {
            man::CropPtr const& crop = op.GetCrop();
            veg = crop->GetName();
            veg = EncodeXML( veg, nFlags );
            veg = FixUpPath( veg, OBJTYP_VEGETATION );
            veg = DecodeXML( veg, nFlags );
        }
        rw.SetAttrVal( newMan,
            PTR( OBJTYP_VEGETATION ),
            veg, idx );
    }

    //
    std::string path = FixUpPath(
        filePath.string(), OBJTYP_MANAGEMENT, HASHTAG_SKEL );
    rw.SaveFile( newMan, path );
    rw.CloseFile( newMan );
    newMan = NULL;
}
////////////////////////////////////////////////////////////////////////////////
bool CheckDates()
{
    //Make sure management duration makes sense
    std::string manDuration = GetAttrVal(
        OBJTYP_SLOPE, "MAN_DURATION" );
    std::string durationInMan = GetAttrVal(
        OBJTYP_MANAGEMENT, "DURATION_IN_MAN" );
    std::string durationInManMin = GetAttrVal(
        OBJTYP_MANAGEMENT, "DURATION_IN_MAN_MIN" );
    assert( manDuration == durationInMan && durationInMan == durationInManMin );

    //Check if first date starts at year one
    std::string firstDate = GetAttrVal( OBJTYP_MANAGEMENT, "OP_DATE" );
    assert( !firstDate.empty() );
    unsigned int firstYear = Year::UINT( firstDate );
    assert( firstYear != 0 );
    if( firstYear == 1 ) return true;

    //If first date does not start at year one, adjust all dates
    bool success( true );

    unsigned int numOps = GetAttrSize( OBJTYP_MANAGEMENT, "OP_DATE" );
    assert( numOps > 0 );
    for( unsigned int i = 0; i < numOps; ++i )
    {
        std::string date = GetAttrVal( OBJTYP_MANAGEMENT, "OP_DATE", i );
        assert( !date.empty() );
        unsigned int year = Year::UINT( date );
        assert( year != 0 );
        unsigned int newYear = year - ( firstYear - 1 );
        std::string newDate = Year::Set( newYear, date );
        success = success && SetAttrVal(
            OBJTYP_MANAGEMENT, "OP_DATE", newDate, i );
    }

    //Assert first date starts at year one
    firstDate = GetAttrVal( OBJTYP_MANAGEMENT, "OP_DATE" );
    firstYear = Year::UINT( firstDate );
    assert( firstYear == 1 );

    return success;
}
////////////////////////////////////////////////////////////////////////////////
std::string GetRotationString(
    std::string const& delimiter )
{
    return boost::algorithm::join( GetRotation(), delimiter );
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > GetRotation()
{
    std::vector< std::string > rotation;

    Poco::DateTime dateTime, prevDateTime; int tzd;
    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    if( idx != idxEnd )
    {
        std::string crop = GetAttrVal(
            OBJTYP_MANAGEMENT, PTR( OBJTYP_VEGETATION ), *idx );
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        std::string date = GetAttrVal( OBJTYP_MANAGEMENT, "OP_DATE", *idx );
        assert( !date.empty() );
        Poco::DateTimeParser::parse( "%n/%e/%y", date, dateTime, tzd );
        rotation.push_back( Crop::CROP_TO_ABBR( cropType ) );
        prevDateTime = dateTime;
        ++idx;
    }

    for( ; idx != idxEnd; ++idx )
    {
        std::string crop = GetAttrVal(
            OBJTYP_MANAGEMENT, PTR( OBJTYP_VEGETATION ), *idx );
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        std::string date = GetAttrVal( OBJTYP_MANAGEMENT, "OP_DATE", *idx );
        assert( !date.empty() );
        Poco::DateTimeParser::parse( "%n/%e/%y", date, dateTime, tzd );
        Poco::Timespan timespan = dateTime - prevDateTime;
        if( timespan.days() > 425 )
        {
            rotation.push_back( Crop::CROP_TO_ABBR( Crop::NONE ) );
        }
        rotation.push_back( Crop::CROP_TO_ABBR( cropType ) );
        prevDateTime = dateTime;
    }

    return rotation;
}
////////////////////////////////////////////////////////////////////////////////
std::string GetRunDir()
{
    return RunDir.make_preferred().string();
}
////////////////////////////////////////////////////////////////////////////////
bool SetObjPtr(
    ObjectType const& objType,
    std::string const& fileName,
    FilePrefix const& filePrefix )
{
    fs::path filePath = fileName;
    if( !!filePrefix ) filePath = RunDir/filePath;
    std::string path = FixUpPath( filePath.string(), objType, filePrefix );

    //
    RomeWrapper& rw = RomeWrapper::Instance();
    RT_FileObj*& pObj = rw.OBJECT( objType );
#if __RUSLE2_version >= 2002005
    //RX_FILESOPEN_INTERNAL is important to allow modification of attributes
    pObj = rw.OpenFile( path, RX_FILESOPEN_NO_CREATE | RX_FILESOPEN_INTERNAL );
#else
    pObj = rw.OpenFile( path, RX_FILESOPEN_NO_CREATE );
#endif //__RUSLE2_version
    if( !pObj ) return false;

    return SetAttrVal(
        OBJTYP_SLOPE, PTR( objType ), rw.GetFullname( pObj ) );
}
////////////////////////////////////////////////////////////////////////////////
bool ResetObjPtr(
    ObjectType const& objType )
{
    RomeWrapper& rw = RomeWrapper::Instance();
    RT_FileObj* pObj = rw.OBJECT( objType );
    if( !pObj ) return false;

    bool success( false );
    success = SetAttrVal(
        OBJTYP_SLOPE, PTR( objType ),
        TBL( objType ) + "\\" + "default" );
    assert( success );
    success = rw.FinishUpdates();
    assert( success );
    success = rw.DeleteFile( pObj );
    //success = rw.CloseFile( pObj );
    assert( success );
    pObj = NULL;

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool SaveObjPtr(
    ObjectType const& objType,
    std::string const& fileName,
    FilePrefix const& filePrefix )
{
    fs::path filePath = fileName;
    if( !!filePrefix ) filePath = RunDir/filePath;
    std::string path = FixUpPath( filePath.string(), objType, filePrefix );

    //
    RomeWrapper& rw = RomeWrapper::Instance();
    RT_FileObj*& pObj = rw.OBJECT( objType );
    if( !pObj ) return false;

    //This is to save file to database for testing
    return rw.SaveFile( pObj, path );
}
////////////////////////////////////////////////////////////////////////////////
std::string GetAttrVal(
    ObjectType const& objType,
    std::string const& attrName,
    unsigned int const& index )
{
    RomeWrapper& rw = RomeWrapper::Instance();
    return rw.GetAttrVal( rw.OBJECT( objType ), attrName, index );
}
////////////////////////////////////////////////////////////////////////////////
unsigned int GetAttrSize(
    ObjectType const& objType,
    std::string const& attrName )
{
    RomeWrapper& rw = RomeWrapper::Instance();
    return rw.GetAttrSize( rw.OBJECT( objType ), attrName );
}
////////////////////////////////////////////////////////////////////////////////
bool SetAttrVal(
    ObjectType const& objType,
    std::string const& attrName,
    std::string const& attrVal,
    unsigned int const& index )
{
    RomeWrapper& rw = RomeWrapper::Instance();
    return rw.SetAttrVal( rw.OBJECT( objType ), attrName, attrVal, index );
}
////////////////////////////////////////////////////////////////////////////////
bool AssignSlopeVals(
    Var const& slope,
    Var const& slopeLen )
{
    assert( slope > 0.0 && slope <= 100.0 && slopeLen > 0.0 );
    return SetAttrVal( OBJTYP_SLOPE, "SLOPE_STEEP", slope ) &&
           SetAttrVal( OBJTYP_SLOPE, "SLOPE_HORIZ", slopeLen );
}
////////////////////////////////////////////////////////////////////////////////
std::vector< unsigned int > GetCropIndices()
{
    std::vector< unsigned int > cropIndices;

    unsigned int numVegs = GetAttrSize(
        OBJTYP_MANAGEMENT, PTR( OBJTYP_VEGETATION ) );
    for( unsigned int i = 0; i < numVegs; ++i )
    {
        std::string crop = GetAttrVal(
            OBJTYP_MANAGEMENT, PTR( OBJTYP_VEGETATION ), i );
        if( crop.empty() ) continue;
        assert( crop != "vegetations\\default" );
        cropIndices.push_back( i );
    }

    return cropIndices;
}
////////////////////////////////////////////////////////////////////////////////
bool AssignYields(
    CROP_MAP const& yields,
    bool const& setCvrCrpYlds )
{
    bool success( true );

    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    for( ; idx != idxEnd; ++idx )
    {
        std::string crop = GetAttrVal(
            OBJTYP_MANAGEMENT, PTR( OBJTYP_VEGETATION ), *idx );
        if( Crop::IsCover( crop ) && !setCvrCrpYlds ) continue;
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        CROP_MAP::const_iterator itr = yields.find( cropType );
        assert( itr != yields.end() );
        success = success && SetAttrVal( OBJTYP_MANAGEMENT,
            "MAN_OP_VEG_NUM_HARV_UNITS", itr->second, *idx );
    }

    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool AssignYields(
    CROPYEAR_MAP const& yields,
    bool const& setCvrCrpYlds )
{
    bool success( true );

    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    for( ; idx != idxEnd; ++idx )
    {
        std::string crop = GetAttrVal(
            OBJTYP_MANAGEMENT, PTR( OBJTYP_VEGETATION ), *idx );
        if( Crop::IsCover( crop ) && !setCvrCrpYlds ) continue;
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        std::string date = GetAttrVal( OBJTYP_MANAGEMENT, "OP_DATE", *idx );
        assert( !date.empty() );
        Year::Enum yearEnum = Year::ENUM( date );
        assert( yearEnum != Year::NONE && yearEnum < Year::LAST );

        CROPYEAR_MAP::const_iterator itr =
            yields.find( CROPYEAR_PAIR( cropType, yearEnum ) );
        assert( itr != yields.end() );
        success = success && SetAttrVal( OBJTYP_MANAGEMENT,
            "MAN_OP_VEG_NUM_HARV_UNITS", itr->second, *idx );
    }

    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool AssignYields(
    CROPVAR_VEC& yields,
    unsigned int const& offset,
    bool const& setCvrCrpYlds )
{
    if( offset ) RotationOffset( yields, offset );

    bool success( true );

    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    for( unsigned int i = 0; idx != idxEnd; ++idx, ++i )
    {
        std::string crop = GetAttrVal(
            OBJTYP_MANAGEMENT, PTR( OBJTYP_VEGETATION ), *idx );
        if( Crop::IsCover( crop ) && !setCvrCrpYlds ) continue;
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        CROPVAR_PAIR cv = yields.at( i );
        assert( cv.first == cropType );
        success = success && SetAttrVal( OBJTYP_MANAGEMENT,
            "MAN_OP_VEG_NUM_HARV_UNITS", cv.second, *idx );
    }

    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool SetWindErosionLoss(
    Var const& windEros )
{
    return SetAttrVal( OBJTYP_SLOPE,
        "#RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_WIND_EROSION", windEros );
}
////////////////////////////////////////////////////////////////////////////////
void CallRusle2(
    ero::ErosionPtr& results,
    bool const& reset )
{
    assert( results != ero::ErosionPtr() );

    RomeWrapper& rw = RomeWrapper::Instance();
    assert( rw.Run() );

    results->SetSci( GetAttrVal( OBJTYP_SLOPE,
        "#RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_RESULT" ) );
    results->SetSciOM( GetAttrVal( OBJTYP_SLOPE,
        "#RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_OM_SUBFACTOR" ) );
    results->SetSciER( GetAttrVal( OBJTYP_SLOPE,
        "#RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_ER_SUBFACTOR" ) );
    results->SetSciFO( GetAttrVal( OBJTYP_SLOPE,
        "#RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_FO_SUBFACTOR" ) );
    results->SetWindEros( GetAttrVal( OBJTYP_SLOPE,
        "#RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_WIND_EROSION" ) );
    //These are all the same for our current use of rusle2
    //SLOPE_DEGRAD
    //SLOPE_EROD_FRAC_SOIL_LOSS
    //SLOPE_DETACH
    //SLOPE_DELIVERY
    //SLOPE_SOIL_LOSS
    results->SetWaterEros( GetAttrVal( OBJTYP_SLOPE,
        "SLOPE_DETACH" ) );
    results->SetTotBioRem( GetAttrVal( OBJTYP_SLOPE,
        "ANNUAL_TOTAL_BIOMASS_REMOVAL" ) );

    //RUSLE2_DLL/ccalc.cpp - FnSegSimDaysTotalBiomassRemovalRate
    bool fndVeg( false );
    std::string currVeg;
    unsigned int rnk( 0 );
    unsigned int numSegs = GetAttrSize( OBJTYP_SLOPE, "SEGMENT" );
    unsigned int numSimDays = GetAttrSize( OBJTYP_SLOPE, "DAY_IN_SIM" );
    for( unsigned int dayIndex = 0; dayIndex < numSimDays; ++dayIndex )
    {
        double lengthSum( 0.0 );
        double totLenWeightRemSum( 0.0 );
        for( unsigned int segIndex = 0; segIndex < numSegs; ++segIndex )
        {
            Var var = GetAttrVal( OBJTYP_SLOPE, "SEG_HORIZ", segIndex );
            double segLength = Convert< double >( var, NaN );
            assert( segLength != NaN );
            lengthSum += segLength;

            unsigned int flatIndex = dayIndex + ( segIndex * numSimDays );
            std::string vegString = GetAttrVal(
                OBJTYP_SLOPE, "SEG_SIM_DAY_VEG_NAMES", flatIndex );
            if( !vegString.empty() )
            {
                Crop::Type cropType = Crop::REGEX_SEARCH( vegString );
                if( cropType != Crop::NONE )
                {
                    currVeg = Crop::CROP_TO_ABBR( cropType );
                    if( !fndVeg ) fndVeg = true;
                }
            }

            var = GetAttrVal( OBJTYP_SLOPE,
                "SEG_SIM_DAY_TOTAL_BIOMASS_REMOVAL_RATE", flatIndex );
            double totRemThisDay = Convert< double >( var, NaN );
            if( totRemThisDay == 0.0 || totRemThisDay == NaN ) continue;
            if( !fndVeg ) currVeg = RevSimVegLu( segIndex, numSimDays );
            assert( !( currVeg.empty() ) );

            //Var liveBioThisDay = GetAttrVal( OBJTYP_SLOPE,
                //"SEG_SIM_DAY_LIVE_BIOMASS", flatIndex );
            //Var soilLossThisDay = GetAttrVal( OBJTYP_SLOPE,
                //"SEG_SIM_DAY_SOIL_LOSS_RATE", flatIndex );

            totLenWeightRemSum += totRemThisDay * segLength;
        } //end seg

        if( totLenWeightRemSum <= 0 ) continue;
        //If this is not true, something is seriously wrong
        assert( lengthSum > 0 );
        totLenWeightRemSum /= lengthSum;
        ero::ErosionByCropPtr eroByCrop = ero::ErosionByCrop::Create(
            results, currVeg, totLenWeightRemSum, ++rnk );
    } //end day

    if( reset ) rw.Reset();
}
////////////////////////////////////////////////////////////////////////////////
ero::ErosionPtr CallRusle2(
    scn::ScenarioPtr const& scenario,
    bool const& reset )
{
    ero::ErosionPtr results = ero::Erosion::Create( ero::WATER, scenario );
    CallRusle2( results, reset );
    return results;
}
////////////////////////////////////////////////////////////////////////////////
std::string RevSimVegLu(
    unsigned int segIndex,
    unsigned int numSimDays )
{
    for( unsigned int dayIndex = numSimDays; dayIndex-- > 0; )
    {
        unsigned int flatIndex = dayIndex + ( segIndex * numSimDays );
        std::string vegString = GetAttrVal(
            OBJTYP_SLOPE, "SEG_SIM_DAY_VEG_NAMES", flatIndex );
        if( !vegString.empty() )
        {
            Crop::Type cropType = Crop::REGEX_SEARCH( vegString );
            if( cropType != Crop::NONE )
                return Crop::CROP_TO_ABBR( cropType );
        }
    } //end day

    return "";
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    RomeWrapper::Instance( true );
}
////////////////////////////////////////////////////////////////////////////////

} //end rusle2
} //end wrapper
} //end leaf
