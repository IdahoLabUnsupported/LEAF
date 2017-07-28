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


// --- STL Includes --- //
#include <fstream>
#include <iostream>
#include <sstream>

// --- LEAF Includes --- //
#include <leaf/wrapper/daycent/cal/YieldCalibration.h>
#include <leaf/wrapper/daycent/cal/Calibrate.h>
#include <leaf/wrapper/daycent/cal/Common.h>
#include <leaf/wrapper/daycent/PostProcessor.h>
#include <leaf/wrapper/daycent/file/f_header.h>
#include <leaf/wrapper/daycent/list/L_header.h>

// --- POCO Includes --- //
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>

namespace fs = boost::filesystem;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace cal
{

////////////////////////////////////////////////////////////////////////////////
std::map< std::string, double > const& YieldCalibration::crop_conv =
    boost::assign::map_list_of
    ( "CG", 2.31 ) ("SB", 2.98 ) ("WW", 2.50 );
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > const& YieldCalibration::file_ext =
    boost::assign::list_of ( ".bin" ) ( ".lis" );
////////////////////////////////////////////////////////////////////////////////
YieldCalibration::YieldCalibration(void)
    :
    m_abbr( null ),
    m_mukey( null ),
    m_cokey( null ),
    m_muacres( null ),
    m_pctacres( null ),
    m_yld( null ),
    m_wtyld( null ),
    m_tillreg( null ),
    m_remreg( null ),
    m_rotreg( null ),
    m_yldreg( null ),
    m_windEros( null ),
    m_waterEros( null ),
    m_sci( null ),
    m_sciOM( null ),
    m_sciER( null ),
    m_sciFO( null ),
    m_totBioRem( null ),
    m_ftapp(0),
    m_ftside(0)
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
YieldCalibration::~YieldCalibration(void)
{
}
////////////////////////////////////////////////////////////////////////////////
YieldCalibration* YieldCalibration::instance(
    bool const& erase )
{
    static YieldCalibration* sYieldCalibration = new YieldCalibration();
    if( erase )
    {
        delete sYieldCalibration;
        sYieldCalibration = NULL;
    }

    return sYieldCalibration;
}
////////////////////////////////////////////////////////////////////////////////
bool YieldCalibration::CalibrateYield(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& calPath,
    int const& simyr,
    bool dist)
{
    fs::path path( fs::current_path() );

    Calibrate calibrate;
    std::vector< double > prdxes;
    for( size_t i=0; i<m_rot.size();  ++i )
    {
        std::cout << m_rot.size() << " " << m_yield.size() << std::endl;
        prdxes.push_back( calibrate.GetPRDX(
            cokey, m_rot.at(i), m_yield.at(i) ) );
    }
    for( size_t i=0; i<prdxes.size();  ++i )
    {
        std::cout << prdxes.at(i) << std::endl;
    }

    fs::current_path( calPath.c_str() );

    std::vector< double > yield;

    std::vector< std::string >::iterator iter = m_rot.begin();
    for( ; iter != m_rot.end(); ++iter )
    {
        if( *iter == "HV" )
            m_rot.erase( iter );
    }
    std::vector< double > diff( m_rot.size(), 1 );

    //std::reverse( m_rot.begin(), m_rot.end() );

    PostProcessor& pp = *( PostProcessor::instance() );

    unsigned int count(0);
    double cal(0.005);
    double y1(0);
    double y2(0);
    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );
    unsigned int num( simyr/m_rot.size() );

    bool calibrated = false;
    bool loop = false;

    if( simyr != 0 && !dist)
    {
        /*Poco::Dynamic::Var ftapp;
        file::F_100&  f100 = *file::F_100::instance();
        assert( f100.OpenFile( FERT, "", abbr ) );
        file::Option const& opt = f100.GetOption( "NAPP" );
        file::Option const& opt2 = f100.GetOption( "NSIDE" );
        opt.GetValue( "FERAMT(1)", ftapp );
        if( m_ftapp > ftapp )
        {
            opt.SetValue( "FERAMT(1)", m_ftapp );
            opt2.SetValue( "FERAMT(1)", m_ftside );
        }
        std::cout << m_ftapp.convert< double >() <<
            ftapp.convert< double >() <<std::endl;
        f100.WriteOptions();
        f100.CloseFile();
        */
        //make copy of previous binary file to extend from
        if( fs::exists( abbr + "_" + ck + "_prev.bin" ) )
        {
            RemoveAllFilesWithExtension( abbr + "_" + ck + "_prev.bin" );
        }

        fs::copy_file( abbr + "_" + ck + ".bin", abbr + "_" + ck + "_prev.bin" );
    }

