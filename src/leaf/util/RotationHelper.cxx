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
#include <leaf/util/RotationHelper.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace leaf
{
namespace util
{

////////////////////////////////////////////////////////////////////////////////
unsigned int Year::UINT(
    std::string const& d )
{
    return boost::lexical_cast< unsigned int >(
        d.substr( d.rfind( "/" ) + 1 ) );
}
////////////////////////////////////////////////////////////////////////////////
Year::Enum Year::ENUM(
    std::string const& d )
{
    unsigned int year = Year::UINT( d );
    return static_cast< Year::Enum >( year );
}

////////////////////////////////////////////////////////////////////////////////
std::string Year::Set(
    unsigned int const& y,
    std::string const& d )
{
    std::string newYear = boost::lexical_cast< std::string >( y );
    std::string newDate = d;

    newDate.replace( newDate.rfind( "/" ) + 1, std::string::npos, newYear );
    return newDate;
}
////////////////////////////////////////////////////////////////////////////////
Crop::Type const& Crop::ABBR_TO_CROP(
    std::string const& s )
{
    static std::map< std::string, Crop::Type > const s_cropTypeMap =
        boost::assign::map_list_of
            ( "FA", Crop::NONE )
            ( "BA", Crop::BARLEY )
            ( "DB", Crop::BEAN )
            ( "CA", Crop::CANOLA )
            ( "CG", Crop::CORN )
            ( "SC", Crop::CORN_SWEET )
            ( "CT", Crop::COTTON )
            ( "CC", Crop::COVER_WHEAT_WINTER )
            ( "FX", Crop::FLAX )
            ( "LE", Crop::LENTILS )
            ( "LT", Crop::LETTUCE )
            ( "ML", Crop::MILLET )
            ( "OT", Crop::OATS )
            ( "ON", Crop::ONION )
            ( "PN", Crop::PEANUT )
            ( "PE", Crop::PEAS )
            ( "PT", Crop::POTATO )
            ( "RI", Crop::RICE )
            ( "RY", Crop::RYE )
            ( "SL", Crop::SAFFLOWER )
            ( "FS", Crop::SORGHUM_FORAGE )
            ( "SG", Crop::SORGHUM )
            ( "SB", Crop::SOYBEAN )
            ( "SU", Crop::SUGARBEET )
            ( "SE", Crop::SUGARCANE )
            ( "SF", Crop::SUNFLOWER )
            ( "SWG", Crop::SWITCHGRASS )
            ( "TM", Crop::TOMATO )
            ( "DW", Crop::WHEAT_DURUM )
            ( "SW", Crop::WHEAT_SPRING )
            ( "WW", Crop::WHEAT_WINTER );

    std::map< std::string, Crop::Type >::const_iterator itr =
        s_cropTypeMap.find( s );

    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Crop::CROP_TO_ABBR(
    Crop::Type const& cropType )
{
    static std::map< Crop::Type, std::string > const s_cropTypeMap =
        boost::assign::map_list_of
            ( Crop::NONE, "FA" )
            ( Crop::BARLEY, "BA" )
            ( Crop::BEAN, "DB" )
            ( Crop::CANOLA, "CA" )
            ( Crop::CORN, "CG" )
            ( Crop::CORN_SWEET, "SC" )
            ( Crop::COTTON, "CT" )
            ( Crop::COVER_WHEAT_WINTER, "CC" )
            ( Crop::FLAX, "FX" )
            ( Crop::LENTILS, "LE" )
            ( Crop::LETTUCE, "LT" )
            ( Crop::MILLET, "ML" )
            ( Crop::OATS, "OT" )
            ( Crop::ONION, "ON" )
            ( Crop::PEANUT, "PN" )
            ( Crop::PEAS, "PE" )
            ( Crop::POTATO, "PT" )
            ( Crop::RICE, "RI" )
            ( Crop::RYE, "RY" )
            ( Crop::SAFFLOWER, "SL" )
            ( Crop::SORGHUM_FORAGE, "FS" )
            ( Crop::SORGHUM, "SG" )
            ( Crop::SOYBEAN, "SB" )
            ( Crop::SUGARBEET, "SU" )
            ( Crop::SUGARCANE, "SE" )
            ( Crop::SUNFLOWER, "SF" )
            ( Crop::SWITCHGRASS, "SWG" )
            ( Crop::TOMATO, "TM" )
            ( Crop::WHEAT_DURUM, "DW" )
            ( Crop::WHEAT_SPRING, "SW" )
            ( Crop::WHEAT_WINTER, "WW" );

    std::map< Crop::Type, std::string >::const_iterator itr =
        s_cropTypeMap.find( cropType );

    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
Crop::Type const& Crop::REGEX_SEARCH(
    std::string const& s )
{
    static CROP_MAP const s_cropTypeMap = boost::assign::map_list_of
        ( Crop::NONE, boost::regex( "(?=.*\\bfallow\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::BARLEY, boost::regex( "(?=.*\\bbarley\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::BEAN, boost::regex( "(?=.*\\bbean(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::CANOLA, boost::regex( "(?=.*\\bcanola\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::CORN, boost::regex( "(?=.*\\bcorn\\b)(?=.*\\bgrain\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::CORN_SWEET, boost::regex( "(?=.*\\bcorn\\b)(?=.*\\bsweet\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::COTTON, boost::regex( "(?=.*\\bcotton\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::COVER_WHEAT_WINTER, boost::regex( "(?=.*\\bcover\\b)(?=.*\\bwheat\\b)(?=.*\\bwinter\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::FLAX, boost::regex( "(?=.*\\bflax(?:seed)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::LENTILS, boost::regex( "(?=.*\\blentil(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::LETTUCE, boost::regex( "(?=.*\\blettuce\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::MILLET, boost::regex( "(?=.*\\bmillet\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::OATS, boost::regex( "(?=.*\\boat(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::ONION, boost::regex( "(?=.*\\bonion(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::PEANUT, boost::regex( "(?=.*\\bpeanut(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::PEAS, boost::regex( "(?=.*\\bpea(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::POTATO, boost::regex( "(?=.*\\bpotato(?:s|es)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::RICE, boost::regex( "(?=.*\\brice\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::RYE, boost::regex( "(?=.*\\brye\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SAFFLOWER, boost::regex( "(?=.*\\bsafflower\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SORGHUM_FORAGE, boost::regex( "(?=.*\\bsorghum\\b)(?=.*\\bforage\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SORGHUM, boost::regex( "(?=.*\\bsorghum\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SOYBEAN, boost::regex( "(?=.*\\bsoybean(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SUGARBEET, boost::regex( "(?=.*\\bsugarbeet(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SUGARCANE, boost::regex( "(?=.*\\bsugarcane\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SUNFLOWER, boost::regex( "(?=.*\\bsunflower(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::SWITCHGRASS, boost::regex( "(?=.*\\bswitchgrass(?:s)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::TOMATO, boost::regex( "(?=.*\\btomato(?:s|es)?\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::WHEAT_DURUM, boost::regex( "(?=.*\\bwheat\\b)(?=.*\\bdurum\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::WHEAT_SPRING, boost::regex( "(?=.*\\bwheat\\b)(?=.*\\bspring\\b)",
            boost::regex::perl | boost::regex_constants::icase ) )
        ( Crop::WHEAT_WINTER, boost::regex( "(?=.*\\bwheat\\b)(?=.*\\bwinter\\b)",
            boost::regex::perl | boost::regex_constants::icase ) );

    CROP_MAP::const_iterator itr = s_cropTypeMap.begin();
    for( ; itr != s_cropTypeMap.end(); ++itr )
    {
        boost::regex const& rgx = itr->second.extract< boost::regex >();
        if( boost::regex_search( s, rgx ) ) return itr->first;
    }

    return s_cropTypeMap.begin()->first;
}
////////////////////////////////////////////////////////////////////////////////
bool Crop::IsCover(
    std::string const& s )
{
    static boost::regex const rgx(
        "(?=.*\\bcover\\b)|(?=.*Clover, red, released growing crop)",
        boost::regex::perl | boost::regex_constants::icase );
    return boost::regex_search( s, rgx );
}
////////////////////////////////////////////////////////////////////////////////
bool Crop::IsBarrier(
    std::string const& s )
{
    static boost::regex const rgx(
        "(?=.*\\bbarrier\\b)",
        boost::regex::perl | boost::regex_constants::icase );
    return boost::regex_search( s, rgx );
}
////////////////////////////////////////////////////////////////////////////////

} //end util
} //end leaf
