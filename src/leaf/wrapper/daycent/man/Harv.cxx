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
#include "leaf/wrapper/daycent/man/Harv.h"

#include "leaf/wrapper/daycent/file/f_header.h"

// --- Boost Includes --- //
#include <boost/filesystem.hpp>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

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
std::set< std::string > const Harv::res_crops = boost::assign::list_of
    ( "Corn" ) ( "Wheat" );
////////////////////////////////////////////////////////////////////////////////
Harv::Harv(void)
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Harv::~Harv(void)
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
std::string Harv::SetHarvOp( 
    std::string const& harv,
    std::string const& crop,
    std::string const& remrate,
    std::string const& abbr )
{
    std::string op;
    std::string cp(crop);
    boost::trim( cp );

    std::set< std::string >::const_iterator itr = res_crops.begin();

    bool exists = true;
    for( ; itr != res_crops.end(); ++itr )
    {
        if( cp == *itr )
        {
            exists = true;
        }
    }

    bool isCrop = res_crops.find( cp ) != res_crops.end();

    if( harv.find( "Harvest" ) != std::string::npos && isCrop )
	{
        if( remrate == "NRH" )
		{ op = "G"; }
		else if( remrate == "HGC" )
		{ op = "G22"; }
		else if( remrate == "MRH" )
		{ op = "G35"; }
		else if( remrate == "MHH" )
		{ op = "G52"; }
		else if( remrate == "HRH" )
		{ op = "G83"; }
        else
        { 
            op = "GRES"; 
            std::string path( "daycent" );
            fs::path curpath( fs::current_path() );
            fs::current_path( curpath/path );

            file::F_100& f100 = *file::F_100::instance();
            assert( f100.OpenFile( HARV, "", abbr ) );
            file::Option const& opt = f100.GetOption( "GRES" );
            opt.SetValue( "RMVSTR", remrate );
            f100.WriteOptions();
            f100.CloseFile();

            fs::current_path( curpath );
        }
	}
    else if( harv.find( "Harvest" ) != std::string::npos && cp == "SG3" )
    {
         op = "SGH";
    }
    else
    {
        op = "G";
    }
	/*else if( harv.find( "Bale" ) != std::string::npos )
	{ 
		op = "B"; 
	}*/

    return op;
}
////////////////////////////////////////////////////////////////////////////////
void Harv::UpdateHarvFile(
    std::string const& filePath,
    std::vector< double > cultparams )
{
    std::string line;
    std::ofstream outfile;
    if( fs::exists( filePath ) )
    {
        fs::path file( filePath + "/harv.100" );
        if( !( fs::exists( file ) ) )
        {
            outfile.open( file.string().c_str() );
        }
        else
        {
            outfile.open( file.string().c_str(), std::ios::app );
        }

        outfile << "G     GRAIN_with_no_straw_removal" << std::endl;
        outfile << "0.0               'AGLREM'" << std::endl;
        outfile << "0.0               'BGLREM'" << std::endl;
        outfile << "1.0               'FLGHRV'" << std::endl;
        outfile << "0.0               'RMVSTR'" << std::endl;
        outfile << "0.2               'REMWSD'" << std::endl;
        outfile << "0.0               'HIBG'" << std::endl;
    }
    outfile.close();
}
////////////////////////////////////////////////////////////////////////////////
} //end man
} //end daycent
} //end wrapper
} //end leaf