    std::vector< Poco::Dynamic::Var > var(m_rot.size(),0);

    //initialize prdx if it exist in db
    for( size_t i=0; i<m_rot.size(); ++i )
    {
        if( prdxes.at(i) != 0 )
        {
            file::F_100&  f100 = *file::F_100::instance();
            assert( f100.OpenFile( CROP, "", abbr ) );
            file::Option const& opt = f100.GetOption( m_rot.at(i) );
            opt.GetValue( "PRDX(1)", var.at(i) );
            std::cout << var.at(i).convert< double >() << std::endl;
            var.at(i) = prdxes.at(i);
            std::cout << var.at(i).convert< double >() << std::endl;
            opt.SetValue( "PRDX(1)", var.at(i) );
            f100.WriteOptions();
            f100.CloseFile();
        }
    }

    while( !calibrated )
    {
        if( count > 20 )
        {
            cal += 0.002;
            count = 0;
        }

        yield.clear();

        std::vector< std::string >::const_iterator extitr = file_ext.begin();
        for( ; extitr != file_ext.end(); ++extitr )
        {
            std::string ext = *extitr;
            std::string file( abbr + "_" + ck + ext );
            RemoveAllFilesWithExtension( file );
        }

        std::string runCmd( path.string() + "/bin/Windows/DailyDayCent.exe" );
        std::string lisCmd( path.string() + "/bin/Windows/DailyDayCent_list100.exe" );

        Poco::Process::Args args;
        //execute spinups
        if( simyr == 0 )
        {
            std::string temp = "CMZ" + m_cmz;
            std::cout << temp << std::endl;
            if( temp.find( "0" ) != std::string::npos )
            {
                temp.erase( temp.find( "0" ), 1 );
            }
            args.push_back( "-s " + abbr );
            args.push_back( "-n " + abbr + "_" + ck);
            args.push_back( "-e " + temp + "_" + abbr + "_" + ck );
        }
        else if( dist )
        {
            args.push_back( "-s " + abbr );
            args.push_back( "-n " + abbr + "_" + ck );
            args.push_back( "-e " + abbr + "_" + ck + "_" + boost::lexical_cast< std::string >(num-1) );
        }
        else
        {
            args.push_back( "-s " + abbr );
            args.push_back( "-n " + abbr + "_" + ck );
            args.push_back( "-e " + abbr + "_" + ck + "_prev" );
        }

        Poco::ProcessHandle ph =
            Poco::Process::launch( runCmd, args, 0, 0, 0 );

        //Wait until child processes exit
        int numSecsToExecute = Poco::Process::wait( ph );

        std::string file( abbr + "_" + ck );
        args.clear();
        args.push_back( file );
        args.push_back( file );
        args.push_back( "outvars.txt" );
        ph = Poco::Process::launch( lisCmd, args, 0, 0, 0 );
        numSecsToExecute = Poco::Process::wait( ph );

        std::vector< double > yc;
        yc = pp.GetResults( abbr, file + ".lis" );

        std::map< std::string, double >::const_iterator itr;
        for( size_t i=0; i<m_rot.size(); ++i )
        {
            std::string curCrop = m_rot.at(i);

            for( itr = crop_conv.begin() ; itr != crop_conv.end(); ++itr )
            {
                if( curCrop.find( (*itr).first ) != std::string::npos )
                {
                    yield.push_back( *(yc.rbegin() + i + 1) / (*itr).second );
                }
            }

            if( curCrop == "CG" )
            {
                y1 = yield.at(i);
            }

            diff.at(i) = (yield.at(i)-m_yield.at(i))/m_yield.at(i);
            std::cout << diff.at(i) << " " << yield.at(i) << " " << m_yield.at(i) << std::endl;
            std::vector< double >::iterator ylditr = diff.begin();
            for( ; ylditr != diff.end(); ++ylditr )
            {
                if( abs((*ylditr)) > cal )
                {
                    calibrated = false;
                    break;
                }
                else
                {
                    calibrated = true;
                }
            }
            if( !calibrated )
            {
                bool fert = false;
                file::F_100&  f100 = *file::F_100::instance();
                assert( f100.OpenFile( CROP, "", abbr ) );
                file::Option const& opt = f100.GetOption( m_rot.at(i) );
                opt.GetValue( "PRDX(1)", var.at(i) );
                std::cout << var.at(i).convert< double >() << std::endl;
                //fix for when not calibrating with nitrogen
                if( var.at(i).convert< double >() > 2 )
                {
                    calibrated = true;
                    var.at(i) = 1.25;
                }
                else if( var.at(i).convert< double >() < 0 )
                {
                    calibrated = true;
                    var.at(i) = 0.25;
                }
/*                if( var.at(i).convert< double >() > 1.5 )
                {
                    var.at(i) = 0.7;
                    if( curCrop == "CG" || curCrop == "CG1" )
                    {
                        fert = true;
                    }
                }
                else
                {
                    if( curCrop == "CG" || curCrop == "CG1" )
                    {
                        std::cout << y1 << "   " << y2 << std::endl;
                        if( y1 > m_yield.at(i) &&
                            y2 < m_yield.at(i) )
                        {
                            if( !loop )
                            {
                                loop = true;
                            }
                            else
                            {
                                var.at(i) = 0.7;
                                loop = false;
                            }
                        }
                    }*/
                    var.at(i) -= diff.at(i) * 0.2;
//                }
                std::cout << var.at(i).convert< double >() << std::endl;
                opt.SetValue( "PRDX(1)", var.at(i) );
                f100.WriteOptions();
                f100.CloseFile();

                prdxes.at(i) = var.at(i).convert< double >();

                if( fert )
                {
                    if( curCrop == "CG" || curCrop == "CG1" )
                    {
                        assert( f100.OpenFile( FERT, "", abbr ) );
                        file::Option const& opt3 = f100.GetOption( "NAPP" );
                        file::Option const& opt4 = f100.GetOption( "NSIDE" );
                        opt3.GetValue( "FERAMT(1)", m_ftapp );
                        opt4.GetValue( "FERAMT(1)", m_ftside );
                        std::cout << m_ftapp.convert< double >() <<
                            m_ftside.convert< double >() <<std::endl;
                        double tmp = m_ftside.convert< double >() -
                            m_ftapp.convert< double >();
                        if(  tmp > 3 )
                        {
                            m_ftapp = (m_ftapp.convert< double >()) + 1;
                        }
                        else
                        {
                             m_ftside = (m_ftside.convert< double >()) + 1;
                        }
                        std::cout << m_ftapp.convert< double >() <<
                            m_ftside.convert< double >() <<std::endl;
                        opt3.SetValue( "FERAMT(1)", m_ftapp );
                        opt4.SetValue( "FERAMT(1)", m_ftside );
                        f100.WriteOptions();
                        f100.CloseFile();

                        fert = false;
                    }
                }
            }
            else
            {
                calibrated = true;
                for( size_t j=0; j<diff.size(); ++j )
                {
                    if( abs( diff.at(j) ) > cal )
                    {
                        std::cout << diff.at(i) << std::endl;
                        calibrated = false;
                        break;
                    }
                }
            }
            if( curCrop == "CG" || curCrop == "CG1" )
            {
                y2 = y1;
            }
        }
        count += 1;
        std::cout << "count: " << count << " " << cal << std::endl;
    }

