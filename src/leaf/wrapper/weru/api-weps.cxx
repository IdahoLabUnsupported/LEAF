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
#include <leaf/wrapper/weru/api-weps.h>
#include <leaf/wrapper/weru/Harvest.h>
#include <leaf/wrapper/weru/WepsWrapper.h>

#include <leaf/wrapper/weru/mcrew/ConfigData.h>
#include <leaf/wrapper/weru/mcrew/CropObject.h>
#include <leaf/wrapper/weru/mcrew/ManageData.h>
#include <leaf/wrapper/weru/mcrew/OperationObject.h>
#include <leaf/wrapper/weru/mcrew/RowInfo.h>
#include <leaf/wrapper/weru/mcrew/SkelImportPanel.h>
#include <leaf/wrapper/weru/mcrew/XMLConstants.h>

#include <leaf/wrapper/weru/misc/FileInputStream.h>
#include <leaf/wrapper/weru/misc/NonSI.h>
#include <leaf/wrapper/weru/misc/PropertyConfigurator.h>
#include <leaf/wrapper/weru/misc/SI.h>

#include <leaf/wrapper/weru/soil/IFC.h>

#include <leaf/wrapper/weru/weps/location/Site.h>
#include <leaf/wrapper/weru/weps/location/FileStation.h>

#include <leaf/wrapper/weru/weps/reports/ReportManager.h>
#include <leaf/wrapper/weru/weps/reports/query/HarvestsResultSet.h>
#include <leaf/wrapper/weru/weps/reports/query/OutputResultSet.h>
#include <leaf/wrapper/weru/weps/reports/query/SciEnergyResultSet.h>
#include <leaf/wrapper/weru/weps/reports/query/WepsConnection.h>

#include <leaf/wrapper/weru/util/About.h>
#include <leaf/wrapper/weru/util/ConfigData.h>
#include <leaf/wrapper/weru/util/Util.h>

#include <leaf/wrapper/java/io/InputStream.h>
#include <leaf/wrapper/java/lang/System.h>

using namespace leaf::open;
using namespace leaf::util;
using namespace leaf::wrapper;

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

// --- POCO Includes --- //
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Environment.h>

using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

// --- STL Includes --- //
#include <cstdlib>
#include <sstream>
#include <fstream>

