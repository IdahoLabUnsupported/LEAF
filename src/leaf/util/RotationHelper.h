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

// --- Poco Includes --- //
#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>

#include <Poco/Dynamic/Struct.h>

// --- Boost Includes --- //
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
//#include <boost/multi_index/hashed_index.hpp>
//#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
//#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <boost/noncopyable.hpp>

// --- STL Includes --- //
#include <string>
#include <map>

namespace leaf
{
namespace util
{

///Using class instead of namespace for SWIG
class LEAF_UTIL_EXPORTS Year : public boost::noncopyable
{
public:
    ///
    enum Enum
    {
        NONE = 0x0,
        ONE = 1,
        TWO = 2,
        THREE = 3,
        FOUR = 4,
        FIVE = 5,
        SIX = 6,
        LAST = 7
    };

    ///
    static unsigned int UINT(
        std::string const& d );

    ///
    static Enum ENUM(
        std::string const& d );

    ///
    static std::string Set(
        unsigned int const& y,
        std::string const& d );

protected:

private:
    ///
    Year();

    ///
    ~Year();

};

///Using class instead of namespace for SWIG
class LEAF_UTIL_EXPORTS Crop : public boost::noncopyable
{
public:
    ///
    enum Type
    {
        NONE = 0x0,         //FA
        BARLEY,             //BA
        BEAN,               //DB
        CANOLA,             //CA
        CORN,               //CG
        CORN_SWEET,         //SC
        COTTON,             //CT
        COVER_WHEAT_WINTER, //CC
        FLAX,               //FX
        LENTILS,            //LE
        LETTUCE,            //LT
        MILLET,             //ML
        OATS,               //OT
        ONION,              //ON
        PEANUT,             //PN
        PEAS,               //PE
        POTATO,             //PT
        RICE,               //RI
        RYE,                //RY
        SAFFLOWER,          //SL
        SORGHUM_FORAGE,     //FS
        SORGHUM,            //SG
        SOYBEAN,            //SB
        SUGARBEET,          //SU
        SUGARCANE,          //SE
        SUNFLOWER,          //SF
        SWITCHGRASS,        //SWG
        TOMATO,             //TM
        WHEAT_DURUM,        //DW
        WHEAT_SPRING,       //SW
        WHEAT_WINTER        //WW
    };

    ///
    static Type const& ABBR_TO_CROP(
        std::string const& s );

    ///
    static std::string const& CROP_TO_ABBR(
        Type const& cropType );

    ///
    static Type const& REGEX_SEARCH(
        std::string const& s );

    ///
    static bool IsCover(
        std::string const& s );

    ///
    static bool IsBarrier(
        std::string const& s );

protected:

private:
    ///
    Crop();

    ///
    ~Crop();

};

///
typedef std::pair< Crop::Type, Poco::Dynamic::Var > CROPVAR_PAIR;
typedef std::map< Crop::Type, Poco::Dynamic::Var > CROP_MAP;
typedef std::pair< Crop::Type, Year::Enum > CROPYEAR_PAIR;
typedef std::pair< CROPYEAR_PAIR, Poco::Dynamic::Var > CROPYEAR_VAR_PAIR;
typedef std::map< CROPYEAR_PAIR, Poco::Dynamic::Var > CROPYEAR_MAP;

typedef std::pair< Crop::Type, Poco::Dynamic::Var > CROPVAR_PAIR;
typedef std::vector< CROPVAR_PAIR > CROPVAR_VEC;

///
template< typename T >
void RotationOffset(
    std::vector< T >& rotation,
    unsigned int const& offset = 1 )
{
    std::rotate( rotation.begin(), rotation.end() - offset, rotation.end() );
}

///
template< typename T >
unsigned int RotationCheck(
    std::vector< T >& check,
    std::vector< T > const& against )
{
    unsigned int offset( 0 );
    while( check != against )
    {
        RotationOffset( check );
        ++offset;
    }

    return offset;
}

} //end util
} //end leaf
