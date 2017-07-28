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


#ifndef EROSION_H
#define EROSION_H

#include <string>
#include <vector>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{

class Erosion
{
public:
    ///
    Erosion( const int years );

    ///
    ~Erosion(void);

    ///
    std::vector< std::vector< std::vector<
        std::string > > > GetErosion();

    /// 
    void SetErosion( double totalEros );

    ///
    int m_rotYears;

private:
    

	///
	std::vector< std::vector< std::vector< std::string > > > m_erosion;
};

} //end man
} //end daycent
} //end wrapper
} //end leaf

#endif