namespace leaf
{
namespace wrapper
{
namespace weru
{

fs::path WepsDir;
fs::path RunDir;

////////////////////////////////////////////////////////////////////////////////
void Init(
    std::string const& runDir )
{
    //This must be called before any java wrapper calls are made
    java::VM::Instance();

    //
    fs::path leafDataDir( getenv( "LEAF_DATA_DIR" ) );
    WepsDir = leafDataDir/"weps";
    RunDir = WepsDir/runDir;

    //
    fs::path sterr( RunDir/"stderr.txt" ); fs::remove( sterr );
    fs::path stout( RunDir/"stdout.txt" ); fs::remove( stout );
    fs::path log( RunDir/"weps.log" ); fs::remove( log );

    //
    fs::path cfg( WepsDir/"cfg" );
    fs::path mainConfig( cfg/"weps.cfg" );
    fs::path userConfig( cfg/"weps_user.cfg" );
    fs::path logConfig( cfg/"log.cfg" );

    //
    fs::path tables( WepsDir/"tables" );
    fs::path tablesHack( leafDataDir/"tables" );
    fs::path detailTableFile( tables/"detail.xml" );
    fs::path detailTableFileHack( tablesHack/"detail.xml" );

    //HACK: for hard-coded OutputResultSet::PATH_META = "tables/detail.xml";
    fs::create_directories( tablesHack );
    if( !( fs::exists( detailTableFileHack ) ) )
        fs::copy_file( detailTableFile, detailTableFileHack );
    fs::path detailTableFilterFile( tables/"nrcs_detail_filters.xml" );

    //
    fs::path mcrew( "mcrew_cfg" );
    fs::path mcrewDataConfigFile( "nrcs_dataconfig.xml" );

    //
    fs::path images( WepsDir/"images" );
    fs::path reportFileName( images/"werureport.gif" ); //NRCSlogoV4.jpg

    //
    java::lang::System::SetErr( java::io::PrintStream( sterr.string() ) );
    java::lang::System::SetOut( java::io::PrintStream( stout.string() ) );

    //Make file line separator unix compatible
    java::lang::System::SetProperty( "line.separator", "\n" );
    java::lang::System::SetProperty( "sun.java2d.d3d", "false" );

    //
    util::About::SetUserWepsRoot( misc::File( RunDir.string() ) );
    util::ConfigData::Load(
        misc::File( mainConfig.string() ), misc::File( userConfig.string() ) );
    util::ConfigData::SetData( util::ConfigData::DetailTableFilterFile(),
        detailTableFilterFile.string() );
    util::ConfigData::SetData( util::ConfigData::MCREW(), mcrew.string() );
    util::ConfigData::SetData(
        util::ConfigData::McrewDataConfigFile(), mcrewDataConfigFile.string() );
    util::ConfigData::SetData(
        util::ConfigData::ProjDir(), RunDir.string() );
    util::ConfigData::SetData(
        util::ConfigData::SingleProjectMode(), "1" );
    util::ConfigData::SetData(
        util::ConfigData::CurrentProj(), RunDir.string() );
    util::ConfigData::SetData(
        util::ConfigData::ReportsCustomized(), "nrcs" );
    util::ConfigData::SetData(
        util::ConfigData::ReportFileName(), reportFileName.string() );
    util::ConfigData::SetData(
        util::ConfigData::Units(), util::Util::USUnits() );
    mcrew::ConfigData::Initialize();
    WepsWrapper::Instance().Initialize( WepsDir, RunDir );

    //Configure logging
    java::util::Properties logProps;
    logProps.Load(
        misc::FileInputStream( java::io::File( logConfig.string() ) ) );
    logProps.SetProperty( "log4j.appender.file.File", log.string() );
    misc::PropertyConfigurator::Configure( logProps );
}
////////////////////////////////////////////////////////////////////////////////
void UseCycleCount(
    unsigned int const& cycleCount )
{
    weps::RunFileDataPtr rfd = WepsWrapper::Instance().rfd();
    rfd->SetData( weps::RunFileData::RunTypeDisp(), util::ConfigData::Cycle() );
    std::ostringstream CycleCount; CycleCount << cycleCount;
    rfd->SetData( weps::RunFileData::CycleCount(), CycleCount.str() );
}
////////////////////////////////////////////////////////////////////////////////
void SetLocation(
    std::string const& fips )
{
    weps::RunFileDataPtr rfd = WepsWrapper::Instance().rfd();
    weps::RunFileBeanPtr rfb = rfd->GetBean();
    weps::location::Site site(
        "FIPS:US-" + fips.substr( 0, 2 ) + "-" + fips.substr( 2, 3 ) );
    rfb->SetSite( site );
}
////////////////////////////////////////////////////////////////////////////////
void SetLocation(
    double const& latitude,
    double const& longitude )
{
    weps::RunFileDataPtr rfd = WepsWrapper::Instance().rfd();
    weps::RunFileBeanPtr rfb = rfd->GetBean();
    misc::LatLong latLong( latitude, longitude );
    rfb->SetLatLong( latLong );
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

    soil::NASIS nasis( component->GetNumHorizons() );
    SoilError err = nasis.LoadSQL( component );
    if( err == SOIERR_NONE )
    {
        soil::IFC ifc( nasis );
        ifc.WriteNewIfc( filePath.string() );
        bool exists = fs::exists( filePath ) && fs::file_size( filePath );
        if( !exists ) err = SOIERR_NAN;
    }

    return err;
}
////////////////////////////////////////////////////////////////////////////////
bool CreateManFile(
    std::string const& fileName,
    bool const& overwrite )
{
    fs::path filePath( RunDir/fileName );
    if( !overwrite && fs::exists( filePath ) )
    {
        return true;
    }

    mcrew::SkelImportPanelPtr skelimport = mcrew::SkelImportPanel::Create();
    return skelimport->SkelToMan( filePath.string(), "" );
}
////////////////////////////////////////////////////////////////////////////////
bool CheckDates()
{
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();

    return ( manData->GetRotationYears() == manData->CheckRotationYears() );
}
////////////////////////////////////////////////////////////////////////////////
bool CheckManagement(
    std::vector< std::string >* msgs )
{
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();

    if( msgs )
    {
        if( !( msgs->empty() ) ) msgs->clear();
    }

    java::util::Vector< java::lang::String > v;
    if( manData->CheckAllConditions( v ) )
    {
        return true;
    }

    if( msgs )
    {
        msgs->resize( v.Size() );
        for( unsigned int i = 0; i < v.Size(); ++i )
        {
            java::lang::StringPtr str = v.ElementAt( i );
            msgs->push_back( str->StdString() );
        }
    }

    return false;
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
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    if( idx != idxEnd )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( *idx );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        std::string crop = crpObj->GetCropName()->StdString();
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        std::string date = row->GetDate()->ToString()->StdString();
        assert( !date.empty() );
        Poco::DateTimeParser::parse( "%e/%n/%y", date, dateTime, tzd );
        rotation.push_back( Crop::CROP_TO_ABBR( cropType ) );
        prevDateTime = dateTime;
        ++idx;
    }