    fs::path cp( fs::current_path() );

    fs::current_path( path );

    for( size_t i=0; i<m_rot.size(); ++i )
    {
        calibrate.SetPRDX( cokey, m_rot.at(i), m_yield.at(i), prdxes.at(i) );
    }

    if( simyr == 0 )
    {
        pp.CreateOutDB( cp.string() + "/year_summary.out" );
    }

    pp.WriteOutFile( cp.string() + "/year_summary.out", abbr, soil,
        soil, tillage, removal, rotation, yldreg );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool YieldCalibration::CalibrateBaseline(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& calPath,
    unsigned int simyr )
{
    fs::path path( fs::current_path() );
    fs::current_path( calPath.c_str() );

    std::vector< double > yield;

    std::vector< std::string >::iterator iter = m_rot.begin();
    for( ; iter != m_rot.end(); ++iter )
    {
        if( *iter == "HV" )
            m_rot.erase( iter );
    }
    std::vector< double > diff( m_rot.size(), 1 );

    //std::reverse( m_rot.begin(), m_rot.end() );

    PostProcessor& pp = *( PostProcessor::instance() );

    unsigned int count(0);
    double cal(0.005);
    double y1(0);
    double y2(0);
    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );

    bool calibrated = false;
    bool loop = false;

    Poco::Dynamic::Var ftapp;
    file::F_100&  f100 = *file::F_100::instance();
    assert( f100.OpenFile( FERT, "", abbr ) );
    file::Option const& opt = f100.GetOption( "NAPP" );
    file::Option const& opt2 = f100.GetOption( "NSIDE" );
    opt.GetValue( "FERAMT(1)", ftapp );
    if( m_ftapp > ftapp )
    {
        opt.SetValue( "FERAMT(1)", m_ftapp );
        opt2.SetValue( "FERAMT(1)", m_ftside );
    }
    std::cout << m_ftapp.convert< double >() <<
        ftapp.convert< double >() <<std::endl;
    f100.WriteOptions();
    f100.CloseFile();

