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


#include "Erosion.h"

#include <boost/lexical_cast.hpp>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{
////////////////////////////////////////////////////////////////////////////////
Erosion::Erosion( const int years )
{
    m_rotYears = years;
}
////////////////////////////////////////////////////////////////////////////////
Erosion::~Erosion(void)
{
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::vector< std::vector< std::string > > > Erosion::GetErosion()
{
    return m_erosion;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetErosion( double totalEros )
{
	std::vector< std::string > day;
	//day.push_back("2");
	//day.push_back("33");
	day.push_back("64");
	day.push_back("95");
	day.push_back("125");
	//day.push_back("155");
	//day.push_back("186");
	//day.push_back("216");
	//day.push_back("246");
	day.push_back("276");
	day.push_back("306");
	//day.push_back("332");

	std::vector< double > ero;
    for( size_t i=0; i<day.size(); ++i )
    {
        ero.push_back( totalEros * 0.22417 //ton/arce to kg/m^2
            / boost::lexical_cast< int >(day.size()) );
    }
	/*ero.push_back( 0.000134522 );
	ero.push_back( 0.000955452 );
	ero.push_back( 0.016358763 );
	ero.push_back( 0.047470223 );
	ero.push_back( 0.105942365 );
	ero.push_back( 0.183325035 );
	ero.push_back( 0.191353107 );
	ero.push_back( 0.200205101 );
	ero.push_back( 0.159387137 );
	ero.push_back( 0.019041509 );
	ero.push_back( 0.011753355 );
	ero.push_back( 0.001411444 );*/

    for( size_t i=0; i<m_rotYears; ++i )
    {
        std::vector< std::vector< std::string > > erosion;
        for( size_t j=0; j<day.size(); ++j )
        {
            std::vector< std::string > temp;
            temp.push_back( boost::lexical_cast< std::string >(i+1) );
            temp.push_back( day.at(j) );
            temp.push_back( "EROD" );
            temp.push_back( boost::lexical_cast< std::string >(ero.at(i)) );
            erosion.push_back( temp );
        }
        m_erosion.push_back(erosion);
    }
}
////////////////////////////////////////////////////////////////////////////////
} //end man
} //end daycent
} //end wrapper
} //end leaf