    for( ; idx != idxEnd; ++idx )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( *idx );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        std::string crop = crpObj->GetCropName()->StdString();
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        std::string date = row->GetDate()->ToString()->StdString();
        assert( !date.empty() );
        Poco::DateTimeParser::parse( "%e/%n/%y", date, dateTime, tzd );
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
void SetMaxRotYears(
    unsigned int const& maxRotYears )
{
    WepsWrapper::Instance().SetMaxRotYears( maxRotYears );
}
////////////////////////////////////////////////////////////////////////////////
bool SetRfdFilePtr(
    Rfd::File const& rfdFile,
    std::string const& fileName )
{
    return WepsWrapper::Instance().SetRfdFileName( rfdFile, fileName );
}
////////////////////////////////////////////////////////////////////////////////
bool AssignSlopeVals(
    Var const& slope,
    Var const& slopeLen )
{
    assert( slope > 0.0 && slopeLen > 0.0 );
    weps::RunFileDataPtr rfd = WepsWrapper::Instance().rfd();
    rfd->SetData( weps::RunFileData::AverageSlope(), slope );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< unsigned int > GetCropIndices(
    bool const& onlyCal )
{
    std::vector< unsigned int > cropIndices;

    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    for( unsigned int i = 0; i < rows->Size(); ++i )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( i );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        if( !oprObj->HasCrop() ) continue;
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        std::string crop = crpObj->GetCropName()->StdString();
        //HACK: fix this somehow
        if( crop == "manure, liquid" ) continue;
        if( onlyCal )
        {
            //Test if crop was calibrated
            Var cbaflag = crpObj->GetValue( "cbaflag" )->StdString();
            if( cbaflag == 0 ) continue;
        }
        cropIndices.push_back( i );
    }

    return cropIndices;
}
////////////////////////////////////////////////////////////////////////////////
bool AssignYields(
    CROP_MAP const& yields,
    bool const& setCvrCrpYlds )
{
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    for( ; idx != idxEnd; ++idx )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( *idx );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        //Biomass adjustment factor
        crpObj->SetValue( "cbafact", "1", 0 );
        std::string crop = crpObj->GetCropName()->StdString();
        if( Crop::IsCover( crop ) && !setCvrCrpYlds ) continue;
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        CROP_MAP::const_iterator itr = yields.find( cropType );
        assert( itr != yields.end() );
        crpObj->SetValue( "tgtyield", itr->second, 0 );
    }

