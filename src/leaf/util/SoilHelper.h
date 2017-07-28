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

// --- Boost Includes --- //
#include <boost/noncopyable.hpp>

// --- STL Includes --- //
#include <string>

namespace leaf
{
namespace util
{

///
enum SoilError
{
    SOIERR_NONE = 0x0,
    SOIERR_MISCAREA = 1,
    SOIERR_HORIZON = 2,
    SOIERR_MINERAL = 3,
    SOIERR_SANDSILTCLAY = 4,
    SOIERR_SLOPE = 5,
    SOIERR_SLOPELEN = 6,
    SOIERR_ORGANICTEST = 7,
    SOIERR_ORGLAYEXCIGNDEP = 8,
    SOIERR_SOILAYBELIGNORGLAYISORG = 9,
    SOIERR_NAN = 99

};

///Using class instead of namespace for SWIG
class LEAF_UTIL_EXPORTS lightle : public boost::noncopyable
{
public:
    ///
    static bool InPalouse(
        std::string const& areasymbol_legend );

    ///Buckets
    static double SlopeLenInPalouse(
        double const& slope );

    ///Linear Interpolation
    static double SlopeLen(
        double const& slope,
        bool const& inPalouse = false );

    ///Buckets
    static double SlopeLenBuckets(
        double const& slope,
        bool const& inPalouse = false );

protected:

private:
    ///
    lightle();

    ///
    ~lightle();

};

} //end util
} //end leaf
