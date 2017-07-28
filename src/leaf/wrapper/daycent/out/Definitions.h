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
#include <leaf/util/DataHelper.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>

// --- STL Includes --- //
#include <map>
#include <string>
#include <vector>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace out
{

///
enum FILE_NAMES
{
    NONE =                                                          0x00000000,
    BIO =                                                           0x00000001,
    CO2 =                                                           0x00000002,
    DAILY =                                                         0x00000004,
    DC_SIP =                                                        0x00000008,
    DEADC =                                                         0x00000010,
    DELS =                                                          0x00000020,
    DN2LYR =                                                        0x00000040,
    DN2OLYR =                                                       0x00000080,
    GRESP =                                                         0x00000100,
    LIVEC =                                                         0x00000200,
    MRESP =                                                         0x00000400,
    NFLUX =                                                         0x00000800,
    SOILC =                                                         0x00001000,
    SOILN =                                                         0x00002000,
    SOILTAVG =                                                      0x00004000,
    SOILTMAX =                                                      0x00008000,
    SOILTMIN =                                                      0x00010000,
    STEMP_DX =                                                      0x00020000,
    SUMMARY =                                                       0x00040000,
    SYSC =                                                          0x00080000,
    TGMONTH =                                                       0x00100000,
    VSWC =                                                          0x00200000,
    WATRBAL =                                                       0x00400000,
    WFLUX =                                                         0x00800000,
    WFPS =                                                          0x01000000,
    YEAR_SUMMARY =                                                  0x02000000,

    DEFAULT = DAILY | NFLUX | SUMMARY,
    OUTPUT =
        BIO | CO2 | DC_SIP | DEADC | DELS | DN2LYR | DN2OLYR | GRESP | LIVEC |
        MRESP | SOILC | SOILN | SOILTAVG | SOILTMAX | SOILTMIN | STEMP_DX |
        SYSC | TGMONTH | VSWC | WATRBAL | WFLUX | WFPS | YEAR_SUMMARY,
    ALL = DEFAULT | OUTPUT,

    TIME_DAILY =
        BIO | CO2 | DAILY | DC_SIP | DEADC | DELS | DN2LYR | DN2OLYR | GRESP |
        LIVEC | MRESP | NFLUX | SOILC | SOILN | SOILTAVG | SOILTMAX | SOILTMIN |
        STEMP_DX | SUMMARY | SYSC | VSWC | WATRBAL | WFLUX | WFPS,
    TIME_MONTHLY = TGMONTH,
    TIME_YEARLY = YEAR_SUMMARY
};

///
std::string const OutFilesName = "outfiles.in";

///
struct ID{};
struct FN{};
typedef std::pair< FILE_NAMES, std::string > LU;
#if( _MSC_VER >= 1600 && _MSC_VER < 1700 )
typedef LU::_Mybase LU_Class;
#else
typedef LU LU_Class;
#endif
typedef boost::multi_index_container<
    LU,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< ID >,
            BOOST_MULTI_INDEX_MEMBER( LU_Class, FILE_NAMES, first )
        >,
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< FN >,
            BOOST_MULTI_INDEX_MEMBER( LU_Class, std::string, second )
        >
    >
> LU_MAP;
typedef LU_MAP::index< ID >::type LuById;
typedef LU_MAP::index< FN >::type LuByFn;

///
LU_MAP const FileNames =
    boost::assign::list_of
    //Default
    ( std::make_pair( DAILY, "daily.out" ) )
    ( std::make_pair( NFLUX, "nflux.out" ) )
    ( std::make_pair( SUMMARY, "summary.out" ) )
    //Everything else
    ( std::make_pair( BIO, "bio.out" ) )
    ( std::make_pair( CO2, "co2.out" ) )
    ( std::make_pair( DC_SIP, "dc_sip.csv" ) )
    ( std::make_pair( DEADC, "deadc.out" ) )
    ( std::make_pair( DELS, "dels.out" ) )
    ( std::make_pair( DN2LYR, "dN2lyr.out" ) )
    ( std::make_pair( DN2OLYR, "dN2Olyr.out" ) )
    ( std::make_pair( GRESP, "gresp.out" ) )
    ( std::make_pair( LIVEC, "livec.out" ) )
    ( std::make_pair( MRESP, "mresp.out" ) )
    ( std::make_pair( SOILC, "soilc.out" ) )
    ( std::make_pair( SOILN, "soiln.out" ) )
    ( std::make_pair( SOILTAVG, "soiltavg.out" ) )
    ( std::make_pair( SOILTMAX, "soiltmax.out" ) )
    ( std::make_pair( SOILTMIN, "soiltmin.out" ) )
    ( std::make_pair( STEMP_DX, "stemp_dx.out" ) )
    ( std::make_pair( SYSC, "sysc.out" ) )
    ( std::make_pair( TGMONTH, "tgmonth.out" ) )
    ( std::make_pair( VSWC, "vswc.out" ) )
    ( std::make_pair( WATRBAL, "watrbal.out" ) )
    ( std::make_pair( WFLUX, "wflux.out" ) )
    ( std::make_pair( WFPS, "wfps.out" ) )
    ( std::make_pair( YEAR_SUMMARY, "year_summary.out" ) );

///
FILE_NAMES const& Id(
    std::string const& filename );

///
std::string const& FileName(
    FILE_NAMES const& id );

} //end out
} //end daycent
} //end wrapper
} //end leaf