    std::vector< Poco::Dynamic::Var > var(m_rot.size(),0);

    while( !calibrated )
    {
        if( count > 20 )
        {
            cal += 0.002;
            count = 0;
        }

        yield.clear();

        std::vector< std::string >::const_iterator extitr = file_ext.begin();
        for( ; extitr != file_ext.end(); ++extitr )
        {
            std::string ext = *extitr;
            std::string file( abbr + "_" + ck + ext );
            RemoveAllFilesWithExtension( file );
        }

        std::string runCmd( path.string() + "/bin/Windows/DailyDayCent.exe" );
        std::string lisCmd( path.string() + "/bin/Windows/DailyDayCent_list100.exe" );

        Poco::Process::Args args;
        std::string temp = "CMZ" + m_cmz;
        std::cout << temp << std::endl;
        if( temp.find( "0" ) != std::string::npos )
        {
            temp.erase( temp.find( "0" ), 1 );
        }
        args.push_back( "-s " + abbr );
        args.push_back( "-n " + abbr + "_" + ck);
        args.push_back( "-e " + temp + "_" + abbr + "_" + ck );

        Poco::ProcessHandle ph =
            Poco::Process::launch( runCmd, args, 0, 0, 0 );

        //Wait until child processes exit
        int numSecsToExecute = Poco::Process::wait( ph );

        std::string file( abbr + "_" + ck );
        args.clear();
        args.push_back( file );
        args.push_back( file );
        args.push_back( "outvars.txt" );
        ph = Poco::Process::launch( lisCmd, args, 0, 0, 0 );
        numSecsToExecute = Poco::Process::wait( ph );

        std::vector< double > yc;
        yc = pp.GetResults( abbr, file + ".lis" );

        std::map< std::string, double >::const_iterator itr;
        for( size_t i=0; i<m_rot.size(); ++i )
        {
            std::string curCrop = m_rot.at(i);

            for( itr = crop_conv.begin() ; itr != crop_conv.end(); ++itr )
            {
                if( curCrop.find( (*itr).first ) != std::string::npos )
                {
                    yield.push_back( *(yc.rbegin() + i + 1) / (*itr).second );
                }
            }

            if( curCrop == "CG" )
            {
                y1 = yield.at(i);
            }

            diff.at(i) = (yield.at(i)-m_yield.at(i))/m_yield.at(i);
            std::cout << diff.at(i) << " " << yield.at(i) << " " << m_yield.at(i) << std::endl;
            std::vector< double >::iterator ylditr = diff.begin();
            for( ; ylditr != diff.end(); ++ylditr )
            {
                if( abs((*ylditr)) > cal )
                {
                    calibrated = false;
                    break;
                }
                else
                {
                    calibrated = true;
                }
            }
            if( !calibrated )
            {
                bool fert = false;
                file::F_100&  f100 = *file::F_100::instance();
                assert( f100.OpenFile( CROP, "", abbr ) );
                file::Option const& opt = f100.GetOption( m_rot.at(i) );
                //var.push_back( diff.at(i) );
                opt.GetValue( "PRDX(1)", var.at(i) );
                std::cout << var.at(i).convert< double >() << std::endl;
                //fix for when not calibrating with nitrogen
                if( var.at(i).convert< double >() > 2 )
                {
                    calibrated = true;
                    var.at(i) = 1.25;
                }
                else if( var.at(i).convert< double >() < 0 )
                {
                    calibrated = true;
                    var.at(i) = 0.25;
                }
                if( var.at(i).convert< double >() > 1.5 )
                {
                    var.at(i) = 0.7;
                    if( curCrop == "CG" || curCrop == "CG1" )
                    {
                        fert = true;
                    }
                }
                else
                {
                    if( curCrop == "CG" || curCrop == "CG1" )
                    {
                        std::cout << y1 << "   " << y2 << std::endl;
                        if( y1 > m_yield.at(i) &&
                            y2 < m_yield.at(i) )
                        {
                            if( !loop )
                            {
                                loop = true;
                            }
                            else
                            {
                                var.at(i) = 0.7;
                                loop = false;
                            }
                        }
                    }
                    var.at(i) -= diff.at(i) * 0.2;
                }
                std::cout << var.at(i).convert< double >() << std::endl;
                opt.SetValue( "PRDX(1)", var.at(i) );
                f100.WriteOptions();
                f100.CloseFile();

                if( fert )
                {
                    if( curCrop == "CG" || curCrop == "CG1" )
                    {
                        assert( f100.OpenFile( FERT, "", abbr ) );
                        file::Option const& opt3 = f100.GetOption( "NAPP" );
                        file::Option const& opt4 = f100.GetOption( "NSIDE" );
                        opt3.GetValue( "FERAMT(1)", m_ftapp );
                        opt4.GetValue( "FERAMT(1)", m_ftside );
                        std::cout << m_ftapp.convert< double >() <<
                            m_ftside.convert< double >() <<std::endl;
                        double tmp = m_ftside.convert< double >() -
                            m_ftapp.convert< double >();
                        if(  tmp > 3 )
                        {
                            m_ftapp = (m_ftapp.convert< double >()) + 1;
                        }
                        else
                        {
                             m_ftside = (m_ftside.convert< double >()) + 1;
                        }
                        std::cout << m_ftapp.convert< double >() <<
                            m_ftside.convert< double >() <<std::endl;
                        opt3.SetValue( "FERAMT(1)", m_ftapp );
                        opt4.SetValue( "FERAMT(1)", m_ftside );
                        f100.WriteOptions();
                        f100.CloseFile();

                        fert = false;
                    }
                }
            }
            else
            {
                calibrated = true;
                for( size_t j=0; j<diff.size(); ++j )
                {
                    if( abs( diff.at(j) ) > cal )
                    {
                        std::cout << diff.at(i) << std::endl;
                        calibrated = false;
                        break;
                    }
                }
            }
            if( curCrop == "CG" || curCrop == "CG1" )
            {
                y2 = y1;
            }
        }
        count += 1;
        std::cout << "count: " << count << " " << cal << std::endl;
    }

