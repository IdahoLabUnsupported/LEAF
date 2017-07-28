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
#include <leaf/util/SoilHelper.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>

// --- STL Includes --- //
#include <set>
#include <map>
#include <cmath>

namespace leaf
{
namespace util
{

////////////////////////////////////////////////////////////////////////////////
bool lightle::InPalouse(
    std::string const& areasymbol_legend )
{
    static std::set< std::string > const asTable =
        boost::assign::list_of
            ( "ID607" )
            ( "ID610" )
            ( "OR021" )
            ( "OR049" )
            ( "OR055" )
            ( "OR625" )
            ( "OR667" )
            ( "OR670" )
            ( "OR673" )
            ( "WA001" )
            ( "WA021" )
            ( "WA025" )
            ( "WA043" )
            ( "WA063" )
            ( "WA071" )
            ( "WA075" )
            ( "WA603" )
            ( "WA605" )
            ( "WA613" )
            ( "WA617" )
            ( "WA623" )
            ( "WA639" )
            ( "WA676" )
            ( "WA677" );

    std::set< std::string >::const_iterator itr =
        asTable.find( areasymbol_legend );

    return ( itr != asTable.end() );
}
////////////////////////////////////////////////////////////////////////////////
double lightle::SlopeLenInPalouse(
    double const& slope )
{
    if( slope >= 0.0 && slope < 5.5 )
    {
        return 350.0;
    }
    else if( slope >= 5.5 && slope < 10.5 )
    {
        return 275.0;
    }
    else if( slope >= 10.5 && slope < 15.5 )
    {
        return 225.0;
    }
    else if( slope >= 15.5 && slope < 20.5 )
    {
        return 175.0;
    }
    else if( slope >= 20.5 && slope < 25.5 )
    {
        return 150.0;
    }
    else if( slope >= 25.5 && slope < 35.5 )
    {
        return 125.0;
    }
    else
    {
        return 100.0;
    }
}
////////////////////////////////////////////////////////////////////////////////
double lightle::SlopeLen(
    double const& slope,
    bool const& inPalouse )
{
    //Slope should never be negative
    assert( slope >= 0 );

    if( inPalouse ) return SlopeLenInPalouse( slope );

    //( slope, slope length ): slope length is in ft
    static std::map< double, double > const slopeTable =
        boost::assign::map_list_of
            ( 0.5, 100 ) ( 1, 200 ) ( 2, 300 ) ( 3, 200 ) ( 4, 180 )
            ( 5, 160 ) ( 6, 150 ) ( 7, 140 ) ( 8, 130 ) ( 9, 125 )
            ( 10, 120 ) ( 11, 110 ) ( 12, 100 ) ( 13, 90 ) ( 14, 80 )
            ( 15, 70 ) ( 16, 60 ) ( 17, 60 ) ( 18, 50 );

    if( slope >= 0.0 && slope < 0.5 )
    {
        return 100.0;
    }
    else if( slope >= 0.5 && slope < 1.0 )
    {
        //Reduced linear interpolation equation for this range of values
        return slope * 200.0;
    }
    else if( slope >= 1.0 && slope < 18.0 )
    {
        double x( slope );
        std::map< double, double >::const_iterator itr =
            slopeTable.find( floor( x ) );

        assert( itr != slopeTable.end() );
        double xo( itr->first ), yo( itr->second );

        assert( ++itr != slopeTable.end() );
        double x1( itr->first ), y1( itr->second );

        return yo + ( x - xo ) * ( y1 - yo ) / ( x1 - xo );
    }

    return 50.0;
}
////////////////////////////////////////////////////////////////////////////////
double lightle::SlopeLenBuckets(
    double const& slope,
    bool const& inPalouse )
{
    //Slope should never be negative
    assert( slope >= 0 );

    if( inPalouse ) return SlopeLenInPalouse( slope );

    if( slope >= 0.0 && slope < 0.75 )
    {
        return 100.0;
    }
    else if( slope >= 0.75 && slope < 1.5 )
    {
        return 200.0;
    }
    else if( slope >= 1.5 && slope < 2.5 )
    {
        return 300.0;
    }
    else if( slope >= 2.5 && slope < 3.5 )
    {
        return 200.0;
    }
    else if( slope >= 3.5 && slope < 4.5 )
    {
        return 180.0;
    }
    else if( slope >= 4.5 && slope < 5.5 )
    {
        return 160.0;
    }
    else if( slope >= 5.5 && slope < 6.5 )
    {
        return 150.0;
    }
    else if( slope >= 6.5 && slope < 7.5 )
    {
        return 140.0;
    }
    else if( slope >= 7.5 && slope < 8.5 )
    {
        return 130.0;
    }
    else if( slope >= 8.5 && slope < 9.5 )
    {
        return 125.0;
    }
    else if( slope >= 9.5 && slope < 10.5 )
    {
        return 120.0;
    }
    else if( slope >= 10.5 && slope < 11.5 )
    {
        return 110.0;
    }
    else if( slope >= 11.5 && slope < 12.5 )
    {
        return 100.0;
    }
    else if( slope >= 12.5 && slope < 13.5 )
    {
        return 90.0;
    }
    else if( slope >= 13.5 && slope < 14.5 )
    {
        return 80.0;
    }
    else if( slope >= 14.5 && slope < 15.5 )
    {
        return 70.0;
    }
    else if( slope >= 15.5 && slope < 17.5 )
    {
        return 60.0;
    }
    else
    {
        return 50.0;
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end util
} //end leaf
