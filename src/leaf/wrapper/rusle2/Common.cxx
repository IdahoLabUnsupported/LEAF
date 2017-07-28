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
#include <leaf/wrapper/rusle2/Common.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

typedef std::map< FilePrefix, std::string > FILEPFX_MAP;
typedef std::map< ObjectType, std::string > OBJTYPE_MAP;

////////////////////////////////////////////////////////////////////////////////
std::string const& PFX(
    FilePrefix const& filePrefix )
{
    static FILEPFX_MAP const s_filePrefixMap = boost::assign::map_list_of
        ( HASHTAG_DB, "" )
        ( HASHTAG_XML, "#XML:" )
        ( HASHTAG_SKEL, "#SKEL:" )
        ( HASHTAG_FILELIST, "#FILELIST:" )
        ( HASHTAG_FILESET, "#FILESET:" );

    FILEPFX_MAP::const_iterator itr = s_filePrefixMap.find( filePrefix );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& EXT(
    ObjectType const& objType )
{
    static OBJTYPE_MAP const s_objTypeMap = boost::assign::map_list_of
        ( OBJTYP_CLIMATE, ".cli" )
        ( OBJTYP_CONTOUR, "" )
        ( OBJTYP_MANAGEMENT, ".man" )
        ( OBJTYP_OPERATION, ".ope" )
        ( OBJTYP_RESIDUE, ".rsd" )
        ( OBJTYP_SLOPE, ".pro" )
        ( OBJTYP_SOIL, ".soi" )
        ( OBJTYP_STRIPBARRIER, "" )
        ( OBJTYP_VEGETATION, ".veg" );

    OBJTYPE_MAP::const_iterator itr = s_objTypeMap.find( objType );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& NAME(
    ObjectType const& objType )
{
    static OBJTYPE_MAP const s_objTypeMap = boost::assign::map_list_of
        ( OBJTYP_CLIMATE, "CLIMATE" )
        ( OBJTYP_CONTOUR, "CONTOUR_SYSTEM" )
        ( OBJTYP_MANAGEMENT, "MANAGEMENT" )
        ( OBJTYP_OPERATION, "OPERATION" )
        ( OBJTYP_RESIDUE, "RESIDUE" )
        ( OBJTYP_SLOPE, "SLOPE" )
        ( OBJTYP_SOIL, "SOIL" )
        ( OBJTYP_STRIPBARRIER, "STRIP_BARRIER_SYSTEM" )
        ( OBJTYP_VEGETATION, "VEGETATION" );

    OBJTYPE_MAP::const_iterator itr = s_objTypeMap.find( objType );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& PTR(
    ObjectType const& objType )
{
    static OBJTYPE_MAP const s_objTypeMap = boost::assign::map_list_of
        ( OBJTYP_CLIMATE, "CLIMATE_PTR" )
        ( OBJTYP_CONTOUR, "CONTOUR_SYSTEM_PTR" )
        ( OBJTYP_MANAGEMENT, "MAN_BASE_PTR" )
        ( OBJTYP_OPERATION, "OP_PTR" )
        ( OBJTYP_RESIDUE, "" )
        ( OBJTYP_SLOPE, "" )
        ( OBJTYP_SOIL, "SOIL_PTR" )
        ( OBJTYP_STRIPBARRIER, "STRIP_BARRIER_SYSTEM_PTR" )
        ( OBJTYP_VEGETATION, "VEG_PTR" );

    OBJTYPE_MAP::const_iterator itr = s_objTypeMap.find( objType );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& TBL(
    ObjectType const& objType )
{
    static OBJTYPE_MAP const s_objTypeMap = boost::assign::map_list_of
        ( OBJTYP_CLIMATE, "climates" )
        ( OBJTYP_CONTOUR, "contour-systems" )
        ( OBJTYP_MANAGEMENT, "managements" )
        ( OBJTYP_OPERATION, "operations" )
        ( OBJTYP_RESIDUE, "residues" )
        ( OBJTYP_SLOPE, "profiles" )
        ( OBJTYP_SOIL, "soils" )
        ( OBJTYP_STRIPBARRIER, "strip-barrier-systems" )
        ( OBJTYP_VEGETATION, "vegetations" );

    OBJTYPE_MAP::const_iterator itr = s_objTypeMap.find( objType );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
std::string FixUpPath(
    std::string const& s,
    ObjectType const& objType,
    FilePrefix const& filePrefix )
{
    //
    std::string path = s;
    std::replace( path.begin(), path.end(), '/', '\\' );

    if( !!filePrefix )
    {
        if( ( filePrefix == HASHTAG_SKEL ) && ( objType != OBJTYP_MANAGEMENT ) )
        {
            throw std::runtime_error(
                "leaf::wrapper::rusle2::Common::FixUpPath - "
                "Only management objects can be converted to .skel format" );
        }
        path.insert( 0, PFX( filePrefix ) );
    }
    else if( !!objType )
    {
        std::string tbl = TBL( objType );
        std::string tmp = path.substr( 0, tbl.length() );
        if( tmp != tbl )
        {
            path.insert( 0, tbl + '\\' );
        }
    }

    return path;
}
////////////////////////////////////////////////////////////////////////////////

} //end rusle2
} //end wrapper
} //end leaf
