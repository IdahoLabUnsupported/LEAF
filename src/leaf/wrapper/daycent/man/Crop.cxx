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
#include <leaf/wrapper/daycent/man/Crop.h>
#include <leaf/wrapper/daycent/man/Common.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

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
Crop::Crop(void)
{
}
////////////////////////////////////////////////////////////////////////////////

Crop::~Crop(void)
{
}
////////////////////////////////////////////////////////////////////////////////
std::string Crop::SetCrop( const std::string& crop)
{
    std::string op;

    if( crop.find( "Corn" ) != std::string::npos )
    {   op = "CG";}
    else if( crop.find( "Soybean" ) != std::string::npos )
    {   op = "SB";}
    else if( crop.find( "Rye" ) != std::string::npos )
    {   op = "HV";}
    else if( crop.find( "Switchgrass" ) != std::string::npos )
    {   op = "SG3";}

    return op;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > Crop::GetCrops( 
    std::vector< std::string > files, 
    const std::string& path )
{
    std::vector< std::string > crops;

    for( size_t i=0; i<files.size(); ++i )
    {
        fs::path current( path );
        if( fs::exists( fs::path( path + "/" + files.at(i) ) ) )
        {
            fs::path fd( path + "/" + files.at(i) );

            std::string line;
            std::ifstream myfile;
            bool cp = false;
            if( fs::exists( current ) )
            {
                myfile.open( fd.string().c_str(), std::ios::out );

                while( getline( myfile, line ) )
                {
                    if( cp )
                    {
                        crops.push_back( line );
                        cp = false;
                    }
                    if( line.find( "CROP" ) != std::string::npos )
                    {
                        cp = true; 
                    }                        
                }
                myfile.close();
            }
        }
    }

    return crops;
}
////////////////////////////////////////////////////////////////////////////////
bool Crop::BuildCropFile( std::vector< std::string > crops )
{
    std::sort( crops.begin(), crops.end() );
    crops.erase( std::unique( crops.begin(), crops.end() ), crops.end() );

    std::pair< std::string, std::string > cropParams;

    //std::vector< std::string > paramNames = GetCropParamNames();

    //std::vector< std::string > vals = GetCropParams( crops );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
/*std::vector< std::string > Crop::GetCropParams()
{
    std::vector< std::string > params = GetCropParams();    

    std::cout << params.at(0) << std::endl;
    
    return params;
}*/
} //end man
} //end daycent
} //end wrapper
} //end leaf
