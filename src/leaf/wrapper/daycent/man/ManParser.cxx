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
#include "leaf/wrapper/daycent/man/ManParser.h"
#include "leaf/wrapper/daycent/man/Fert.h"
#include "leaf/wrapper/daycent/man/Erosion.h"
#include "leaf/wrapper/daycent/man/Cult.h"
#include "leaf/wrapper/daycent/man/Harv.h"
#include "leaf/wrapper/daycent/man/Crop.h"

// --- Boost Includes --- //
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

namespace fs = boost::filesystem;

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{
////////////////////////////////////////////////////////////////////////////////
std::set< std::string > const ManParser::crops = boost::assign::list_of
    ( "CG" ) ( "CG1" ) ( "WW" ) ( "SB" ) ( "SB1" ) ("RC");
////////////////////////////////////////////////////////////////////////////////
ManParser::ManParser(
    std::string const& filePath,
    std::string const& abbr,
    const int rotYears )
{
    m_path = filePath;
    m_abbr = abbr;
    m_rotYears = rotYears;

    //Populating files from here until we have xml parser for skel files
    //SetFertParams( filePath );

    m_crop["Corn"] = 2;
    m_crop["Alfalfa/oat"] = 1;
    m_crop["Alfalfa/oat(silage)"] = 1;
    m_crop["Alfalfa brome"] = 1;
    m_crop["Alfalfa"] = 1;
    m_crop["Soybean"] = 5;
}
////////////////////////////////////////////////////////////////////////////////
ManParser::~ManParser(void)
{
}
////////////////////////////////////////////////////////////////////////////////
void ManParser::BuildSchFile(
    std::vector< std::vector< std::vector< std::string > > > schedule,
    const int simYears,
    bool const& sim)
{
    std::cout << (2011 + simYears) << std::endl;
    std::cout << (2011 + simYears + m_rotYears - 1) << std::endl;
    std::string file( m_abbr + ".sch" );
    fs::path cp( fs::current_path() );
    std::string filepath = cp.string() + "/" + m_path + "/" + file;
    if( fs::exists( filepath ) )
    {
        fs::remove( filepath.c_str() );
    }
    std::ofstream outputFile;
    std::string sch(m_path);
    sch.append( "/" + file);
    outputFile.open( sch.c_str() );
    if( !sim )
    {
        outputFile << (2011 + simYears) << "        Starting year\n";
        outputFile << (2011 + simYears + m_rotYears - 1) << "        Ending year\n";
    }
    else
    {
        outputFile << (2011 ) << "        Starting year\n";
        outputFile << (2011 + simYears - 1) << "        Ending year\n";
    }
    outputFile << m_abbr + ".100" << "        Site file name\n";
    outputFile << 0 << "        Labeling type\n";
    outputFile << -1 << "        Labeling year\n";
    outputFile << -1 << "        Microcosm\n";
    outputFile << -1 << "        CO2 systems\n";
    //if( co2 )
    //{
    //    outputFile << co2InitialTime << "  " << co2FinalTime;
    //}
    outputFile << -1 << "        pH effect\n";
    //if( pH )
    //{
    //   outputFile << pHShiftTime;
    //}
    outputFile << -1 << "        Soil warming\n";
    //if( soilWarm )
    //{
    //   outputFile << soilWarmingTime << "\n";
    //    outputFile << soilWarmingTemp << "\n";
    //}
    outputFile << 0 << "        N input scalar option\n";
    //if( nitrogenInput != 0 )
    //{
    //    outputFile << nScalarOptionTime << "\n";
    //}
    outputFile << 0 << "        OMAD scalar option\n";
    //if( omadScalar )
    //{
    //    outputFile << omadScalarOptionTime << "\n";
    //}
    outputFile << 0 << "        Climate scalar option\n";
    //if( climateScalar )
    //{
    //    outputFile << climateScalarOptionTime << "\n";
    //}
    outputFile << 1 << "        Initial system\n";
    outputFile << "CG" << "        Initial crop\n";
    outputFile << "           Initial tree\n\n";
    outputFile << "Year Month Option \n";

    outputFile << 1 << "      Block #" << "\n";

    if( !sim )
    {
        outputFile << (2011 + simYears + m_rotYears - 1) <<  "       Last year" << "\n";
        outputFile << m_rotYears << "       Repeats # years" << "\n";
        outputFile << (2011 + simYears) <<  "       Output starting year" << "\n";
    }
    else
    {
        outputFile << (2011 + simYears - 1) <<  "       Last year" << "\n";
        outputFile << m_rotYears << "       Repeats # years" << "\n";
        outputFile << 2011 <<  "       Output starting year" << "\n";
    }
    outputFile << 12 <<  "       Output month" << "\n";
    /*if( outputInterval == 0 )
    {
        blockInfo.push_back( "0.0833  Output interval" );
    }
    else if( outputInterval == 1)
    {
        blockInfo.push_back( "0.500  Output interval" );
    }
    else if( outputInterval == 2)
    {
        blockInfo.push_back( "1.000  Output interval" );
    }*/
    outputFile << "1.000  Output interval" << "\n";
    /*if( numBlocks == 0 )
    {
        blockInfo.push_back( "F Weather choice" );
        blockInfo.push_back( weatherFileName );
    }
    else
    {
        blockInfo.push_back( "C Weather choice" );
    }
    */
    outputFile << "F Weather choice" << "\n";
    outputFile << m_abbr + ".wth" << "\n";
    for( size_t i=0; i<schedule.size(); ++i )
    {
        for( size_t j=0; j<schedule.at(i).size(); ++j )
        {
            outputFile << "   " << schedule.at(i).at(j).at(0) << "   "
                        << schedule.at(i).at(j).at(1) << "   "
                        << schedule.at(i).at(j).at(2) << "\n";
            if( schedule.at(i).at(j).size() > 3 )
            {
                outputFile << schedule.at(i).at(j).at(3) << "\n";
            }
        }
    }
    outputFile << "-999 -999 X" << "\n";

    outputFile.close();

    fs::current_path( cp );
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::vector< std::string > > ManParser::MergeErosionOperations(
    std::vector< std::vector< std::string > > op,
    std::vector< std::vector< std::string > > ero )
{
    std::vector< std::vector< std::string > > schedule(ero);
    std::vector< std::vector< std::string > >::iterator itt;

    for( size_t i=0; i<op.size(); ++i )
    {
        for( itt=schedule.begin(); itt<schedule.end(); ++itt )
        {
            if( boost::lexical_cast< int >(op.at(i).at(1) )
                <= boost::lexical_cast< int >((*itt).at(1)) )
            {
                if( i == 0 )
                {
                    schedule.insert( itt, op.at(i) );
                }
                else if( op.at(i).at(1) == op.at(i-1).at(1) )
                {
                    schedule.insert( itt+1, op.at(i) );
                }
                else
                {
                    schedule.insert( itt, op.at(i) );
                }
                break;
            }
            else if( boost::lexical_cast< int >( op.at(i).at(1) )
                > boost::lexical_cast< int >( schedule.back().at(1) ) )
            {
                schedule.insert( schedule.end(), op.at(i) );
            }
        }
    }

	return schedule;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::vector< std::pair< std::string, std::string > > >
    ManParser::GetAnnualErosion()
{
    return m_erosion;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::vector< std::vector< std::string > > >
    ManParser::AssignOperationType(
    std::string const& tillreg,
    std::vector< std::vector< std::pair< std::string, std::string > > > mgnt )
{
    std::vector< std::vector< std::string > > management;
    std::vector< std::vector< std::vector< std::string > > > op;
    man::Cult cult;
    man::Harv harv;
    man::Fert fert;
    man::Crop crop;

    std::vector< std::string > mancrops;

    for( size_t i=0; i<mgnt.size(); ++i )
    {
        std::string curCrop;
        management.clear();
        std::string curEvent;
        for( size_t j=0; j<mgnt.at(i).size(); ++j )
        {
            std::vector< std::string > temp;
            temp.push_back( boost::lexical_cast< std::string >(i+1) );
            temp.push_back( mgnt.at(i).at(j).first );
            std::string opEvent( mgnt.at(i).at(j).second );

            if( !opEvent.find("Harvest") )//|| !opEvent.find("Bale") )
            {
                temp.push_back( "HARV" );
                std::string harvOp = harv.SetHarvOp( mgnt.at(i).at(j).second, curCrop, m_removalRate, m_abbr );
				temp.push_back( harvOp );
				curEvent = "HARV";
			}
			else if( !opEvent.find("Fert") )
			{
				temp.push_back( "FERT" );
				temp.push_back( fert.SetFertOp( 
                    mgnt.at(i).at(j).second, mgnt.at(i).at(j).first ) );
				curEvent = "FERT";
                if( mgnt.at(i).at(j).second.find( "anhyd" ) != std::string::npos )
                    temp.clear();
			}
			else if( !opEvent.find("Manure") )
			{
				temp.push_back( "OMAD" );
				temp.push_back( fert.SetFertOp( 
                    mgnt.at(i).at(j).second, mgnt.at(i).at(j).first ) );
				curEvent = "OMAD";
			}
			else if( !opEvent.find("Chisel") || !opEvent.find("Cultivator")
				|| !opEvent.find("Disk") || !opEvent.find("Drill") 
				|| !opEvent.find("Planter") || !opEvent.find("Rake") 
				|| !opEvent.find("Plow") || !opEvent.find("Harrow") )
			{
				temp.push_back( "CULT" );
				std::string cultOp = cult.SetCultOp( mgnt.at(i).at(j).second );
				temp.push_back( cultOp );
				curEvent = "CULT";
                management.push_back( temp );
                temp.clear();

                //simulate additional decomposition after initial cultivation
                if( cultOp == "H" )
                {
                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( "330" );
                    temp.push_back( "CULT" );
                    temp.push_back( "H1" );
                    management.push_back( temp );
                    temp.clear();

                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( "360" );
                    temp.push_back( "CULT" );
				    temp.push_back( "H2" );
                    management.push_back( temp );
                    temp.clear();
                }
                if( cultOp == "K" )
                {
                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( "330" );
                    temp.push_back( "CULT" );
                    temp.push_back( "K1" );
                    management.push_back( temp );
                    temp.clear();

                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( "360" );
                    temp.push_back( "CULT" );
				    temp.push_back( "K2" );
                    management.push_back( temp );
                    temp.clear();
                }
			}
            else if( !opEvent.find("Shred") || !opEvent.find("Bale") )
			{
				continue;
			}
            else if( !opEvent.find("Kill crop") )
            {
                temp.push_back( "CULT" );
				temp.push_back( "HERB" );
				curEvent = "CULT";;
            }
			else
			{
				curCrop = opEvent.substr( 0, opEvent.find( "," ) );
                std::string cp = crop.SetCrop( curCrop );
                std::vector< std::string >::iterator cropitr;
                for( cropitr = mancrops.begin(); cropitr != mancrops.end(); ++cropitr )
                {
                    if( cp == *cropitr )
                    {
                        cp.append( "1" );
                    }
                }
                mancrops.push_back( cp );
                temp.push_back( "CROP" );
                temp.push_back( cp );
                curEvent = "CROP";
                management.push_back( temp );
                temp.clear();

                if( tillreg == "NT" )
                {
                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( boost::lexical_cast< std::string >
                        (boost::lexical_cast< double >(mgnt.at(i).at(j).first) + 30 ) );
                    temp.push_back( "CULT" );
                    temp.push_back( "WORM" );
                    management.push_back( temp );
                    temp.clear();
                }
            }

            if( !temp.empty() )
            {
                management.push_back( temp );
            }
            if( !curCrop.empty() && !PerennialCrop( curCrop ) )
            {
                if( !curEvent.compare( "CROP" ) )
                {
                    temp.clear();
                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( mgnt.at(i).at(j).first);
                    temp.push_back( "PLTM" );
                    management.push_back( temp );
                }
                else if( !curEvent.compare( "HARV" ) )
                {
                    temp.clear();
                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    //temp.push_back( mgnt.at(i).at(j).first );
                    temp.push_back( mgnt.at(i).at(j).first );
                    temp.push_back( "LAST" );
                    management.push_back( temp );

                    /*if( curCrop.find( "Corn" ) != std::string::npos )
                    {
                        temp.clear();
                        temp.push_back( boost::lexical_cast< std::string >(i+1) );
                        temp.push_back( mgnt.at(i).at(j).first );
                        temp.push_back( "OMAD" );
                        temp.push_back( "OO16.7" );
                        management.push_back( temp );
                    }*/
                }
            }
            else
            {
                if( !curEvent.compare( "CROP" ) )
                {
                    temp.clear();
                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( mgnt.at(i).at(j).first);
                    temp.push_back( "PLTM" );
                    management.push_back( temp );
                }
                else if( !curEvent.compare( "HARV" ) )
                {
                    temp.clear();
                    temp.push_back( boost::lexical_cast< std::string >(i+1) );
                    temp.push_back( mgnt.at(i).at(j).first);
                    temp.push_back( "LAST" );
                    management.push_back( temp );
                }
            }
        }

        //hack - doesn't really impact overall change in SOC
        /*std::vector< std::vector< std::string > >::iterator it = management.begin();
        std::vector< std::string > tmp;
        tmp.push_back( boost::lexical_cast< std::string >(i+1) );
        tmp.push_back( "60" );
        tmp.push_back( "CULT" );
	    tmp.push_back( "H4" );
        management.insert( it, tmp );
        tmp.clear();

        it = management.begin();
        tmp.push_back( boost::lexical_cast< std::string >(i+1) );
        tmp.push_back( "30" );
        tmp.push_back( "CULT" );
	    tmp.push_back( "H3" );
        management.insert( it, tmp );
        tmp.clear();*/

        op.push_back( management );
    }

    m_rot = mancrops;

    return op;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > ManParser::GetCropAbbrev()
{
    return m_rot;
}
////////////////////////////////////////////////////////////////////////////////
/*bool ManParser::GrowingDegreeDay( int gdd )
{
    if( gdd > 2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}*/
////////////////////////////////////////////////////////////////////////////////
bool ManParser::GrowingDegreeDay( const std::string& crop )
{
    int index;
    std::map< std::string, int >::iterator it;
    for( it=m_crop.begin(); it!=m_crop.end(); ++it )
    {
        if( (*it).first == crop )
        {
            index = (*it).second;
            break;
        }
    }

    if( index > 2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
////////////////////////////////////////////////////////////////////////////////
bool ManParser::PerennialCrop( const std::string &crop )
{
    int index;
    std::map< std::string, int >::iterator it;
    for( it=m_crop.begin(); it!=m_crop.end(); ++it )
    {
        if( (*it).first == crop )
        {
            index = (*it).second;
            break;
        }
    }

    if( index == 1 || index == 3 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
////////////////////////////////////////////////////////////////////////////////
void ManParser::SetFertParams( std::string const& filePath )
{
    double nitrogen( 20 );
    double phosphate( 0 );
    double potassium( 0 );

    Fert fert;
    fert.UpdateFertFile( nitrogen, phosphate, potassium, filePath );
}
////////////////////////////////////////////////////////////////////////////////
void ManParser::SetRemovalRate( std::string const& removalRate )
{
    m_removalRate = removalRate;
}
////////////////////////////////////////////////////////////////////////////////
void ManParser::SetCrop( std::string const& crop )
{
    //m_crop = crop;
}
////////////////////////////////////////////////////////////////////////////////
std::string ManParser::GetCrop( std::vector< std::vector< std::string > > op )
{
    std::string crop;
    std::vector< std::vector< std::string > >::iterator itr = op.begin();

    for( ; itr != op.end(); ++itr )
    {
        for( size_t i=0; i<(*itr).size(); ++i )
        {
            if( crops.find( (*itr).at(i) ) != crops.end() )
            {
                crop = (*itr).at(i);
                break;
            }
        }
    }

    return crop;
}
////////////////////////////////////////////////////////////////////////////////
} //end man
} //end daycent
} //end wrapper
} //end leaf