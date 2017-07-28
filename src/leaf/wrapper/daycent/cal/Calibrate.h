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
#include <leaf/LEAFConfig.h>

#include <leaf/wrapper/daycent/cal/Common.h>

// --- STL Includes --- //
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace cal
{
class Calibrate
{
public:
    ///
    Calibrate(void);

    ///
    ~Calibrate(void);

    ///
    void SetPRDX(
        std::string const& mukey,
        std::string crop,
        double yld,
        double prdx);
    double GetPRDX(
        std::string const& mukey,
        std::string const& crop,
        double yld);

    ///
    bool PRDXExist(
        std::string const& mukey,
        std::string const& crop,
        double yld);

    ///
    double prdx;

};

} //end cal
} //end daycent
} //end wrapper
} //end leaf