    if( fs::exists( abbr + "_" + ck + "_0.bin" ) )
    {
        RemoveAllFilesWithExtension( abbr + "_" + ck + "_0.bin" );
    }
    fs::copy_file( abbr + "_" + ck + ".bin", abbr + "_" + ck + "_0.bin" );

    fs::path cp( fs::current_path() );
    fs::current_path( path );

    pp.CreateOutDB( cp.string() + "/year_summary.out" );
    pp.WriteOutFile( cp.string() + "/year_summary.out", abbr, soil,
        soil, tillage, removal, rotation, yldreg );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool YieldCalibration::RunBaseline(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& path)
{
    fs::path cp( fs::current_path() );

    PostProcessor& pp = *( PostProcessor::instance() );
    pp.CreateOutDB( path + "/year_summary.out" );

    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );

    fs::current_path( cp/path );

    std::vector< std::string >::const_iterator extitr = file_ext.begin();
    for( ; extitr != file_ext.end(); ++extitr )
    {
        std::string ext = *extitr;
        std::string file( abbr + "_" + ck + ext );
        RemoveAllFilesWithExtension( file );
    }

    std::string temp = "CMZ" + m_cmz;
    std::cout << temp << std::endl;
    if( temp.find( "0" ) != std::string::npos )
    {
        temp.erase( temp.find( "0" ), 1 );
    }
    std::string runCmd( cp.string() + "/bin/Windows/DailyDayCent.exe" );
    std::string lisCmd( cp.string() + "/bin/Windows/DailyDayCent_list100.exe" );

