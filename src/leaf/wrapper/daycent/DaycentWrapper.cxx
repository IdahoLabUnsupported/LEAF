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
#include <leaf/wrapper/daycent/DaycentWrapper.h>
#include <leaf/wrapper/daycent/Common.h>
//#include <leaf/wrapper/daycent/PostProcessor.h>
#include <leaf/wrapper/daycent/man/ManParser.h>
#include <leaf/wrapper/daycent/man/XMLParser.h>
#include <leaf/wrapper/daycent/man/Erosion.h>
#include <leaf/wrapper/daycent/man/Fert.h>
#include <leaf/wrapper/daycent/man/Crop.h>

#include <leaf/wrapper/daycent/soil/Soils.h>

#include <leaf/wrapper/daycent/file/F_header.h>

#include <leaf/wrapper/daycent/cal/YieldCalibration.h>

// --- POCO Includes --- //
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>

// --- STL Includes --- //
#include <iomanip>

namespace althm = boost::algorithm;
namespace fs = boost::filesystem;

namespace leaf
{
namespace wrapper
{
namespace daycent
{

////////////////////////////////////////////////////////////////////////////////
DaycentWrapper::DaycentWrapper()
:m_simyr(0)
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DaycentWrapper::~DaycentWrapper()
{
}
////////////////////////////////////////////////////////////////////////////////
DaycentWrapper* DaycentWrapper::instance(
    bool const& erase )
{
    static DaycentWrapper* sDaycentWrapper = new DaycentWrapper();
    if( erase )
    {
        delete sDaycentWrapper;
        sDaycentWrapper = NULL;
    }

    return sDaycentWrapper;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::Initialize()
{
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::Run(
    std::string const& dir,
    std::string const& abbr,
    std::string const& cokey )
{
    fs::path path( fs::current_path() );
    fs::path exePath( path/"bin/Windows/DailyDayCent.exe" );
    fs::path outPath( path/"bin/Windows/DailyDayCent_list100.exe" );
    fs::path simPath( path/dir );
    std::cout << path << std::endl;

    fs::current_path( simPath.string().c_str() );
    std::string runCmd = exePath.string();
    std::string lisCmd = outPath.string();

    GetSpinupFiles(abbr);

    std::cout << abbr << std::endl;

    std::cout << m_spinupFile << std::endl;

    //execute spinups

    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );

    Poco::Process::Args args;
    args.push_back( "-s " + m_spinupFile );
    args.push_back( "-n " + m_spinupFile + "_" + abbr + "_" + ck);

    Poco::ProcessHandle ph =
        Poco::Process::launch( runCmd, args, 0, 0, 0 );

    //Wait until child processes exit
    int numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( "-s " + m_killFile );
    args.push_back( "-n " + m_killFile + "_" + abbr + "_" + ck);
    args.push_back( "-e " + m_spinupFile + "_" + abbr + "_" + ck);
    ph = Poco::Process::launch( runCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( "-s " + m_baseFile );
    args.push_back( "-n " + m_baseFile + "_" + abbr + "_" + ck);
    args.push_back( "-e " + m_killFile + "_" + abbr + "_" + ck);
    ph = Poco::Process::launch( runCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( "-s " + m_cmzFile );
    args.push_back( "-n " + m_cmzFile + "_" + abbr + "_" + ck);
    args.push_back( "-e " + m_baseFile + "_" + abbr + "_" + ck);
    ph = Poco::Process::launch( runCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );
/*
    std::vector< std::string > const m_ext = boost::assign::list_of
        ( ".bin" ) ( ".lis" );
    std::vector< std::string >::const_iterator extitr = m_ext.begin();
    for( ; extitr != m_ext.end(); ++extitr )
    {
        std::string ext = *extitr;
        std::string file( "switchgrass_" + ck + ext );
        RemoveAllFilesWithExtension( file );
    }
*/   
    args.clear();
    //args.push_back( "-s switchgrass_" + ck);
    //args.push_back( "-n switchgrass_" + ck);
    args.push_back( "-s " + abbr + "_" + ck);
    args.push_back( "-n " + abbr + "_" + ck);
    args.push_back( "-e " + m_cmzFile + "_" + abbr + "_" + ck);
    ph = Poco::Process::launch( runCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    //args.push_back( "switchgrass_" + ck);
    //args.push_back( "switchgrass_" + ck);
    args.push_back( abbr + "_" + ck);
    args.push_back( abbr + "_" + ck);
    args.push_back( "outvars.txt" );
    ph = Poco::Process::launch( lisCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    fs::current_path( path.string().c_str() );
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::RunSpinups( 
    std::string const& dir,
    std::string const& abbr,
    std::string const& cokey )
{
    fs::path path( fs::current_path() );
    fs::path exePath( path/"bin/Windows/DailyDayCent.exe" );
    fs::path outPath( path/"bin/Windows/DailyDayCent_list100.exe" );
    fs::path simPath( path/dir );
    std::cout << path << std::endl;
    
    fs::current_path( simPath.string().c_str() );
    std::string runCmd = exePath.string();
    std::string lisCmd = outPath.string();

    GetSpinupFiles(abbr);

    std::cout << abbr << std::endl;

    std::cout << m_spinupFile << std::endl;

    //execute spinups
    std::string ck = cokey; ck.replace( ck.find( ":" ), 1, "-" );

    Poco::Process::Args args;
    args.push_back( "-s " + m_spinupFile );
    args.push_back( "-n " + m_spinupFile + "_" + abbr + "_" + ck);

    Poco::ProcessHandle ph =
        Poco::Process::launch( runCmd, args, 0, 0, 0 );

    //Wait until child processes exit
    int numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( "-s " + m_killFile );
    args.push_back( "-n " + m_killFile + "_" + abbr + "_" + ck);
    args.push_back( "-e " + m_spinupFile + "_" + abbr + "_" + ck);
    ph = Poco::Process::launch( runCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( "-s " + m_baseFile );
    args.push_back( "-n " + m_baseFile + "_" + abbr + "_" + ck);
    args.push_back( "-e " + m_killFile + "_" + abbr + "_" + ck);
    ph = Poco::Process::launch( runCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    args.clear();
    args.push_back( "-s " + m_cmzFile );
    args.push_back( "-n " + m_cmzFile + "_" + abbr + "_" + ck);
    args.push_back( "-e " + m_baseFile + "_" + abbr + "_" + ck);
    ph = Poco::Process::launch( runCmd, args, 0, 0, 0 );
    numSecsToExecute = Poco::Process::wait( ph );

    fs::current_path( path.string().c_str() );
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::CreateWeatherFile(
    std::string const& abbr,
    std::string const& wthDC,
    std::string const& cliPath )
{
    fs::path filename(wthDC  + ".wth");//+ "/" + abbr

    std::string day("");
    std::string month("");
    std::string year("");
    std::string testYear("");
    unsigned int dayOfYear( 0 );
    //std::string prec("");
    double prec( 0 );
    double maxTemp( 0 );
    double minTemp( 0 );
    double rad( 0 );
    double wind( 0 );
    std::string line("");

    //if( !fs::exists( filename ) )
    {
        int lineNum = 1;
        std::ifstream myfile;
        std::ofstream outfile ( filename.string().c_str() );
        myfile.open ( cliPath.c_str(), std::ios::out );

        typedef althm::split_iterator< std::string::iterator > string_split_iterator;

        while( getline( myfile, line ) )
        {
            std::vector< std::string > curline;
            althm::trim( line );
            if( lineNum == 5 )
            {
                    for( string_split_iterator It = althm::make_split_iterator(line,
                        althm::first_finder(" ", althm::is_equal() ) );
                        It!= string_split_iterator();
                        ++It)
                    {
                        if( boost::copy_range<std::string>(*It).compare("") )
                        {
                            curline.push_back( boost::copy_range<std::string>(*It) );
                        }
                    }

                    m_latitude = curline.at(0);
                    m_longitude = curline.at(1);
            }
            if( lineNum > 15 )
            {
                if( line != "" )
                {
                    //breaks string into vector
                    for( string_split_iterator It = althm::make_split_iterator(line,
                        althm::first_finder(" ", althm::is_equal() ) );
                        It!= string_split_iterator();
                        ++It)
                    {
                        if( boost::copy_range<std::string>(*It).compare("") )
                        {
                            curline.push_back( boost::copy_range<std::string>(*It) );
                        }
                    }
                    day = curline.at(0);
                    month = curline.at(1);
                    year = curline.at(2);
                    if( year == "100" )
                    {
                        break;
                    }
                    prec = atof( curline.at(3).c_str() ) * 0.1; //convert from mm to cm
                    maxTemp = atof( curline.at(7).c_str() );
                    minTemp = atof( curline.at(8).c_str() );
                    rad = atof( curline.at(9).c_str() );
                    wind = atof( curline.at(10).c_str() ) * 2.237; //convert from m/s to mph
                    if( year.compare( testYear ) )
                    {
                        dayOfYear = 1;
                    }
                    else
                    {
                        dayOfYear++;
                    }
                    outfile << std::setw( 2 ) << day;
                    outfile << std::setw( 7 ) << month;
                    outfile << std::setw( 7 ) << year;
                    outfile << std::setw( 7 ) << dayOfYear;
                    outfile << std::setw( 9 ) << std::fixed << std::setprecision( 2 ) << maxTemp;
                    outfile << std::setw( 9 ) << std::fixed << std::setprecision( 2 ) << minTemp;
                    outfile << std::setw( 9 ) << std::fixed << std::setprecision( 2 ) << prec;
                    outfile << std::setw( 9 ) << std::fixed << std::setprecision( 2 ) << rad;
                    outfile << std::setw( 9 ) << std::fixed << std::setprecision( 2 ) << -99.90;//relative humidity
                    outfile << std::setw( 9 ) << std::fixed << std::setprecision( 2 ) << wind << std::endl;

                    testYear = year;
                }
            }

            lineNum++;
        }
        outfile.close();
        myfile.close();

    }
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::UpdateSiteFile(
    std::string const& abbr,
    std::string const& sitefile,
    std::string const& orgsitefile)
{
    std::vector< std::pair< std::string, std::string > >
        latLong( GetLatLong( abbr ) );
    std::vector< std::pair< std::string, std::string > >::iterator
        it = latLong.begin();

    std::cout << (*it).first << " " << (*it).second << std::endl;

    fs::path path( fs::current_path() );
    std::string test( path.string() );
    std::cout << test << std::endl;

    std::cout << sitefile << std::endl;
    std::cout << orgsitefile << std::endl;

    file::F_100& f100 = *file::F_100::instance();
    assert( f100.OpenFile( SITE, sitefile, abbr ) );
    file::Option const& opt = f100.GetOption();
    opt.SetValue( "SITLAT", (*it).first );
    opt.SetValue( "SITLNG", (*it).second );
    opt.SetValue( "NLAYER", m_nlayer );
    //unsigned int adep =
    //    std::distance( ADEP.begin(), ADEP.find( layer.GetMaxDepth() ) ) + 1;
    //opt.SetValue( "NLAYER", adep );*/
    //Use first layer to set these values (probably not needed at all)
    //Layers::iterator litr = layers.begin();
    opt.SetValue( "SAND", m_sand * 0.01 );
    opt.SetValue( "SILT", m_silt * 0.01 );
    opt.SetValue( "CLAY", m_clay * 0.01 );
    opt.SetValue( "BULKD", m_bulkden );
    f100.WriteOptions();
    f100.CloseFile();

    std::string site( sitefile + "/" + abbr );
    weather( (char*)(site.c_str()), (char*)(site.c_str()) );
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::CreateMgntFiles(
    std::string const& wepsdir,
    std::string const& fileDir,
    std::string const& abbr,
    const int simyears,
    bool const& sim )
{
    m_projFile = abbr;

    std::string configFile = wepsdir;
/*    std::string configFile="C:\\Pioneer\\skel\\";
    //std::string configFile="S:\\daycent\\CMZ\\";
    if(m_rotation == "CG,SB" )
        m_rotation = "SB,CG";
    configFile.append( m_field + "_[" + m_rotation + "]_" + m_tillage + "_" + m_removalRate + ".man.skel" );// 04_CG,SB_RT_NRH.skel";
*/    
    configFile.append( "\\" + m_cmz + "_" + m_rotation + "_" + m_tillage + "_" + m_removalRate + ".skel" );
    //configFile.append( "\\" + m_cmz + "_" + m_rotation + "_" + m_tillage + "_" + "MRH" + ".skel" );
//    configFile.append( "\\CG,RCSB-CH+FC,NT-DB25-CMZ04.skel" );
    //configFile.append( "\\Switchgrass,FilterScenario,RT,CMZ4.skel" );

    std::cout << configFile << std::endl;
    //std::string configFile="C:\\integrated-analysis\\trunk\\src\\apps\\man.jar\\01_CG,SB_RT_MHH.skel";
    //std::string configFile="E:\\Bioenergy\\AgronomicModels\\DayCent\\ResidueRemoval\\MonsantoWork\\Monsanto\\CarbonBudget\\CMZ04-CG,SB-NT-NRH-100rye-NVB.skel";

    if( !fs::exists( configFile ) )
    {
        return;
    }

    man::XMLParser appConfig;

    appConfig.readConfigFile(configFile);

    std::vector< std::vector< std::pair< std::string, std::string > > > mgnt;
    mgnt = appConfig.GetManagement();

    std::reverse( mgnt.begin(), mgnt.end() );

    int rotYears = appConfig.GetRotYears();

    if( m_simyr < 20 )
    {
        m_simyr += rotYears;
    }
    else
    {
        m_simyr = 0;
    }
    std::cout << m_simyr << std::endl;

    man::ManParser manParser( fileDir, abbr, rotYears );
    manParser.SetRemovalRate( m_removalRate );

    std::vector< std::vector< std::vector< std::string > > > op;
    op = manParser.AssignOperationType( m_tillage, mgnt );

    std::vector< std::string > rotation;
    rotation = manParser.GetCropAbbrev();
    cal::YieldCalibration& yc = *( cal::YieldCalibration::instance() );
    yc.SetRotation( rotation );

    man::Erosion erosion( rotYears );
    double totalEros = GetWindEros() + GetWaterEros();
    erosion.SetErosion( totalEros );
    std::vector< std::vector< std::vector< std::string > > > ero;
    ero = erosion.GetErosion();

    man::Fert fert;
    std::vector< std::vector< std::string > > ft;

    assert( op.size() == ero.size() );

    std::vector< std::vector< std::vector< std::string > > > schedule;
    for( size_t i=0; i<rotYears; ++i )
    {
        std::string crop = manParser.GetCrop( op.at(i) );
        if( crop == "CG" || crop == "CG1" )
        {
            fs::path cp( fs::current_path() );
            fs::current_path(cp/fileDir);
//            fert.UpdateFert( m_yield.at(i), abbr );
            fs::current_path( cp );
        }

        ft = fert.CreateFertEvent( i, crop );

        std::vector< std::vector< std::string > > yearSchFert;
        std::vector< std::vector< std::string > > yearSchEro;
        if( !ft.empty() )
        {
            yearSchFert = manParser.MergeErosionOperations( ft, op.at(i) );
            if( boost::lexical_cast< double >( ero.at(i).back().at(1) ) > 
                boost::lexical_cast< double >( yearSchFert.back().at(1) ) )
            {
                yearSchEro = manParser.MergeErosionOperations( yearSchFert, ero.at(i) );
            }
            else
            {
                yearSchEro = manParser.MergeErosionOperations( ero.at(i), yearSchFert );
            }
        }
        else
        {
            if( boost::lexical_cast< double >( ero.at(i).back().at(1) ) > 
                boost::lexical_cast< double >( op.at(i).back().at(1) ) )
            {
                yearSchEro = manParser.MergeErosionOperations( op.at(i),  ero.at(i) );
            }
            else
            {
                yearSchEro = manParser.MergeErosionOperations( ero.at(i), op.at(i) );                
            }
        }

        schedule.push_back( yearSchEro );        
    }

    SetSchedule( schedule );
    //interval harvest scenario
/*    std::vector< std::vector< std::string > > temp;
    
    //std::vector< std::vector< std::vector< std::string > > >::iterator schitr = schedule.begin()+1;
    //for( ; schitr != schedule.end(); ++schitr )
    temp = schedule.at(0);
    if( m_simyr % 3 != 0 )
    {
        //std::vector< std::vector< std::string > >::iterator tmpitr = temp.begin();
        //for( ; tmpitr != temp.end(); ++tmpitr )
        for( size_t i=0; i<temp.size(); ++i )
        {
            if( temp.at(i).at(2) == "HARV" )
            {
                temp.at(i).at(3) = "G";
            }
        }
    }

    schedule.at(0) = temp;
*/
    ////////interval harvest scenario
    if( sim == false )
    {
        manParser.BuildSchFile( schedule, simyears );
    }
    else
    {
        manParser.BuildSchFile( schedule, simyears, sim );
    }
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::CopyFileDir(
    std::string const& filePath,
    std::string const& fileDir,
    bool const& overwrite )
{
    m_curDir = fileDir;
    std::string fp = filePath + "/simfiles";

    try
    {
        if(!fs::exists( fp ) || !fs::is_directory( fp ))
        {
            std::cerr << "Source directory " << fp.c_str()
                << " does not exist or is not a directory." << std::endl;
            fs::create_directory( fileDir );
        }
        if(fs::exists( fileDir ))
        {
            std::cerr << "Destination directory " << fileDir.c_str()
                << " already exists." << std::endl;
        }

        for( fs::directory_iterator it( fp ); it != fs::directory_iterator(); it++ )
        {
            try
            {
                fs::path current( it->path() );
                if( fs::is_directory( current ) )
                {
                    // Found directory: Recursion
                    CopyFileDir( current.string(),
                        fileDir + "/" + current.filename().string() );
                }
                else
                {
                    if( !fs::exists(
                        fs::path( fileDir + "/" + current.filename().string() ) ) )
                    {
                        // Found file: Copy
                        fs::copy_file(current,
                            fs::path( fileDir + "/" + current.filename().string() ));
                    }
                }
            }
            catch( fs::filesystem_error& e )
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
    catch( fs::filesystem_error& e )
    {
        std::cerr << e.what() << std::endl;
        return;
    }
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::GetSpinupFiles(
    std::string const& filePath,
    std::string const& fileDir,
    std::string const& abbr )
{
    std::vector< std::string > fp;
    fp.push_back( filePath + "/spinups" );
    fp.push_back( filePath + "/cmz" );
    std::map< std::string, std::vector< std::string > > m_spinupFiles = GetSpinups( abbr );
    std::map< std::string, std::vector< std::string > >::iterator it = m_spinupFiles.begin();

    std::vector< std::string > cmz;
    std::vector< std::string > file;
    std::vector< std::string > veg;

    std::vector< std::string > spinupFiles;

    //Get spinup file names
    for( ; it!=m_spinupFiles.end(); ++it )
    {
        if( (*it).first == "CMZ" )
        {
            cmz = (*it).second;
            for( size_t i=0; i<cmz.size(); ++i )
            {
                std::string file( "CMZ" + cmz.at(i) + ".sch" );
                spinupFiles.push_back( file );
                m_cmzFile = file.substr( 0, file.find(".") );;
            }
        }
        else if( (*it).first == "FILE" )
        {
            file = (*it).second;
            for( size_t i=0; i<cmz.size(); ++i )
            {
                std::string file( file.at(i) + "_base.sch" );
                spinupFiles.push_back( file );
                m_baseFile = file.substr( 0, file.find(".") );;
            }
        }
        else if( (*it).first == "VEG" )
        {
            veg = (*it).second;
            for( size_t i=0; i<cmz.size(); ++i )
            {
                std::string file( "lu" + veg.at(i) + ".sch" );
                std::string killFile( "lu" + veg.at(i) + "_kill.sch" );
                spinupFiles.push_back( file );
                spinupFiles.push_back( killFile );
                m_spinupFile = file.substr( 0, file.find(".") );
                m_killFile = killFile.substr( 0, killFile.find(".") );
            }
        }
    }
    std::cout << m_cmzFile << " " << m_baseFile << " " << m_spinupFile << " " << m_killFile << std::endl;

    for( size_t i=0; i<spinupFiles.size(); ++i )
    {
        for( size_t j=0; j<fp.size(); ++j )
        {
            try
            {
                for( fs::directory_iterator it( fp.at(j) ); it != fs::directory_iterator(); it++ )
                {
                    try
                    {
                        fs::path current( it->path() );
                        if( /*!fs::exists( fs::path( fileDir + "/" + spinupFiles.at(i) ) ) &&*/
                            current.filename() == spinupFiles.at(i) )
                        {
                            fs::path fd( fileDir + "/" + spinupFiles.at(i) );

                            std::string line;
                            std::ifstream myfile;
                            std::ofstream outfile ( fd.string().c_str() );
                            if( fs::exists( current ) )
                            {
                                myfile.open( current.string().c_str(), std::ios::out );

                                while( getline( myfile, line ) )
                                {
                                    if( line.find( ".wth" ) != std::string::npos )
                                    {
                                        outfile << abbr + ".wth" << '\n';
                                    }
                                    else if( line.find( ".100" ) != std::string::npos )
                                    {
                                        outfile << abbr + ".100" << '\n';
                                    }
                                    else
                                    {
                                        outfile << line << '\n';
                                    }
                                }
                                outfile.close();
                                myfile.close();
                            }
                        }
                    }
                    catch( fs::filesystem_error& e )
                    {
                        std::cerr << e.what() << std::endl;
                    }
                }
            }
            catch( fs::filesystem_error& e )
            {
                std::cerr << e.what() << std::endl;
                return;
            }
        }
    }

    man::Crop crop;
    assert( crop.BuildCropFile( crop.GetCrops( spinupFiles, fileDir ) ) );

}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::GetSpinupFiles(
    std::string const& abbr )
{
    std::map< std::string, std::vector< std::string > > m_spinupFiles = GetSpinups( abbr );
    std::map< std::string, std::vector< std::string > >::iterator it = m_spinupFiles.begin();

    std::vector< std::string > cmz;
    std::vector< std::string > file;
    std::vector< std::string > veg;

    std::vector< std::string > spinupFiles;

    //Get spinup file names
    for( ; it!=m_spinupFiles.end(); ++it )
    {
        if( (*it).first == "CMZ" )
        {
            cmz = (*it).second;
            for( size_t i=0; i<cmz.size(); ++i )
            {
                std::string file( "CMZ" + cmz.at(i) + ".sch" );
                spinupFiles.push_back( file );
                m_cmzFile = file.substr( 0, file.find(".") );;
            }
        }
        else if( (*it).first == "FILE" )
        {
            file = (*it).second;
            for( size_t i=0; i<cmz.size(); ++i )
            {
                std::string file( file.at(i) + "_base.sch" );
                spinupFiles.push_back( file );
                m_baseFile = file.substr( 0, file.find(".") );;
            }
        }
        else if( (*it).first == "VEG" )
        {
            veg = (*it).second;
            for( size_t i=0; i<cmz.size(); ++i )
            {
                std::string file( "lu" + veg.at(i) + ".sch" );
                std::string killFile( "lu" + veg.at(i) + "_kill.sch" );
                spinupFiles.push_back( file );
                spinupFiles.push_back( killFile );
                m_spinupFile = file.substr( 0, file.find(".") );
                m_killFile = killFile.substr( 0, killFile.find(".") );
            }
        }
    }
    std::cout << m_cmzFile << " " << m_baseFile << " " << m_spinupFile << " " << m_killFile << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetWindEros( double windEros )
{
    m_windEros = windEros;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetWaterEros( double waterEros )
{
    m_waterEros = waterEros;
}
////////////////////////////////////////////////////////////////////////////////
double DaycentWrapper::GetWindEros()
{
    return m_windEros;
}
////////////////////////////////////////////////////////////////////////////////
double DaycentWrapper::GetWaterEros()
{
    return m_waterEros;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetRemovalRate( std::string const& removalRate )
{
    m_removalRate =  removalRate;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetRotation( std::string const& rotation )
{
    m_rotation =  rotation;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetTillage( std::string const& tillage )
{
    m_tillage =  tillage;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetNLayer( int const& layer )
{
    m_nlayer = layer;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetSilt( double const& silt )
{
    m_silt = silt;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetSand( double const& sand )
{
    m_sand = sand;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetClay( double const& clay )
{
    m_clay = clay;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetBulkDensity( double const& bulkden )
{
    m_bulkden = bulkden;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetInputYield( std::vector< double > yield )
{
    m_yield.clear();
    m_yield = yield;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetCMZ( std::string const& cmz )
{
    m_cmz =  cmz;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& DaycentWrapper::GetCMZ()
{
    return m_cmz;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetField( std::string field )
{
    m_field = field;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetFert( 
              std::string const& abbr,
              std::string const& rot,
              std::string const& fert,
              std::string const& path )
{
    man::Fert& ft = *( man::Fert::instance() );
    ft.SetFert( abbr, rot, fert, path );
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetSchedule(
    SCHEDULE schedule )
{
    m_schedule = schedule;
}
////////////////////////////////////////////////////////////////////////////////
SCHEDULE DaycentWrapper::GetSchedule()
{
    return m_schedule;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::BuildSchFile(
    SCHEDULE schedule,
    std::string const& abbr,
    const int simyr,
    bool run )
{
    man::ManParser manParser( "daycent", abbr, schedule.size() );
    manParser.BuildSchFile( schedule, simyr, run );    
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::RemoveAllFilesWithExtension( std::string const& ext)
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
/*////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > DaycentWrapper::GetCropRotation()
{
    return m_rot;
}
////////////////////////////////////////////////////////////////////////////////
void DaycentWrapper::SetCropRotation( std::vector< std::string > rot )
{
    m_rot =  rot;
}*/
////////////////////////////////////////////////////////////////////////////////
} //end daycent
} //end wrapper
} //end leaf