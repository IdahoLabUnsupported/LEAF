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
#include <leaf/wrapper/weru/WepsWrapper.h>

#include <leaf/wrapper/java/lang/Thread.h>

#include <leaf/wrapper/weru/mcrew/JulianCalendar.h>

#include <leaf/wrapper/weru/util/ConfigData.h>
#include <leaf/wrapper/weru/util/LoadingContext.h>
#include <leaf/wrapper/weru/util/RelativeFileContext.h>
#include <leaf/wrapper/weru/util/Util.h>

#include <leaf/wrapper/weru/util/windgen/MakeInterpolatedStation.h>

#include <leaf/wrapper/weru/weps/location/FileStation.h>
#include <leaf/wrapper/weru/weps/location/CligenStation.h>
#include <leaf/wrapper/weru/weps/location/WindgenStation.h>

#include <leaf/util/RotationHelper.h>

using namespace leaf::open;
using namespace leaf::util;

// --- Boost Includes --- //
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace fs = boost::filesystem;

// --- POCO Includes --- //
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Environment.h>

using namespace Poco;
using namespace Poco::Dynamic;

// --- STL Includes --- //
#include <iostream>
#include <sstream>
#include <fstream>

namespace leaf
{
namespace wrapper
{
namespace weru
{

////////////////////////////////////////////////////////////////////////////////
WepsWrapper::WepsWrapper()
    :
    m_maxRotYears( 0 ),
    m_manageData(),
    m_rfd(),
    m_locationPanel(),
    m_calibs()
{
    m_fileMap[ Rfd::CLIMATE ] = "";
    m_fileMap[ Rfd::WIND ] = "";
    m_fileMap[ Rfd::MANAGEMENT ] = "";
    m_fileMap[ Rfd::SOIL ] = "";
}
////////////////////////////////////////////////////////////////////////////////
WepsWrapper::~WepsWrapper()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& WepsWrapper::GetRfdFileName(
    Rfd::File const& rfdFile ) const
{
    //
    FileMap::const_iterator itr = m_fileMap.find( rfdFile );
    assert( itr != m_fileMap.end() );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
weps::RunFileDataPtr WepsWrapper::rfd() const
{
    return m_rfd;
}
////////////////////////////////////////////////////////////////////////////////
WepsWrapper& WepsWrapper::Instance(
    bool const& erase )
{
    static WepsWrapper* s_wepsWrapper = new WepsWrapper();
    if( erase )
    {
        delete s_wepsWrapper;
        s_wepsWrapper = NULL;
    }

    return *s_wepsWrapper;
}
////////////////////////////////////////////////////////////////////////////////
void WepsWrapper::Initialize(
    fs::path const& wepsPath,
    fs::path const& runDirPath )
{
    m_wepsPath = wepsPath;
    m_runDirPath = runDirPath;

    m_rfd = java::lang::Object::Create< weps::RunFileData >();
    m_locationPanel =
        weps::location::LocationPanel::Create( *m_rfd->GetBean() );

    Reset();
}
////////////////////////////////////////////////////////////////////////////////
CalibrationPtr WepsWrapper::GetCalibration(
    scn::ScenarioPtr const& scenario,
    Var const& date,
    Var const& crop,
    bool const& mustExist ) const
{
    CalibsByKey::const_iterator itr = m_calibs.get< CALIBKEY >().find(
        boost::make_tuple( scenario->GetId(), date, crop ) );
    if( itr == m_calibs.get< CALIBKEY >().end() )
    {
        if( !mustExist ) return CalibrationPtr();

        std::stringstream err;
        err << "leaf::wrapper::weru::WepsWrapper::GetCalibration() "
            << "could not find specified key in Calibration map!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
Calibrations const& WepsWrapper::GetCalibrations() const
{
    return m_calibs;
}
////////////////////////////////////////////////////////////////////////////////
mcrew::ManageDataPtr WepsWrapper::GetManageData() const
{
    return m_manageData;
}
////////////////////////////////////////////////////////////////////////////////
void WepsWrapper::Reset()
{
    //Remove old files from last run
    RemoveOldFiles();

    //
    FileMap::iterator itr = m_fileMap.begin();
    for( ; itr != m_fileMap.end(); ++itr ) itr->second = "";

    //
    m_manageData = mcrew::ManageDataPtr();

    //
    m_rfd->Initialize();
    java::lang::Thread::WaitForEDT();
    util::LoadingContext::Enter();
    m_rfd->FireAll();
    util::LoadingContext::Exit();
    java::lang::Thread::WaitForEDT();
}
////////////////////////////////////////////////////////////////////////////////
void WepsWrapper::RunCligen(
    unsigned int const& startYear,
    unsigned int const& totalYears )
{
    fs::path filePath( m_runDirPath/GetRfdFileName( Rfd::CLIMATE ) );

    weps::RunFileBeanPtr rfb = m_rfd->GetBean();
    weps::location::StationPtr station = rfb->GetCligenStation();

    if( !( fs::exists( filePath ) ) )
    {
        switch( station->GetType() )
        {
        case weps::location::Station::CLIGEN:
        {
            weps::location::CligenStationPtr cligenStation =
                boost::static_pointer_cast<
                    weps::location::CligenStation >( station );

            std::string cliExe = util::ConfigData::GetData(
                util::ConfigData::CliExe() )->StdString();
            cliExe = ( m_wepsPath/cliExe ).string();
            std::string cliData = util::ConfigData::GetDataParsed(
                util::ConfigData::CliData() )->StdString();

            //-S<state number> -s<station ID number>
            //-i<input file name> -o<output file name>
            //-b<beginning year> -y<duration in years>
            //-f (old WEPS record format)
            //-F (overwrite output file if it exists)
            //-H (omit header output) -r<random seed>
            //-t<Sim Type (WEPP: 4=SglStm, 5=Contin)>
            //-I0 <no interpolation (default)>
            //-I1 <linear interpolation>
            //-I2 <Fourier interpolation>
            //-I3 <interpolation to preserve avgs>
            //-v, -V<verbose> -h, -?, -\\? <help>
            //-O <option 6 -- observed data filename>
            Poco::Process::Args args;
            std::ostringstream S; S << "-S" << cligenStation->GetState();
            args.push_back( S.str() );
            std::ostringstream s; s << "-s" << cligenStation->GetId();
            args.push_back( s.str() );
            args.push_back( "-t5" );
            args.push_back( "-I3" );
            args.push_back( "-F" );
            args.push_back( "-i\"" + cliData + "\"" );
            std::ostringstream b; b << "-b" << startYear;
            args.push_back( b.str() );
            std::ostringstream y; y << "-y" << totalYears;
            args.push_back( y.str() );
            args.push_back( "-o\"" + filePath.string() + "\"" );

            //Launch the cligen file builder
            Poco::Pipe outPipe, errPipe;
            Poco::ProcessHandle ph =
                Poco::Process::launch( cliExe, args, NULL, &outPipe, &errPipe );

            Poco::PipeInputStream outIstr( outPipe );
            std::ofstream outOstr(
                ( m_runDirPath/"cligen.output" ).string().c_str(),
                std::ios::out | std::ios::trunc | std::ios::binary );
            Poco::StreamCopier::copyStreamUnbuffered( outIstr, outOstr );

            Poco::PipeInputStream errIstr( errPipe );
            std::ofstream errOstr(
                ( m_runDirPath/"cligen.error" ).string().c_str(),
                std::ios::out | std::ios::trunc | std::ios::binary );
            Poco::StreamCopier::copyStreamUnbuffered( errIstr, errOstr );

            //Wait until child processes exit
            int numSecsToExecute = Poco::Process::wait( ph );

            break;
        } //end CLIGEN
        default:
        {
            throw std::runtime_error( "Cligen station has the wrong type!" );
        }
        } //end case
    }

    misc::File relative( filePath.string() );
    util::RelativeFileContext::Enter( relative );
    rfb->SetCligenStation( weps::location::FileStation( relative ) );
    util::RelativeFileContext::Exit();
}
////////////////////////////////////////////////////////////////////////////////
void WepsWrapper::RunWindgen(
    unsigned int const& startYear,
    unsigned int const& totalYears )
{
    fs::path filePath( m_runDirPath/GetRfdFileName( Rfd::WIND ) );

    weps::RunFileBeanPtr rfb = m_rfd->GetBean();
    weps::location::StationPtr station = rfb->GetWindgenStation();

    if( !( fs::exists( filePath ) ) )
    {
        switch( station->GetType() )
        {
        case weps::location::Station::WINDGEN:
        {
            weps::location::WindgenStationPtr windgenStation =
                boost::static_pointer_cast<
                    weps::location::WindgenStation >( station );

            std::string winExe = util::ConfigData::GetData(
                util::ConfigData::WinExe() )->StdString();
            winExe = ( m_wepsPath/winExe ).string();
            std::string winData = util::ConfigData::GetDataParsed(
                util::ConfigData::WinData() )->StdString();

            //-D         debug flag
            //-V         display version and exit
            //-v         set the verbose option
            //-h         do not display output title heading
            //-l         display long (additional) output
            //-f dbfile  specify wind database file (wind_gen.wdb)
            //-o outfile specify output file (stdout)
            //-s #       specify station WBAN no.
            //-x #       specify station database index no.
            //-r #       specify random number seed (54321)
            //-b #       specify beginning simulation (calendar) year (1)
            //-y #       specify number of years to simulate (1)
            //-u #       specify storm duration length in hours (6)
            //-d #       specify number of days to build storms from (5)
            Poco::Process::Args args;
            std::ostringstream s; s << "-s" << windgenStation->GetWBan();
            args.push_back( s.str() );
            args.push_back( "-f\"" + winData + "\"" );
            std::ostringstream b; b << "-b" << startYear;
            args.push_back( b.str() );
            std::ostringstream y; y << "-y" << totalYears;
            args.push_back( y.str() );
            args.push_back( "-o\"" + filePath.string() + "\"" );

            //Launch the windgen file builder
            Poco::Pipe outPipe, errPipe;
            Poco::ProcessHandle ph =
                Poco::Process::launch( winExe, args, NULL, &outPipe, &errPipe );

            Poco::PipeInputStream outIstr( outPipe );
            std::ofstream outOstr(
                ( m_runDirPath/"windgen.output" ).string().c_str(),
                std::ios::out | std::ios::trunc | std::ios::binary );
            Poco::StreamCopier::copyStreamUnbuffered( outIstr, outOstr );

            Poco::PipeInputStream errIstr( errPipe );
            std::ofstream errOstr(
                ( m_runDirPath/"windgen.error" ).string().c_str(),
                std::ios::out | std::ios::trunc | std::ios::binary );
            Poco::StreamCopier::copyStreamUnbuffered( errIstr, errOstr );

            //Wait until child processes exit
            int numSecsToExecute = Poco::Process::wait( ph );

            break;
        } //end WINDGEN
        case weps::location::Station::INTERPOLATED:
        {
            misc::LatLongPtr latLong = station->GetLatLong();
            util::windgen::MakeInterpolatedStation mis;
            std::string WindInterp1EXE = util::ConfigData::GetData(
                util::ConfigData::WindInterp1EXE() )->StdString();
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::INTERPOLATE1_EXE(),
                ( m_wepsPath/WindInterp1EXE ).string() );
            std::string WindInterp2EXE = util::ConfigData::GetData(
                util::ConfigData::WindInterp2EXE() )->StdString();
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::INTERPOLATE2_EXE(),
                ( m_wepsPath/WindInterp2EXE ).string() );
            std::string WinExe = util::ConfigData::GetData(
                util::ConfigData::WinExe() )->StdString();
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::WINDGEN_EXE(),
                ( m_wepsPath/WinExe ).string() );
            std::string WinIndex = util::ConfigData::GetDataParsed(
                util::ConfigData::WinIndex() )->StdString();
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::IDX_FILE(), WinIndex );
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::POL_FILE(),
                *util::Util::FindWindgenProjectionFile()->GetPath() );
            std::string WinData = util::ConfigData::GetDataParsed(
                util::ConfigData::WinData() )->StdString();
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::WDB(), WinData );
            std::ostringstream LAT; LAT << latLong->LatitudeValue();
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::LAT(), LAT.str() );
            std::ostringstream LON; LON << latLong->LongitudeValue();
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::LON(), LON.str() );
            std::ostringstream SIM_START; SIM_START << startYear;
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::SIM_START(),
                SIM_START.str() );
            std::ostringstream SIM_LENGTH; SIM_LENGTH << totalYears;
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::SIM_LENGTH(),
                SIM_LENGTH.str() );
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::WINFILE_NAME(),
                filePath.string() );
            mis.SetParameter(
                util::windgen::MakeInterpolatedStation::WEIGHTSFILE_NAME(),
                ( m_runDirPath/"weights.win" ).string() );
            java::io::PrintWriter out(
                ( m_runDirPath/"windgen.output" ).string() );
            java::io::PrintWriter err(
                ( m_runDirPath/"windgen.error" ).string() );
            mis.SetOut( out, err );
            mis.Run();
            //Don't need the weights file anymore
            fs::remove( m_runDirPath/"weights.win" );

            break;
        } //end INTERPOLATED
        default:
        {
            throw std::runtime_error( "Windgen station has the wrong type!" );
        }
        } //end case
    }

    misc::File relative( filePath.string() );
    util::RelativeFileContext::Enter( relative );
    rfb->SetWindgenStation( weps::location::FileStation( relative ) );
    util::RelativeFileContext::Exit();
}
////////////////////////////////////////////////////////////////////////////////
void WepsWrapper::RunWeps(
    scn::ScenarioPtr const& scenario,
    unsigned int const& numCropCalItrs,
    unsigned int const& maxCalCycles )
{
    //
    fs::path soilPath( m_runDirPath/GetRfdFileName( Rfd::SOIL ) );
    m_rfd->SetData( weps::RunFileData::SoilFile(), soilPath.string() );

    //
    fs::path manPath( m_runDirPath/GetRfdFileName( Rfd::MANAGEMENT ) );
    m_rfd->SetData( weps::RunFileData::ManageFile(), manPath.string() );
    m_rfd->SetRotationYears( manPath.string() );

    //
    unsigned int startYear = 1;
    unsigned int manYears;
    std::istringstream( m_rfd->GetData(
        weps::RunFileData::RotationYears() )->StdString() ) >> manYears;
    assert( m_maxRotYears >= manYears );
    //TODO: This will cause problems if "CycleCount" varies for simulation...
    unsigned int cycleCount;
    std::istringstream( m_rfd->GetData(
        weps::RunFileData::CycleCount() )->StdString() ) >> cycleCount;
    assert( maxCalCycles <= cycleCount );
    unsigned int totalYears = cycleCount * m_maxRotYears;

    //
    m_rfd->UpdateDates();

    //Create windgen and cligen files
    RunWindgen( startYear, totalYears );
    RunCligen( startYear, totalYears );

    //Write the run file
    m_rfd->WriteRunFile( m_runDirPath.string() );

    std::string wepsExe = util::ConfigData::GetData(
        util::ConfigData::WepsExe() )->StdString();
    wepsExe = ( m_wepsPath/wepsExe ).string();

    //-c  Soil Conditioning Index output
    //    0 = no output
    //    1 = create soil-conditioning.out file (default)
    //-C  WEPS crop calibration mode
    //    0 = Do not run crop calibration (default)
    //    # = Run crop calibration # interation max
    //-E  Simulate \"erosion\" in WEPS run
    //    0 = Do not run the erosion submodel
    //    1 = Run the erosion submodel (default)
    //-g  Application of growth stress functions
    //    0 = no growth stress function applied
    //    1 = water stress function applied
    //    2 = temperature stress function applied
    //    3 = both stress functions applied
    //-G  Maximum level of water stress allowed
    //-G0.00 allows maximum water stress to occur
    //-G1.00 does not allow any water stress
    //-I  Specify if initialization is done and if so, the # loops
    //    0 = No initialization
    //    1 = Runs one management cycle (default)
    //    2 = Runs x management cycles
    //-L  Specify soil layer thickness to scale layer splitting (mm)
    //    Specify -L2 for layer splitting to use 2 mm (no decimals)
    //-l  Specify rate of soil layer thickness increase with depth for
    //    layer splitting in percent increase of layer thickness
    //    Specify -l50 to inc. 50 percent for each layer (no decimals)
    //-O  Generate stand alone erosion input file on simulation day
    //    Specify -O2932 to output file on simulation day 2932
    //-o  Generate stand alone erosion input file on DD/MM/YY
    //    Specify -o020901 to output file on day 2 month 9 year 1
    //    Day and month must be 2 digits, Year can be 1 to 4 digits
    //-p  Select soil puddling with saturation all above freezing
    //    0 = disable
    //    1 = enable
    //-P  Specify path to WEPS project run directory
    //    Must be specified if other command line switches are used
    //    Specifying only the path without the \"-P\" option
    //    only works if no other command line switches are specified
    //    e.g: \"weps path_to_weps.run_file\"
    //-r  Select winter annual root depth growth option
    //    0 = depth grows at same rate as height
    //    1 = depth grows with fall heat units
    //-R  WEPS debug messages dumped to screen
    //    0 = no debug messages sent to screen
    //    1 = 1st level debug messages sent to screen
    //    2 = 1st and 2nd level debug messages sent to screen
    //    3 = 1st, 2nd, and 3rd level debug messages sent to screen
    //-s  Specify soil ifc file input format type
    //    0 = new format (default)
    //    1 = old format (slope set in weps.run)
    //-S  Vary type of value input for 1/3 bar, 15 bar water
    //    0 = 1/3bar(vol) 15bar(vol)
    //    1 = 1/3bar(vol) 15bar(grav)
    //    2 = 1/3bar(grav) 15bar(grav)
    //    3 = use texture based calc
    //    4 = use Rawls texture for full properties (default)
    //        Override 1/3bar, 15bar, bulk density w/ texture estimate
    //-t  Confidence Interval on Rotation Mean Annual Erosion
    //    0 = no confidence interval calc (default)
    //    1 = confidence interval reported
    //    2 = used to limit run length (not implem)
    //-T  Deep Furrow Effect on Transpiration Depth
    //    0 = no deep furrow effect (default)
    //    1 = deep furrow affects transpiration
    //-u  Resurfacing buried roots
    //    0 = no resurfacing of buried roots
    //    1 = resurface buried roots (default)
    //-w  Specify method of weighting for layer conductivity and flow
    //    0 = arithmetic mean 0.5 method (default)
    //    1 = layer thickness porportional weighted
    //    2 = internodal method, darcian mean
    //-W  Specify hydrology calculation method used
    //    0 = darcian flow (default)
    //    1 = Green-Ampt infil., simple drainage
    //-X  Specify maximum wind speed cap (m/s)
    //    Specify -X25.0 to limit input wind speeds to a max of 25 m/s
    //-Y  Optional functional Yield/residue ratio
    //    0 = Use full staged biomass partitioning
    //    1 = Use partitioned Yield/residue ratio (default)
    //-Z  Specify maximum number of cycles to run while calibrating
    //    0 = Use all cycles (default)
    Poco::Process::Args args;
    args.push_back( "-W1" );
    args.push_back( "-u0" );
    args.push_back( "-I2" );
    args.push_back( "-t1" );
    args.push_back( "-T1" );
    std::ostringstream C; C << "-C" << numCropCalItrs;
    args.push_back( C.str() );
    std::ostringstream Z; Z << "-Z" << maxCalCycles;
    args.push_back( Z.str() );
    args.push_back( "-P\"" + m_runDirPath.string() + "\"" );

    //Launch the weps executable
    Poco::Pipe outPipe, errPipe;
    Poco::ProcessHandle ph =
        Poco::Process::launch( wepsExe, args, NULL, &outPipe, &errPipe );

    Poco::PipeInputStream outIstr( outPipe );
    std::ofstream outOstr(
        ( m_runDirPath/"weps.output" ).string().c_str(),
        std::ios::out | std::ios::trunc | std::ios::binary );
    Poco::StreamCopier::copyStreamUnbuffered( outIstr, outOstr );

    Poco::PipeInputStream errIstr( errPipe );
    std::ofstream errOstr(
        ( m_runDirPath/"weps.error" ).string().c_str(),
        std::ios::out | std::ios::trunc | std::ios::binary );
    Poco::StreamCopier::copyStreamUnbuffered( errIstr, errOstr );

    //Wait until child processes exit
    int numSecsToExecute = Poco::Process::wait( ph );

    //Read calibration file if it exists
    if( numCropCalItrs ) ReadCalibFile( scenario );
}
////////////////////////////////////////////////////////////////////////////////
bool WepsWrapper::ReadCalibFile(
    scn::ScenarioPtr const& scenario )
{
    m_calibs.clear();

    std::string CalibFileName = weps::RunFileData::CalibFileName().StdString();
    fs::path calibFilePath( m_runDirPath/CalibFileName );
    if( !( fs::exists( calibFilePath ) && fs::file_size( calibFilePath ) ) )
    {
        return false;
    }

    std::ifstream calibFile(
        calibFilePath.string().c_str(), std::ios::in | std::ios::binary );
    if( !calibFile.is_open() )
    {
        return false;
    }

    //Go to end of file
    calibFile.seekg( 0, std::ios::end );
    while( calibFile.peek() != '|' )
    {
        calibFile.seekg( -1, std::ios::cur );
    }
    while( calibFile.peek() != '\n' )
    {
        calibFile.seekg( -1, std::ios::cur );
    }
    calibFile.seekg( 1, std::ios::cur );
    std::string lastLine;
    std::getline( calibFile, lastLine );
    calibFile.close();

    typedef boost::tokenizer< boost::char_separator< char > > toker;
    boost::char_separator< char > sep( "|" );
    toker tokens( lastLine, sep );
    toker::iterator itr = ++(tokens.begin());
    while( itr != tokens.end() )
    {
        //Get the date
        std::string date = *itr; boost::trim( date ); ++itr; ++itr;
        date = mcrew::JulianCalendar( date ).ToString()->StdString();

        //Get the crop
        std::string crop = *itr; boost::trim( crop ); ++itr;
        Crop::Type cropType = Crop::REGEX_SEARCH( crop );
        assert( cropType != Crop::NONE );
        crop = Crop::CROP_TO_ABBR( cropType );

        //Get the bioAdjFac
        std::string bioAdjFac = *itr; boost::trim( bioAdjFac ); ++itr; ++itr;

        m_calibs.insert( Calibration::Create( scenario, date, crop, bioAdjFac ) );
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////
void WepsWrapper::RemoveOldFiles()
{
    //IFC files must be deleted after every run due to
    //weru.weps.reports.query.SoilsResultSet::fill and
    //_runsummary.jrxml: queryString
    //select
    //  runs.*,
    //  soils."soillosst"
    //from weps('runs') AS runs
    //  left join weps('soils') as soils
    //    on runs."runid" = soils."runid"
    //where
    //  runs."runid" = $P{RUN_ID}
    boost::regex e( "(.(ifc|out|run)|logfil.txt|notes.txt)$" );
    fs::directory_iterator dir_iter( m_runDirPath ), dir_end;
    for( ; dir_iter != dir_end; ++dir_iter )
    {
        if( !fs::is_regular_file( dir_iter->status() ) ) continue;
        if( !boost::regex_search(
            dir_iter->path().filename().string(), e ) ) continue;
        fs::remove( *dir_iter );
    }
}
////////////////////////////////////////////////////////////////////////////////
void WepsWrapper::SetMaxRotYears(
    unsigned int const& maxRotYears )
{
    m_maxRotYears = maxRotYears;
}
////////////////////////////////////////////////////////////////////////////////
bool WepsWrapper::SetRfdFileName(
    Rfd::File const& rfdFile,
    std::string const& fileName )
{
    FileMap::iterator itr = m_fileMap.find( rfdFile );
    if( itr == m_fileMap.end() ) return false;
    itr->second = fileName;
    fs::path filePath( m_runDirPath/fileName );
    if( itr->first != Rfd::MANAGEMENT )
    {
        return true;
        //return fs::exists( filePath ) && fs::file_size( filePath );
    }

    //assert( m_manageData == mcrew::ManageDataPtr() );
    m_manageData = java::lang::Object::Create< mcrew::ManageData >();
    return m_manageData->ReadDataFile( filePath.string() );
}
////////////////////////////////////////////////////////////////////////////////
bool WepsWrapper::WriteManFile()
{
    fs::path filePath( m_runDirPath/GetRfdFileName( Rfd::MANAGEMENT ) );
    return m_manageData->WriteDataFile( filePath.string() );
}
////////////////////////////////////////////////////////////////////////////////

} //end weru
} //end wrapper
} //end leaf