    Poco::Process::Args args;
    args.push_back( "-s " + abbr );
    args.push_back( "-n " + abbr + "_" + ck );
    args.push_back( "-e " + temp + "_" + abbr + "_" + ck );

    Poco::ProcessHandle ph =
        Poco::Process::launch( runCmd, args, 0, 0, 0 );
    int numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( abbr + "_" + ck );
    args.push_back( abbr + "_" + ck );
    args.push_back( "outvars.txt" );
    ph = Poco::Process::launch( lisCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    fs::current_path( cp );

    pp.WriteOutFile( path + "/year_summary.out", abbr, soil,
        soil, tillage, removal, rotation, yldreg );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool YieldCalibration::BuildDistributedFiles(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& calPath,
    int const& simyr )
{
    fs::path path( fs::current_path() );
    fs::current_path( calPath.c_str() );

    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );

    unsigned int num( simyr/m_rot.size() );

    std::string exefile( abbr + "_" + ck + "_" + boost::lexical_cast< std::string >(num) + ".bin" );
    std::string lisfile( abbr + "_" + ck + ".lis" );
    RemoveAllFilesWithExtension( exefile );
    RemoveAllFilesWithExtension( lisfile );

    std::string runCmd( path.string() + "/bin/Windows/DailyDayCent.exe" );
    std::string lisCmd( path.string() + "/bin/Windows/DailyDayCent_list100.exe" );

    Poco::Process::Args args;
    args.push_back( "-s " + abbr );
    args.push_back( "-n " + abbr + "_" + ck + "_" + boost::lexical_cast< std::string >(num) );
    args.push_back( "-e " + abbr + "_" + ck + "_" + boost::lexical_cast< std::string >(num-1) );

    Poco::ProcessHandle ph =
        Poco::Process::launch( runCmd, args, 0, 0, 0 );

    //Wait until child processes exit
    int numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( abbr + "_" + ck + "_" + boost::lexical_cast< std::string >(num) );
    args.push_back( abbr + "_" + ck );
    args.push_back( "outvars.txt" );
    ph = Poco::Process::launch( lisCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    fs::path cp( fs::current_path() );
    fs::current_path( path );

    PostProcessor& pp = *( PostProcessor::instance() );
    pp.WriteOutFile( cp.string() + "/year_summary.out", abbr, soil,
        soil, tillage, removal, rotation, yldreg );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool YieldCalibration::RunDistributedFiles(
    std::string const& abbr,
    std::string const& cokey,
    std::string const& calPath,
    int const& simyr )
{
    fs::path path( fs::current_path() );
    fs::current_path( calPath.c_str() );

    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );

    unsigned int num( simyr/m_rot.size() );

    std::vector< std::string >::const_iterator extitr = file_ext.begin();
    for( ; extitr != file_ext.end(); ++extitr )
    {
        std::string ext = *extitr;
        std::string file( abbr + "_" + ck + ext );
        RemoveAllFilesWithExtension( file );
    }

    std::string runCmd( path.string() + "/bin/Windows/DailyDayCent.exe" );
    std::string lisCmd( path.string() + "/bin/Windows/DailyDayCent_list100.exe" );

    Poco::Process::Args args;
    args.push_back( "-s " + abbr );
    args.push_back( "-n " + abbr + "_" + ck );
    args.push_back( "-e " + abbr + "_" + ck + "_" + boost::lexical_cast< std::string >(num) );

    Poco::ProcessHandle ph =
        Poco::Process::launch( runCmd, args, 0, 0, 0 );

    //Wait until child processes exit
    int numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( abbr + "_" + ck + "_" + boost::lexical_cast< std::string >(num) );
    args.push_back( abbr + "_" + ck );
    args.push_back( "outvars.txt" );
    ph = Poco::Process::launch( lisCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    fs::path cp( fs::current_path() );
    fs::current_path( path );

    PostProcessor& pp = *( PostProcessor::instance() );
    pp.WriteOutFile( cp.string() + "/year_summary.out", abbr, soil,
        soil, tillage, removal, rotation, yldreg );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetYieldInput( std::vector< double > yield )
{
    std::reverse( yield.begin(), yield.end() );

    m_yield = yield;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::RemoveAllFilesWithExtension( std::string const& ext)
{
    fs::directory_iterator end_itr;
    for( fs::directory_iterator itr(fs::current_path());
         itr != end_itr;
         ++itr )
    {
        if( itr->path().leaf() == ext )
        {
            fs::remove( itr->path().leaf() );
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetCMZ( std::string const& cmz )
{
    m_cmz = cmz;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& YieldCalibration::GetCMZ()
{
    /*std::string cmz;
    fs::directory_iterator end_itr;
    for( fs::directory_iterator itr( fs::current_path() );
         itr != end_itr;
         ++itr )
    {
        std::string tmp = itr->path().leaf();
        if( !tmp.find( "CMZ" ) )
        {
            m_cmz = itr->path().leaf();
            break;
        }
    }*/
    std::cout << m_cmz << std::endl;
    return m_cmz;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var const& YieldCalibration::GetSci() const
{
    return m_sci;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var const& YieldCalibration::GetSciOM() const
{
    return m_sciOM;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var const& YieldCalibration::GetSciER() const
{
    return m_sciER;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var const& YieldCalibration::GetSciFO() const
{
    return m_sciFO;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var const& YieldCalibration::GetWindEros() const
{
    return m_windEros;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var const& YieldCalibration::GetWaterEros() const
{
    return m_waterEros;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var const& YieldCalibration::GetTotBioRem() const
{
    return m_totBioRem;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > YieldCalibration::GetRotation()
{
    //return rotation to original order
    std::reverse( m_rot.begin(), m_rot.end() );
    return m_rot;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetRotation( std::vector< std::string > rot )
{
    //adjust order for how it is read from file
    m_rot = rot;
    std::reverse( m_rot.begin(), m_rot.end() );
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetSci(
    Poco::Dynamic::Var const& sci )
{
    m_sci = sci;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetSciOM(
    Poco::Dynamic::Var const& sciOM )
{
    m_sciOM = sciOM;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetSciER(
    Poco::Dynamic::Var const& sciER )
{
    m_sciER = sciER;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetSciFO(
    Poco::Dynamic::Var const& sciFO )
{
    m_sciFO = sciFO;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetWindEros(
    Poco::Dynamic::Var const& windEros )
{
    m_windEros = windEros;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetWaterEros(
    Poco::Dynamic::Var const& waterEros )
{
    m_waterEros = waterEros;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetTotBioRem(
    Poco::Dynamic::Var const& totBioRem )
{
    m_totBioRem = totBioRem;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetAbbr(
    std::string const& abbr )
{
    m_abbr = abbr;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetMukey(
    std::string const& mukey )
{
    soil = mukey;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetCokey(
    std::string const& cokey )
{
    soil = cokey;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetMuacres(
    std::string const& muacres)
{
    m_muacres = muacres;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetPctAcres(
    std::string const& pctacres )
{
    m_pctacres = pctacres;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetTillage(
    std::string const& tillreg )
{
    tillage = tillreg;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetRemoval(
    std::string const& remreg )
{
    removal = remreg;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetRotation(
    std::string const& rotreg )
{
    rotation = rotreg;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetYield(
    std::string const& yieldreg )
{
    yldreg = yieldreg;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetPRDX(
    std::vector< double > prdx )
{
    m_prdx = prdx;
}
////////////////////////////////////////////////////////////////////////////////
void YieldCalibration::SetField(
    std::string field )
{
    name = field;
}
} //end cal
} //end daycent
} //end wrapper
} //end leaf