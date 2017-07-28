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
#include <leaf/wrapper/rusle2/Common.h>

// --- STL Includes --- //
#include <vector>

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

///Convert a single Rusle2 GDB file to its constituent XML file
LEAF_RUSLE2_EXPORTS
bool GdbFileToXml(
    std::string const& gdbFilePath,
    ObjectType const& objType,
    std::string const& virtualFilePath,
    std::string const& newFilePath );

///Convert a Rusle2 GDB to its constituent XML files
LEAF_RUSLE2_EXPORTS
bool GdbToXml(
    std::string const& gdbFilePath,
    ObjectType const& objType,
    std::string const& newFilePath,
    bool const& useDbStruct = false );

///Search for names of a certain object type in specified gdb
LEAF_RUSLE2_EXPORTS
std::vector< std::string > GdbSearch(
    std::string const& gdbFilePath,
    ObjectType const& objType,
    std::string const& whereClause );

///Trim all names of a certain object type in specified gdb
LEAF_RUSLE2_EXPORTS
bool GdbTrim(
    std::string const& gdbFilePath,
    ObjectType const& objType );

///
//LEAF_RUSLE2_EXPORTS
//bool Gdb_Open(
    //std::string const& gdbFilePath );

///
LEAF_RUSLE2_EXPORTS
std::string DecodeXML(
    std::string const& pszEncoded );

///
LEAF_RUSLE2_EXPORTS
std::string DecodeXML(
    std::string const& pszEncoded,
    unsigned int const& nFlags );

///
LEAF_RUSLE2_EXPORTS
bool IsDecodedXML(
    std::string const& pszEncoded );

///
LEAF_RUSLE2_EXPORTS
bool IsDecodedXML(
    std::string const& pszEncoded,
    unsigned int const& nFlags );

///
LEAF_RUSLE2_EXPORTS
std::string EncodeXML(
    std::string const& pszDecoded );

///
LEAF_RUSLE2_EXPORTS
std::string EncodeXML(
    std::string const& pszDecoded,
    unsigned int const& nFlags );

///
LEAF_RUSLE2_EXPORTS
bool IsEncodedXML(
    std::string const& pszEncoded );

///
LEAF_RUSLE2_EXPORTS
bool IsEncodedXML(
    std::string const& pszEncoded,
    unsigned int const& nFlags );

} //end rusle2
} //end wrapper
} //end leaf
