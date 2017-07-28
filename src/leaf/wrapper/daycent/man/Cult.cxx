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


#include "Cult.h"


namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{
////////////////////////////////////////////////////////////////////////////////
Cult::Cult(void)
{
}
////////////////////////////////////////////////////////////////////////////////
Cult::~Cult(void)
{
}
////////////////////////////////////////////////////////////////////////////////
std::string Cult::SetCultOp( const std::string& cult)
{
    std::string op;

	if( cult.find( "Cultivator" ) != std::string::npos )
	{ op = "D"; }
	else if( cult.find( "Drill" ) != std::string::npos )//|| 
        //cult.find( "double disk" ) != std::string::npos )
    { op = "N"; }
    else if( cult.find( "Chisel" ) != std::string::npos )
    { op = "H"; }
    else if( cult.find( "Planter" ) != std::string::npos )
    { op = "C"; }
    else if( cult.find( "Rake" ) != std::string::npos )
    { op = "E"; }
    else if( cult.find( "moldboard" ) != std::string::npos )
    { op = "K"; }
    else if( cult.find( "Disk" ) != std::string::npos )
	{ op = "D"; }

    return op;
}
} //end man
} //end daycent
} //end wrapper
} //end leaf

