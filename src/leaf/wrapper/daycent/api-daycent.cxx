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


// --- LEAF Includes --- //
#include <leaf/wrapper/daycent/api-daycent.h>
#include <leaf/wrapper/daycent/DaycentWrapper.h>
#include <leaf/wrapper/daycent/PostProcessor.h>
#include <leaf/wrapper/daycent/cal/YieldCalibration.h>
#include <leaf/wrapper/daycent/man/Fert.h>
#include <leaf/wrapper/daycent/soil/Soils.h>
#include <leaf/wrapper/daycent/file/F_header.h>
#include <leaf/wrapper/daycent/list/L_header.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

// --- POCO Includes --- //
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Environment.h>

// --- STL Includes --- //
#include <sstream>
#include <fstream>
#include <iostream>

using namespace leaf::open;

namespace althm = boost::algorithm;
namespace fs = boost::filesystem;

namespace leaf
{
namespace wrapper
{
namespace daycent
{

////////////////////////////////////////////////////////////////////////////////
void Init( std::string const& filePath )
{
    m_filePath = filePath;
    file::F_100::instance()->Init( filePath );
    list::L_100::instance()->Init( filePath );
}
////////////////////////////////////////////////////////////////////////////////
void CallDaycent(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& wepsDir,
    std::string const& siteDir,
    std::string const& dir,
    unsigned int simyr)
{
    CreateManagementFiles( abbr, wepsDir, dir, simyr );

    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.Run( dir, abbr, cokey );
}
////////////////////////////////////////////////////////////////////////////////
void CallDaycentSpinups(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& dir)
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.RunSpinups( dir, abbr, cokey );
}
////////////////////////////////////////////////////////////////////////////////
bool CreateClimateFile(
    std::string const& abbr,
    std::string const& wthDC,
    std::string const& cliPath,
    bool const& overwrite )
{
    if( !fs::exists( wthDC ) )
    {
        fs::create_directories( wthDC );
    }

    fs::path wthFile( wthDC + "/" + abbr );
    if( fs::exists( cliPath ) )
    {
        DaycentWrapper& dc = *( DaycentWrapper::instance() );
        dc.CreateWeatherFile( abbr, wthFile.string(), cliPath );
    }

    return fs::exists( cliPath ) && fs::file_size( cliPath );
}
////////////////////////////////////////////////////////////////////////////////
bool UpdateSiteFile(
    std::string const& abbr,
    std::string const& sitePath,
    std::string const& wthPath,
    bool const& overwrite )
{
    fs::path site( (sitePath + "/" + abbr + ".100" ) );
    if( !fs::exists( site ) )
    {
        CreateSiteFile( abbr, sitePath, true );
    }
    if( fs::exists( site ) )
    {
        std::string wthFile( wthPath );
        wthFile.append( "/" + abbr );
        althm::replace_all( wthFile, "/", "\\" );
        std::string siteFile( sitePath + "/" + abbr );
        althm::replace_all( siteFile, "/", "\\" );
        //file::F_100::instance()->Weather( abbr, sitePath );

    }

    return fs::exists( site ) && fs::file_size( site );
}
////////////////////////////////////////////////////////////////////////////////
bool CreateSoilFile(
    soi::Component const& component,
    std::string const& dir,
    std::string const& abbr,
    bool const& overwrite )
{

    if( !component.HasHorizons() )
    {
        return false;
    }

    soil::Soils soils;
    if( !soils.LoadSQL( component, dir, abbr ) )
    {
        return false;
    }

    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetNLayer( soils.GetNLayer() );
    dc.SetSilt( soils.GetSilt() );
    dc.SetClay( soils.GetClay() );
    dc.SetSand( soils.GetSand() );
    dc.SetBulkDensity( soils.GetBulkDensity() );
    assert( CreateSiteFile( abbr, dir ) );

    std::string path(dir);
    fs::path file( path.append( "/soils.in" ) );

    return fs::exists( file ) && fs::file_size( file );
}
////////////////////////////////////////////////////////////////////////////////
bool CreateSiteFile(
    std::string const& abbr,
    std::string const& file,
    bool const& overwrite )
{
    std::string sitefile = "files/daycent/simfiles/site.100";
    //std::string sitefile = "files/daycent/site.100";
    std::string temp( file );
    temp.append("/" + abbr + ".100" );

    if( !fs::exists( temp ) )
    {
        fs::copy_file( sitefile, temp );
    }

    fs::path cp( fs::current_path() );
    fs::path files( cp/"files/daycent" );

    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.CopyFileDir( files.string(), file );
    dc.GetSpinupFiles( files.string(), file, abbr );
    dc.UpdateSiteFile( abbr, file, sitefile );

    return fs::exists( sitefile ) && fs::file_size( sitefile );
}
////////////////////////////////////////////////////////////////////////////////
bool CreateManagementFiles(
    std::string const& abbr,
    std::string const& wepsdir,
    std::string const& dir,
    unsigned int simyears,
    bool const& sim,
    bool const& overwrite )
{
    if( fs::exists( dir ) )
    {
        DaycentWrapper& dc = *( DaycentWrapper::instance() );
        dc.CreateMgntFiles( wepsdir, dir, abbr, simyears, sim );
    }

    return fs::exists( dir );
}
////////////////////////////////////////////////////////////////////////////////
bool CalibrateYield(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path,
    unsigned int simyr,
    bool dist)
{
    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetYieldInput( m_yield );

    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.BuildSchFile( dc.GetSchedule(), abbr, simyr );

    bool success( yc.CalibrateYield( abbr, cokey, path, simyr, dist ) );
    assert( success );

    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool RunBaseline(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path,
    unsigned int simyrs)
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.BuildSchFile( dc.GetSchedule(), abbr, simyrs, true );

    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    bool success( yc.RunBaseline( abbr, cokey, path ) );
    assert( success );

    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool DistributedDaycent(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path,
    unsigned int simyr)
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.BuildSchFile( dc.GetSchedule(), abbr, simyr );  

    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    bool success( yc.BuildDistributedFiles( abbr, cokey, path, simyr ) ); 
    assert( success );
/*
    PostProcessor& pp = *( PostProcessor::instance() );
    pp.WriteToDB( lis.get<0>(), lis.get<1>(), lis.get<2>(), lis.get<3>(), 
        lis.get<4>(), lis.get<5>(), lis.get<6>(), lis.get<7>(), lis.get<8>(), lis.get<9>() );

    success = yc.CalibrateYield( abbr, cokey, path, simyr, true );*/
    assert( success );

    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool CalibrateBaseline(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path)
{
    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetYieldInput( m_yield );

    bool success( yc.CalibrateBaseline( abbr, cokey, path ) );
    assert( success );

    return success;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< double > GetYields( )
{
    std::vector< double > yields;

    return yields;
}
////////////////////////////////////////////////////////////////////////////////
void SetAbbr( std::string abbr )
{
    m_abbr = abbr;
}
////////////////////////////////////////////////////////////////////////////////
std::string GetAbbr()
{
    return m_abbr;
}
////////////////////////////////////////////////////////////////////////////////
void SetInputYield( std::vector< std::string > const& yield )
{
    m_yield.clear();
    for( size_t i=0; i<yield.size(); ++i )
    {
        m_yield.push_back( boost::lexical_cast< double >(yield.at(i)) );
    }
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetInputYield( m_yield );
}
////////////////////////////////////////////////////////////////////////////////
void SetWindEros( double windEros )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetWindEros( windEros );
}
////////////////////////////////////////////////////////////////////////////////
void SetWaterEros( double waterEros )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetWaterEros( waterEros );
}
////////////////////////////////////////////////////////////////////////////////
void SetRemovalRate( std::string removalRate )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetRemovalRate( removalRate );
}
////////////////////////////////////////////////////////////////////////////////
void SetCropRotation( std::vector< std::string > rot )
{
    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetRotation( rot );
}
////////////////////////////////////////////////////////////////////////////////
void SetRotation( std::string const& rot )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetRotation( rot );
}
////////////////////////////////////////////////////////////////////////////////
void SetTillage( std::string tillage )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetTillage( tillage );
}
////////////////////////////////////////////////////////////////////////////////
void SetCMZ( std::string cmz )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetCMZ( cmz );

    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetCMZ( cmz );
}
////////////////////////////////////////////////////////////////////////////////
void SetScenario( leaf::open::scn::ScenarioPtr scenario )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetTillage( (*scenario).GetTillage().convert< std::string >() );
    dc.SetRemovalRate( (*scenario).GetRemoval().convert< std::string >() );
    dc.SetRotation( (*scenario).GetRotation().convert< std::string >() );

    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetTillage( (*scenario).GetTillage().convert< std::string >() );
    yc.SetRemoval( (*scenario).GetRemoval().convert< std::string >() );
    yc.SetRotation( (*scenario).GetRotation().convert< std::string >() ); 
    yc.SetYield( (*scenario).GetYield().convert< std::string >() ); 
    yc.SetCokey( (*scenario).GetSoil().convert< std::string >() );
}
////////////////////////////////////////////////////////////////////////////////
void SetSustResults( 
    std::string const& abbr,
    std::string const& mukey,
    std::string const& cokey,
    std::string const& tillreg,
    std::string const& rem,
    std::string const& rotreg,
    std::string const& yield,
    leaf::open::ero::ErosionPtr data )
    /*Poco::Dynamic::Var const& sci,
    Poco::Dynamic::Var const& sciOM,
    Poco::Dynamic::Var const& sciER,
    Poco::Dynamic::Var const& sciFO,
    Poco::Dynamic::Var const& windEro,
    Poco::Dynamic::Var const& waterEro,
    Poco::Dynamic::Var const& totBioRem )*/
{
    /*cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetSci( sci );
    yc.SetSciOM( sciOM );
    yc.SetSciER( sciER );
    yc.SetSciFO( sciFO );
    yc.SetWindEros( windEro );
    yc.SetWaterEros( waterEro );
    yc.SetTotBioRem( totBioRem );*/

    PostProcessor& pp = *( PostProcessor::instance() );
    pp.CreateSustDB();
    pp.WriteSustDB( abbr, mukey, cokey, tillreg, rem, rotreg, yield, data );

}
////////////////////////////////////////////////////////////////////////////////
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
    std::vector< std::string > eroVec )
{
    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetAbbr( abbr );
    yc.SetMukey( mukey );
    yc.SetCokey( cokey );
    yc.SetTillage( tillreg );
    //yc.SetRemovalReg( remreg );
    yc.SetRotation( rotreg );
    //yc.SetYield( yldreg );

    PostProcessor& pp = *( PostProcessor::instance() );
    pp.WriteToDB( abbr, mukey, cokey, tillreg, rem, remVec, rotreg, cropVec, yldVec, eroVec );
    //yc.WriteToDB( abbr, mukey, cokey, tillreg, remVec, rotreg, yldreg );

    lis = boost::tuples::make_tuple( abbr, mukey, cokey, tillreg, 
        rem, remVec, rotreg, cropVec, yldVec, eroVec );
}
////////////////////////////////////////////////////////////////////////////////
LIS GetInputs()
{
    return lis;
}
////////////////////////////////////////////////////////////////////////////////
void SetFert( std::string const& abbr,
              std::string const& rot,
              std::string const& fert,
              std::string const& path )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetFert( abbr, rot, fert, path );
}
////////////////////////////////////////////////////////////////////////////////
void SetField( std::string field )
{
    DaycentWrapper& dc = *( DaycentWrapper::instance() );
    dc.SetField( field );

    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetField( field );

    PostProcessor& pp = *( PostProcessor::instance() );
    pp.SetField( field );
}
////////////////////////////////////////////////////////////////////////////////
void SetYields( std::pair< std::string, std::string > yields )
{
    PostProcessor& pp = *( PostProcessor::instance() );
    pp.SetYields( yields );
}
////////////////////////////////////////////////////////////////////////////////
void Exit()
{
    file::F_100::instance( true );
    list::L_100::instance( true );
}
////////////////////////////////////////////////////////////////////////////////

} //end daycent
} //end wrapper
} //end leaf