    return WepsWrapper::Instance().WriteManFile();
}
////////////////////////////////////////////////////////////////////////////////
bool AssignYields(
    CROPYEAR_MAP const& yields,
    bool const& setCvrCrpYlds )
{
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    for( ; idx != idxEnd; ++idx )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( *idx );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        //Biomass adjustment factor
        crpObj->SetValue( "cbafact", "1", 0 );
        std::string crop = crpObj->GetCropName()->StdString();
        if( Crop::IsCover( crop ) && !setCvrCrpYlds ) continue;
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        std::string date = row->GetDate()->ToString()->StdString();
        assert( !date.empty() );
        Year::Enum yearEnum = Year::ENUM( date );
        assert( yearEnum != Year::NONE && yearEnum < Year::LAST );

        CROPYEAR_MAP::const_iterator itr =
            yields.find( CROPYEAR_PAIR( cropType, yearEnum ) );
        assert( itr != yields.end() );
        crpObj->SetValue( "tgtyield", itr->second, 0 );
    }

    return WepsWrapper::Instance().WriteManFile();
}
////////////////////////////////////////////////////////////////////////////////
bool AssignYields(
    CROPVAR_VEC& yields,
    unsigned int const& offset,
    bool const& setCvrCrpYlds )
{
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    std::vector< unsigned int > cropIndices = GetCropIndices();
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    for( unsigned int i = 0; idx != idxEnd; ++idx, ++i )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( *idx );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        //Biomass adjustment factor
        crpObj->SetValue( "cbafact", "1", 0 );
        std::string crop = crpObj->GetCropName()->StdString();
        if( Crop::IsCover( crop ) && !setCvrCrpYlds ) continue;
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );

