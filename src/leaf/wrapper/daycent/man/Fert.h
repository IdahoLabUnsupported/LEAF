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


#pragma once

// --- LEAF Includes --- //

// --- STL Includes --- //
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{

class Fert
{
public:
    ///
    Fert(void);

    ///
    ~Fert(void);

    ///
    static Fert* instance(
        bool const& erase = false );

    ///
    void UpdateFertFile(
        double const& nitrogen,
        double const& phosphate,
        double const& potassium,
        std::string const& filePath );

    ///
    void UpdateFert(
        double const& yield,
        std::string const& abbr );

    ///
    void SetFert( 
        std::string const& abbr,
        std::string const& rot,
        std::string const& fert,
        std::string const& path );

    //
    std::string SetFertOp( 
        std::string fert,
        std::string day );

    /// temporary until .skel files contain fert events
    std::vector< std::vector< std::string > >
        CreateFertEvent( 
        const int year,
        std::string const& crop );

    ///
    static std::set< std::string > const fert_crops;

};

} //end man
} //end daycent
} //end wrapper
} //end leaf
