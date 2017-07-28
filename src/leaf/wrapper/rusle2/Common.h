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

// --- STL Includes --- //
#include <string>

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

///
enum FilePrefix
{
    HASHTAG_DB = 0x0,
    HASHTAG_XML,
    HASHTAG_SKEL,
    HASHTAG_FILELIST,
    HASHTAG_FILESET
};

///
LEAF_RUSLE2_EXPORTS
std::string const& PFX(
    FilePrefix const& filePrefix );

///
enum ObjectType
{
    OBJTYP_NULL = 0x0,
    OBJTYP_CLIMATE,
    OBJTYP_CONTOUR,
    OBJTYP_MANAGEMENT,
    OBJTYP_OPERATION,
    OBJTYP_RESIDUE,
    OBJTYP_SLOPE,
    OBJTYP_SOIL,
    OBJTYP_STRIPBARRIER,
    OBJTYP_VEGETATION
};

///
LEAF_RUSLE2_EXPORTS
std::string const& EXT(
    ObjectType const& objType );

///
LEAF_RUSLE2_EXPORTS
std::string const& NAME(
    ObjectType const& objType );

///
LEAF_RUSLE2_EXPORTS
std::string const& PTR(
    ObjectType const& objType );

///
LEAF_RUSLE2_EXPORTS
std::string const& TBL(
    ObjectType const& objType );

///
LEAF_RUSLE2_EXPORTS
std::string FixUpPath(
    std::string const& s,
    ObjectType const& objType = OBJTYP_NULL,
    FilePrefix const& filePrefix = HASHTAG_DB );

} //end rusle2
} //end wrapper
} //end leaf