        CROPVAR_PAIR cv = yields.at( i );
        assert( cv.first == cropType );
        crpObj->SetValue( "tgtyield", cv.second, 0 );
    }

    return WepsWrapper::Instance().WriteManFile();
}
////////////////////////////////////////////////////////////////////////////////
/*bool CalibrationsExist(
    Var const& name,
    Var const& objid,
    Var const& compval,
    Var const& rotation,
    Var const& tillreg,
    CROP_MAP const& yields,
    bool const& useDate,
    bool const& useCrpAbbr )
{
    bool exist( true );

    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    for( unsigned int i = 0; i < rows->Size(); ++i )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( i );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        if( !( oprObj->HasCrop() ) ) continue;

        //Test if crop was calibrated
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        Var cbaflag = crpObj->GetValue( "cbaflag" );
        if( cbaflag == 0 ) continue;

        //Get the crop
        std::string crop = *crpObj->GetCropName();
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );
        if( useCrpAbbr ) crop = Crop::CROP_TO_ABBR( cropType );

        //Get the date
        std::string date = *row->GetDate()->ToString();
        Year::Enum yearEnum = Year::ENUM( date );
        assert( yearEnum != Year::NONE && yearEnum < Year::LAST );
        if( !useDate ) date.clear();

        //Get the target yield
        CROP_MAP::const_iterator itr = yields.find( cropType );
        assert( itr != yields.end() );
        Var const& tgtyield = itr->second;

        //Does it exist?
        exist = exist && CalibrationAdjustment::Exists(
            name, objid, compval, rotation, tillreg, tgtyield, date, crop );
    }

    return exist;
}*/
////////////////////////////////////////////////////////////////////////////////
/*bool AssignCalibrations(
    Var const& name,
    Var const& objid,
    Var const& compval,
    Var const& rotation,
    Var const& tillreg,
    bool const& useDate,
    bool const& useCrpAbbr )
{
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    for( unsigned int i = 0; i < rows->Size(); ++i )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( i );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        if( !( oprObj->HasCrop() ) ) continue;

        //Test if crop was calibrated
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();
        Var cbaflag = crpObj->GetValue( "cbaflag" );
        if( cbaflag == 0 ) continue;

        //Get the crop
        std::string crop = *crpObj->GetCropName();
        if( useCrpAbbr )
        {
            Crop::Type cropType = Crop::REGEX_SEARCH( crop );
            assert( cropType != Crop::NONE );
            crop = Crop::CROP_TO_ABBR( cropType );
        }

        //Get the date
        std::string date;
        if( useDate )
        {
            date = *row->GetDate()->ToString();
            Year::Enum yearEnum = Year::ENUM( date );
            assert( yearEnum != Year::NONE && yearEnum < Year::LAST );
        }

        //Get the target yield
        Var tgtyield = crpObj->GetValue( "tgtyield" );

        //Get the adjustment factor
        Var bioAdjFac =
            CalibrationAdjustment::GetBioAdjFactor(
                name, objid, compval, rotation, tillreg, tgtyield, date, crop );

        //Set the adjustment factor in the crop object
        crpObj->SetValue( "cbafact", bioAdjFac, 0 );
    }

    //Write out the management file with the new calibration data
    return WepsWrapper::Instance().WriteManFile();
}*/
////////////////////////////////////////////////////////////////////////////////
bool SaveCalibrations(
    scn::ScenarioPtr const& scenario )
{
    mcrew::ManageDataPtr manData = WepsWrapper::Instance().GetManageData();
    java::util::Vector< mcrew::RowInfo >::SPtr rows = manData->GetRows();
    std::vector< unsigned int > cropIndices = GetCropIndices( true );
    std::vector< unsigned int >::const_iterator idx = cropIndices.begin();
    std::vector< unsigned int >::const_iterator idxEnd = cropIndices.end();
    for( unsigned int i = 0; idx != idxEnd; ++idx, ++i )
    {
        mcrew::RowInfoPtr row = rows->ElementAt( *idx );
        mcrew::OperationObjectPtr oprObj =
            mcrew::RowInfo::GetOperation( *row->GetAllDataObjects() );
        mcrew::CropObjectPtr crpObj = oprObj->GetCrop();

        //Get the crop
        std::string crop = crpObj->GetCropName()->StdString();
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );
        crop = Crop::CROP_TO_ABBR( cropType );

        //Get the date
        std::string date = row->GetDate()->ToString()->StdString();

        //Retrieve calibration
        CalibrationPtr calib =
            WepsWrapper::Instance().GetCalibration( scenario, date, crop );

        //Get the target yield
        Var tgtYld = crpObj->GetValue( "tgtyield" )->StdString();
        calib->SetTgtYld( tgtYld );

        //Write to database
        calib->WriteToDB();
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool SetWaterErosionLoss(
    Var const& waterEros )
{
    weps::RunFileDataPtr rfd = WepsWrapper::Instance().rfd();
    rfd->SetData( weps::RunFileData::WaterErosionLoss(), waterEros );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
ero::ErosionPtr CallWeps(
    scn::ScenarioPtr const& scenario,
    unsigned int const& numCropCalItrs,
    unsigned int const& maxCalCycles,
    unsigned int const& reportNames )
{
    //Run WEPS
    WepsWrapper::Instance().RunWeps( scenario, numCropCalItrs, maxCalCycles );
    if( numCropCalItrs ) SaveCalibrations( scenario );

    //Get results
    weps::reports::query::WepsConnectionPtr wc =
        weps::reports::ReportManager::GetConnection(
            misc::File( RunDir.string() ) );

    //Harvests
    /*java::sql::ResultSetPtr harvestsResultSet =
        weps::reports::query::WepsConnection::SqlFunctionWeps( *wc,
            weps::reports::query::HarvestsResultSet::NAME() );
    assert( harvestsResultSet != java::sql::ResultSetPtr() );
    while( harvestsResultSet->Next() )
    {
        HarvestPtr harvest = Harvest::Create( scenario );
        assert( harvest != HarvestPtr() );

        harvest->SetCrop( harvestsResultSet->Get< java::lang::String >(
            weps::reports::query::HarvestsResultSet::COLUMN_CROP() )->
                StdString() );
        harvest->SetCropIndex( harvestsResultSet->Get< int >(
            weps::reports::query::HarvestsResultSet::COLUMN_CROPINDEX() ) );
        harvest->SetCycleNumber( harvestsResultSet->Get< int >(
            weps::reports::query::HarvestsResultSet::COLUMN_CYCLENUMBER() ) );
        harvest->SetDryYield( harvestsResultSet->Get< double >(
            weps::reports::query::HarvestsResultSet::COLUMN_DRYYIELD() ) );
        harvest->SetHarvestDate( harvestsResultSet->Get< java::lang::String >(
            weps::reports::query::HarvestsResultSet::COLUMN_HARVESTDATE() )->
                StdString() );
        harvest->SetIndex( harvestsResultSet->Get< double >(
            weps::reports::query::HarvestsResultSet::COLUMN_INDEX() ) );
        harvest->SetMoisture( harvestsResultSet->Get< double >(
            weps::reports::query::HarvestsResultSet::COLUMN_MOISTURE() ) );
        harvest->SetResidue( harvestsResultSet->Get< double >(
            weps::reports::query::HarvestsResultSet::COLUMN_RESIDUE() ) );
        harvest->SetRunId( harvestsResultSet->Get< int >(
            weps::reports::query::HarvestsResultSet::COLUMN_RUNID() ) );
        harvest->SetYield( harvestsResultSet->Get< double >(
            weps::reports::query::HarvestsResultSet::COLUMN_YIELD() ) );
        harvest->SetYieldUnits( harvestsResultSet->Get< java::lang::String >(
            weps::reports::query::HarvestsResultSet::COLUMN_YIELDUNITS() )->
                StdString() );

        harvest->WriteToDB();
    }*/

    ero::ErosionPtr results = ero::Erosion::Create( ero::WIND, scenario );
    assert( results != ero::ErosionPtr() );

    //SCI Energy
    java::sql::ResultSetPtr sciEnergyResultSet =
        weps::reports::query::WepsConnection::SqlFunctionWeps( *wc,
            weps::reports::query::SciEnergyResultSet::NAME() );
    assert( sciEnergyResultSet != java::sql::ResultSetPtr() );
    while( sciEnergyResultSet->Next() )
    {
        results->SetSci( sciEnergyResultSet->Get< double >(
            weps::reports::query::SciEnergyResultSet::COLUMN_SCI() ) );
        results->SetSciOM( sciEnergyResultSet->Get< double >(
            weps::reports::query::SciEnergyResultSet::COLUMN_OMFACTOR() ) );
        results->SetSciER( sciEnergyResultSet->Get< double >(
            weps::reports::query::SciEnergyResultSet::COLUMN_ERFACTOR() ) );
        results->SetSciFO( sciEnergyResultSet->Get< double >(
            weps::reports::query::SciEnergyResultSet::COLUMN_FOFACTOR() ) );
        results->SetWindEros( sciEnergyResultSet->Get< double >(
            weps::reports::query::SciEnergyResultSet::COLUMN_WINDEROS() ) );
        results->SetWaterEros( sciEnergyResultSet->Get< double >(
            weps::reports::query::SciEnergyResultSet::COLUMN_WATEREROS() ) );
        results->SetTotBioRem( 0.0 );
    }

    //Generate report PDFs if requested
    weps::reports::ReportManager::GenerateReportPDFs(
        scenario, ( WepsDir/"reports" ), RunDir, reportNames );

    //Reset
    WepsWrapper::Instance().Reset();

    return results;
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    //
    WepsWrapper::Instance( true );

    //Deallocate static weps interfaces; order should not matter
    mcrew::XMLConstants::Static();

    misc::NonSI::Static();
    misc::SI::Static();

    util::ConfigData::Static();
    util::Util::Static();

    weps::RunFileData::Static();

    weps::location::StationMode::Static();

    weps::reports::ReportManager::Static();

    weps::reports::query::HarvestsResultSet::Static();
    weps::reports::query::OutputResultSet::Static();
    weps::reports::query::SciEnergyResultSet::Static();

    //
    java::VM::Instance( true );
}
////////////////////////////////////////////////////////////////////////////////

} //end weru
} //end wrapper
} //end leaf
